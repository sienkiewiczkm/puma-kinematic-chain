#include "Application.hpp"

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"

#include "fw/Common.hpp"
#include "fw/DebugShapes.hpp"
#include "fw/Resources.hpp"
#include "fw/TextureUtils.hpp"

#include "PumaFastAnimator.hpp"
#include "PumaStraightLineAnimator.hpp"

namespace application
{

Application::Application():
    _enableCameraRotations{false},
    _cameraRotationSensitivity{0.2, 0.2}
{
}

Application::~Application()
{
}

void Application::onAnimationRequest(
    const PumaInverseKinematicsInput &start,
    const PumaInverseKinematicsInput &end
)
{
    _inAnimationMode = true;

    _pumaCalculator->solveIK(start.effectorPosition, start.effectorOrientation);
    auto startConfiguration = _pumaCalculator->getConfiguration();

    _leftAnimator = std::make_shared<PumaFastAnimator>();
    _leftAnimator->setTarget(_pumaCalculator);
    _leftAnimator->startAnimation(start, end, startConfiguration);

    _rightAnimator = std::make_shared<PumaStraightLineAnimator>();
    _rightAnimator->setTarget(_alternativePumaCalculator);
    _rightAnimator->startAnimation(start, end, startConfiguration);
}

void Application::onCreate()
{
    ImGuiApplication::onCreate();

    _universalPhongEffect = std::make_shared<fw::UniversalPhongEffect>();

    _grid = std::make_shared<fw::Grid>(
        glm::ivec2{32, 32},
        glm::vec2{0.5f, 0.5f}
    );

    _frameMarker = std::make_shared<fw::FrameMarker>();
    _sphere = std::make_shared<fw::Mesh<fw::VertexNormalTexCoords>>(
        fw::createSphere(1.0f, 16, 16)
    );

    _pumaCalculator = std::make_shared<PumaCalculator>();
    _pumaModel = std::make_shared<PumaModel>(_pumaCalculator);
    _configurationWindow = std::make_shared<PumaConfigurationWindow>(
        _pumaCalculator
    );

    _alternativePumaCalculator = std::make_shared<PumaCalculator>();
    _alternativePumaModel = std::make_shared<PumaModel>(
        _alternativePumaCalculator
    );

    _configurationWindow->setListener(shared_from_this());

    _camera.rotate(fw::pi()/4, -3.0*fw::pi()/4);
    _camera.setDist(3.0f);

    updateProjectionMatrix();
}

void Application::onDestroy()
{
    ImGuiApplication::onDestroy();
}

void Application::onUpdate(
    const std::chrono::high_resolution_clock::duration& deltaTime
)
{
    ImGuiApplication::onUpdate(deltaTime);

    if (!_inAnimationMode)
    {
        _configurationWindow->updateInterface();
    }
    else
    {
        std::chrono::duration<double> deltaSeconds{deltaTime};
        _leftAnimator->update(deltaSeconds.count());
        _rightAnimator->update(deltaSeconds.count());

        showAnimationInferface();
    }
}

void Application::onRender()
{
    auto resolution = getFramebufferSize();

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!_inAnimationMode)
    {
        glViewport(0, 0, resolution.x, resolution.y);
        updateProjectionMatrix();
        render(*_pumaModel.get());
    }
    else
    {
        auto aspectRatio = static_cast<float>(resolution.x/2) / resolution.y;
        _projectionMatrix = glm::perspective(45.0f, aspectRatio, 0.5f, 100.0f);

        glViewport(0, 0, resolution.x/2, resolution.y);
        render(*_pumaModel.get());

        glViewport(resolution.x/2, 0, resolution.x/2, resolution.y);
        render(*_alternativePumaModel.get());
    }

    ImGuiApplication::onRender();
}

void Application::render(const PumaModel& puma)
{
    glEnable(GL_DEPTH_TEST);

    _universalPhongEffect->begin();
    _universalPhongEffect->setProjectionMatrix(_projectionMatrix);
    _universalPhongEffect->setViewMatrix(_camera.getViewMatrix());
    _universalPhongEffect->setModelMatrix({});
    _universalPhongEffect->setSolidColor(glm::vec4{0.35f, 0.35f, 0.35f, 1.0f});
    _grid->render();
    _universalPhongEffect->end();

    std::vector<fw::GeometryChunk> sceneChunks;

    auto pumaChunks = puma.getGeometryChunks();
    sceneChunks.insert(
        std::end(sceneChunks),
        std::make_move_iterator(std::begin(pumaChunks)),
        std::make_move_iterator(std::end(pumaChunks))
    );

    auto ikTargetScaling = glm::scale(glm::mat4{}, glm::vec3{0.3f, 0.3f, 0.3f});
    auto ikTargetRotation = glm::mat4_cast(
        _configurationWindow->getEffectorOrientation()
    );
    auto ikTargetTranslation = glm::translate(
        glm::mat4{},
        _configurationWindow->getEffectorPosition()
    );

    _frameMarker->setTransform(
        ikTargetTranslation * ikTargetRotation * ikTargetScaling
    );

    auto frameChunks = _frameMarker->getGeometryChunks();
    sceneChunks.insert(
        std::end(sceneChunks),
        std::make_move_iterator(std::begin(frameChunks)),
        std::make_move_iterator(std::end(frameChunks))
    );

    for (const auto& debugPoint: puma.getCalculator()->getDebugPoints())
    {
        auto modelMatrix = glm::translate(glm::mat4{}, debugPoint)
            * glm::scale(glm::mat4{}, glm::vec3{0.1f, 0.1f, 0.1f});
        sceneChunks.push_back({_sphere, nullptr, modelMatrix});
    }

    for (const auto& chunk: sceneChunks)
    {
        _universalPhongEffect->setLightDirection({-1, 1, 0});

        if (chunk.getMaterial() != nullptr)
        {
            _universalPhongEffect->setSolidColor(
                chunk.getMaterial()->getBaseAlbedoColor()
            );
        }
        else
        {
            _universalPhongEffect->setSolidColor({1.0f, 0.0f, 1.0f});
        }

        _universalPhongEffect->begin();
        _universalPhongEffect->setProjectionMatrix(_projectionMatrix);
        _universalPhongEffect->setViewMatrix(_camera.getViewMatrix());
        _universalPhongEffect->setModelMatrix(chunk.getModelMatrix());
        chunk.getMesh()->render();
        _universalPhongEffect->end();
    }
}

bool Application::onMouseButton(int button, int action, int mods)
{
    if (ImGuiApplication::onMouseButton(button, action, mods)) { return true; }

    if (GLFW_MOUSE_BUTTON_LEFT == button)
    {
        _enableCameraRotations = GLFW_PRESS == action;
    }

    return true;
}

bool Application::onMouseMove(glm::dvec2 newPosition)
{
    if (ImGuiApplication::onMouseMove(newPosition)) { return true; }

    if (_enableCameraRotations)
    {
        auto movement = getMouseMovement() * _cameraRotationSensitivity;
        _camera.rotate(movement.y, movement.x);
    }

    return true;
}

bool Application::onScroll(double xoffset, double yoffset)
{
    if (fw::ImGuiApplication::onScroll(xoffset, yoffset))
        return true;

    const double cMinimumDistance = 1.0;
    const double cMaximumDistance = 10.0;
    const double cZoomStep = 0.5;
    auto currentDistance = _camera.getDist();
    _camera.setDist(
        std::min(
            std::max(currentDistance + cZoomStep * yoffset, cMinimumDistance),
            cMaximumDistance
        )
    );

    return true;
}

bool Application::onResize()
{
    updateProjectionMatrix();
    return true;
}

void Application::showAnimationInferface()
{
    if (!ImGui::Begin("Animation"))
    {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Play"))
    {
        _leftAnimator->play();
        _rightAnimator->play();
    }

    ImGui::SameLine();

    if (ImGui::Button("Pause"))
    {
        _leftAnimator->pause();
        _rightAnimator->pause();
    }

    ImGui::SameLine();

    if (ImGui::Button("Restart"))
    {
        _leftAnimator->restart();
        _rightAnimator->restart();
    }

    float animationDuration = static_cast<float>(_leftAnimator->getDuration());
    ImGui::SliderFloat("Duration", &animationDuration, 0.01f, 100.0f);
    _leftAnimator->setDuration(animationDuration);
    _rightAnimator->setDuration(animationDuration);

    float fraction = static_cast<float>(
        _leftAnimator->getElapsed() / _leftAnimator->getDuration()
    );

    ImGui::ProgressBar(fraction, ImVec2(-1, 0), "Animation progress");

    if (ImGui::Button("Quit animation mode"))
    {
        _inAnimationMode = false;
        _leftAnimator = nullptr;
        _rightAnimator = nullptr;
    }

    ImGui::End();
}

void Application::updateProjectionMatrix()
{
    auto windowSize = getWindowSize();
    auto aspectRatio = static_cast<float>(windowSize.x) / windowSize.y;
    _projectionMatrix = glm::perspective(45.0f, aspectRatio, 0.5f, 100.0f);
}

}

#include "Application.hpp"

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"

#include "fw/Common.hpp"
#include "fw/DebugShapes.hpp"
#include "fw/Resources.hpp"
#include "fw/TextureUtils.hpp"

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

void Application::onCreate()
{
    ImGuiApplication::onCreate();

    _universalPhongEffect = std::make_shared<fw::UniversalPhongEffect>();

    _grid = std::make_shared<fw::Grid>(
        glm::ivec2{32, 32},
        glm::vec2{0.5f, 0.5f}
    );

    _pumaModel = std::make_shared<PumaModel>();
    _configurationWindow = std::make_shared<PumaConfigurationWindow>();

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

    _configurationWindow->updateInterface();
    _pumaModel->setConfiguration(
        _configurationWindow->getConfiguration()
    );

    ImGui::ShowTestWindow();
}

void Application::onRender()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    _universalPhongEffect->begin();
    _universalPhongEffect->setProjectionMatrix(_projectionMatrix);
    _universalPhongEffect->setViewMatrix(_camera.getViewMatrix());
    _universalPhongEffect->setModelMatrix({});
    _grid->render();
    _universalPhongEffect->end();

    for (const auto& chunk: _pumaModel->getGeometryChunks())
    {
        _universalPhongEffect->setLightDirection({-1, 1, 0});
        _universalPhongEffect->setSolidColor(
            chunk.getMaterial()->getBaseAlbedoColor()
        );

        _universalPhongEffect->begin();
        // todo: standarize a way to change uniforms
        _universalPhongEffect->setProjectionMatrix(_projectionMatrix);
        _universalPhongEffect->setViewMatrix(_camera.getViewMatrix());
        _universalPhongEffect->setModelMatrix(chunk.getModelMatrix());
        chunk.getMesh()->render();
        _universalPhongEffect->end();
    }

    ImGuiApplication::onRender();
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

void Application::updateProjectionMatrix()
{
    auto windowSize = getWindowSize();
    auto aspectRatio = static_cast<float>(windowSize.x) / windowSize.y;
    _projectionMatrix = glm::perspective(45.0f, aspectRatio, 0.5f, 100.0f);
}

}

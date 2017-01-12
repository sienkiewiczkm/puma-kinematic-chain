#pragma once

#include <memory>

#include "glm/glm.hpp"

#include "fw/FrameMarker.hpp"
#include "fw/Grid.hpp"
#include "fw/ImGuiApplication.hpp"
#include "fw/Mesh.hpp"
#include "fw/OrbitingCamera.hpp"
#include "fw/TexturedPhongEffect.hpp"
#include "fw/UniversalPhongEffect.hpp"
#include "fw/Vertices.hpp"

#include "PumaAnimator.hpp"
#include "PumaCalculator.hpp"
#include "PumaConfigurationWindow.hpp"
#include "PumaModel.hpp"

namespace application
{

class Application:
    public fw::ImGuiApplication,
    public IPumaConfigurationEventListener,
    public std::enable_shared_from_this<Application>
{
public:
    Application();
    virtual ~Application();

    void onAnimationRequest(
        const PumaInverseKinematicsInput &start,
        const PumaInverseKinematicsInput &end
    ) override;

protected:
    virtual void onCreate() override;
    virtual void onDestroy() override;
    virtual void onUpdate(
        const std::chrono::high_resolution_clock::duration& deltaTime
    ) override;
    virtual void onRender() override;

    virtual bool onMouseButton(int button, int action, int mods) override;
    virtual bool onMouseMove(glm::dvec2 newPosition) override;
    virtual bool onScroll(double xoffset, double yoffset) override;
    virtual bool onResize() override;

    void showAnimationInferface();
    void updateProjectionMatrix();

    void render(const PumaModel& puma);

private:
    std::shared_ptr<fw::UniversalPhongEffect> _universalPhongEffect;
    std::shared_ptr<fw::FrameMarker> _frameMarker;
    std::shared_ptr<fw::Grid> _grid;
    std::shared_ptr<fw::Mesh<fw::VertexNormalTexCoords>> _sphere;

    std::shared_ptr<PumaCalculator> _pumaCalculator;
    std::shared_ptr<PumaCalculator> _alternativePumaCalculator;

    std::shared_ptr<PumaModel> _pumaModel;
    std::shared_ptr<PumaModel> _alternativePumaModel;

    std::shared_ptr<PumaConfigurationWindow> _configurationWindow;

    bool _inAnimationMode;
    std::shared_ptr<PumaAnimator> _leftAnimator;
    std::shared_ptr<PumaAnimator> _rightAnimator;

    bool _displayLastIkPoints;

    fw::OrbitingCamera _camera;
    glm::mat4 _projectionMatrix;
    bool _enableCameraRotations;

    glm::dvec2 _cameraRotationSensitivity;
};

}

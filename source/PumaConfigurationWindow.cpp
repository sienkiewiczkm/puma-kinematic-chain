#include "PumaConfigurationWindow.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "PumaIKSolver.hpp"
#include "PumaFastAnimator.hpp"

namespace application
{

PumaInverseKinematicsInput::PumaInverseKinematicsInput():
    effectorPosition{},
    effectorOrientation{}
{
}

PumaInverseKinematicsInput::PumaInverseKinematicsInput(
    glm::vec3 effectorPosition,
    glm::quat effectorOrientation
):
    effectorPosition{effectorPosition},
    effectorOrientation{effectorOrientation}
{
}

PumaConfigurationWindow::PumaConfigurationWindow(
    const std::shared_ptr<PumaCalculator>& calculator
):
    _calculator{calculator},
    _startInputAvailable{false},
    _endInputAvailable{false}
{
    _calculator->updateMatrices();
    auto effectorStart =
        _calculator->getEffectorMatrix() * glm::vec4{0, 0, 0, 1.0};
    _effectorPosition = effectorStart;
    _calculator->solveIK(_effectorPosition, _effectorOrientation);
}

void PumaConfigurationWindow::setListener(
    const std::shared_ptr<IPumaConfigurationEventListener>& listener
)
{
    _listener = listener;
}

void PumaConfigurationWindow::updateInterface()
{
    if (!ImGui::Begin("Puma Configuration"))
    {
        ImGui::End();
        return;
    }

    auto armsProperties = _calculator->getArmsProperties();
    auto configuration = _calculator->getConfiguration();
    bool recalculateInverseKinematics = false;

    if (ImGui::CollapsingHeader("PUMA Properties"))
    {
        ImGui::DragFloat("Arm 1 length", &armsProperties[0], 0.01f);
        ImGui::DragFloat("Arm 3 length", &armsProperties[1], 0.01f);
        ImGui::DragFloat("Arm 4 length", &armsProperties[2], 0.01f);
    }

    if (ImGui::CollapsingHeader("Forward Kinematics"))
    {
        ImGui::DragFloat("Arm extension", &configuration.extension, 0.01f);
        ImGui::DragFloat("Angle 1", &configuration.alpha[0], 0.01f);
        ImGui::DragFloat("Angle 2", &configuration.alpha[1], 0.01f);
        ImGui::DragFloat("Angle 3", &configuration.alpha[2], 0.01f);
        ImGui::DragFloat("Angle 4", &configuration.alpha[3], 0.01f);
        ImGui::DragFloat("Angle 5", &configuration.alpha[4], 0.01f);
    }

    _calculator->setArmsProperties(armsProperties);
    _calculator->setConfiguration(configuration);

    if (ImGui::CollapsingHeader("Inverse Kinematics"))
    {

        if (ImGui::DragFloat3(
            "Effector position",
            glm::value_ptr(_effectorPosition),
            0.01f
        ))
        {
            recalculateInverseKinematics = true;
        }

        if (ImGui::DragFloat4(
            "Effector orientation",
            glm::value_ptr(_effectorOrientation),
            0.01f
        ))
        {
            recalculateInverseKinematics = true;
            _effectorOrientation = glm::normalize(_effectorOrientation);
        }
    }

    if (ImGui::CollapsingHeader("Animation"))
    {

        if (ImGui::Button("Set current IK problem as start"))
        {
            _startInput = PumaInverseKinematicsInput{
                _effectorPosition,
                _effectorOrientation
            };

            _startInputAvailable = true;
        }

        if (!_startInputAvailable)
        {
            ImGui::SameLine();
            ImGui::TextColored(
                ImVec4(255, 0, 0, 255),
                "Start IK inavailable."
            );
        }
        else
        {
            ImGui::SameLine();
            if (ImGui::Button("Load##start"))
            {
                _effectorPosition = _startInput.effectorPosition;
                _effectorOrientation = _startInput.effectorOrientation;
                recalculateInverseKinematics = true;
            }
        }

        if (ImGui::Button("Set current IK problem as end"))
        {
            _endInput = PumaInverseKinematicsInput{
                _effectorPosition,
                _effectorOrientation
            };

            _endInputAvailable = true;
        }

        if (!_endInputAvailable)
        {
            ImGui::SameLine();
            ImGui::TextColored(
                ImVec4(255, 0, 0, 255),
                "End IK inavailable."
            );
        }
        else
        {
            ImGui::SameLine();
            if (ImGui::Button("Load##end"))
            {
                _effectorPosition = _endInput.effectorPosition;
                _effectorOrientation = _endInput.effectorOrientation;
                recalculateInverseKinematics = true;
            }
        }

        if (_startInputAvailable
            && _endInputAvailable)
        {
            if (ImGui::Button("Go to side-by side play mode"))
            {
                if (_listener != nullptr)
                {
                    _listener->onAnimationRequest(_startInput, _endInput);
                }
            }
        }
    }

    if (recalculateInverseKinematics)
    {
        _calculator->solveIK(_effectorPosition, _effectorOrientation);
    }

    ImGui::End();
}

glm::vec3 PumaConfigurationWindow::getEffectorPosition() const
{
    return _effectorPosition;
}

glm::quat PumaConfigurationWindow::getEffectorOrientation() const
{
    return glm::normalize(_effectorOrientation);
}

}

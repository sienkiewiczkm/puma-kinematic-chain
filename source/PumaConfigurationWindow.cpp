#include "PumaConfigurationWindow.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "PumaIKSolver.hpp"

namespace application
{

PumaConfigurationWindow::PumaConfigurationWindow(
    const std::shared_ptr<PumaCalculator>& calculator
):
    _calculator{calculator}
{
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
        bool recalculateInverseKinematics = false;

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

        if (recalculateInverseKinematics)
        {
            ImGui::Text("IK recalculation.");
            _calculator->solveIK(_effectorPosition, _effectorOrientation);
        }
    }

    ImGui::End();
}

glm::vec3 PumaConfigurationWindow::getEffectorPosition() const
{
    return _effectorPosition;
}

glm::quat PumaConfigurationWindow::getEffectorOrientation() const
{
    return _effectorOrientation;
}

}

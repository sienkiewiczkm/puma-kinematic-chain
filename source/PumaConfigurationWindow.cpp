#include "PumaConfigurationWindow.hpp"
#include "imgui.h"

namespace application
{

PumaConfigurationWindow::PumaConfigurationWindow():
    _configuration{},
    _armsProperties{1.0f, 0.6f, 0.6f}
{
}

void PumaConfigurationWindow::setConfiguration(
    const PumaConfiguration& configuration
)
{
    _configuration = configuration;
}

const PumaConfiguration& PumaConfigurationWindow::getConfiguration() const
{
    return _configuration;
}

void PumaConfigurationWindow::setArmsProperties(
    const std::array<float, 3>& armsProperties
)
{
    _armsProperties = armsProperties;
}

const std::array<float, 3>& PumaConfigurationWindow::getArmsProperties() const
{
    return _armsProperties;
}

void PumaConfigurationWindow::updateInterface()
{
    if (!ImGui::Begin("Puma Configuration"))
    {
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader("PUMA Properties"))
    {
        ImGui::DragFloat("Arm 1 length", &_armsProperties[0], 0.01f);
        ImGui::DragFloat("Arm 3 length", &_armsProperties[1], 0.01f);
        ImGui::DragFloat("Arm 4 length", &_armsProperties[2], 0.01f);
    }

    if (ImGui::CollapsingHeader("Forward Kinematics"))
    {
        ImGui::DragFloat("Arm extension", &_configuration.extension, 0.01f);
        ImGui::DragFloat("Angle 1", &_configuration.alpha[0], 0.01f);
        ImGui::DragFloat("Angle 2", &_configuration.alpha[1], 0.01f);
        ImGui::DragFloat("Angle 3", &_configuration.alpha[2], 0.01f);
        ImGui::DragFloat("Angle 4", &_configuration.alpha[3], 0.01f);
        ImGui::DragFloat("Angle 5", &_configuration.alpha[4], 0.01f);
    }

    ImGui::End();
}

}

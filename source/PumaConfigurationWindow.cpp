#include "PumaConfigurationWindow.hpp"
#include "imgui.h"

namespace application
{

PumaConfigurationWindow::PumaConfigurationWindow():
    _configuration{}
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

void PumaConfigurationWindow::updateInterface()
{
    if (!ImGui::Begin("Puma Configuration"))
    {
        ImGui::End();
        return;
    }

    ImGui::DragFloat("Arm extension", &_configuration.extension, 0.01f);
    ImGui::DragFloat("Angle 1", &_configuration.alpha[0], 0.01f);
    ImGui::DragFloat("Angle 2", &_configuration.alpha[1], 0.01f);
    ImGui::DragFloat("Angle 3", &_configuration.alpha[2], 0.01f);
    ImGui::DragFloat("Angle 4", &_configuration.alpha[3], 0.01f);
    ImGui::DragFloat("Angle 5", &_configuration.alpha[4], 0.01f);

    ImGui::End();
}

}

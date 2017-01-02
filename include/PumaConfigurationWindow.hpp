#pragma once

#include "PumaModel.hpp"

namespace application
{

class PumaConfigurationWindow
{
public:
    PumaConfigurationWindow();

    void setConfiguration(const PumaConfiguration& configuration);
    const PumaConfiguration& getConfiguration() const;

    void setArmsProperties(const std::array<float, 3>& armsProperties);
    const std::array<float, 3>& getArmsProperties() const;

    void updateInterface();

private:
    std::array<float, 3> _armsProperties;
    PumaConfiguration _configuration;
};

}

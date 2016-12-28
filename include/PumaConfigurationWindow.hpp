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
    void updateInterface();

private:
    PumaConfiguration _configuration;
};

}

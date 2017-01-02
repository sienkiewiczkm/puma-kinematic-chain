#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "PumaModel.hpp"

namespace application
{

class PumaConfigurationWindow
{
public:
    PumaConfigurationWindow(const std::shared_ptr<PumaCalculator>& calculator);

    glm::vec3 getEffectorPosition() const;
    glm::quat getEffectorOrientation() const;

    void updateInterface();

private:
    std::shared_ptr<PumaCalculator> _calculator;
    glm::vec3 _effectorPosition;
    glm::quat _effectorOrientation;
};

}

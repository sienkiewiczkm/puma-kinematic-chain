#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "PumaModel.hpp"

namespace application
{

struct PumaInverseKinematicsInput
{
    PumaInverseKinematicsInput();
    PumaInverseKinematicsInput(
        glm::vec3 effectorPosition,
        glm::quat effectorOrientation
    );

    glm::vec3 effectorPosition;
    glm::quat effectorOrientation;
};

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

    PumaInverseKinematicsInput _currentInput;

    bool _startInputAvailable, _endInputAvailable;
    PumaInverseKinematicsInput _startInput;
    PumaInverseKinematicsInput _endInput;
};

}

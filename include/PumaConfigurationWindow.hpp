#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "PumaConfiguration.hpp"
#include "PumaModel.hpp"

namespace application
{

class IPumaConfigurationEventListener
{
public:
    virtual ~IPumaConfigurationEventListener() = default;

    virtual void onAnimationRequest(
        const PumaInverseKinematicsInput &start,
        const PumaInverseKinematicsInput &end
    ) = 0;
};

class PumaConfigurationWindow
{
public:
    PumaConfigurationWindow(const std::shared_ptr<PumaCalculator>& calculator);

    void setListener(
        const std::shared_ptr<IPumaConfigurationEventListener>& listener
    );

    glm::vec3 getEffectorPosition() const;
    glm::quat getEffectorOrientation() const;

    void updateInterface();

private:
    std::shared_ptr<IPumaConfigurationEventListener> _listener;
    std::shared_ptr<PumaCalculator> _calculator;
    glm::vec3 _effectorPosition;
    glm::quat _effectorOrientation;

    PumaInverseKinematicsInput _currentInput;

    bool _startInputAvailable, _endInputAvailable;
    PumaInverseKinematicsInput _startInput;
    PumaInverseKinematicsInput _endInput;
};

}

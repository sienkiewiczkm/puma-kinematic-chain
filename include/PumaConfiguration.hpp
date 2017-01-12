#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace application
{

struct PumaConfiguration
{
    PumaConfiguration();

    float alpha[5];
    float extension;

    static PumaConfiguration mix(
        const PumaConfiguration& lhs,
        const PumaConfiguration& rhs,
        float alpha
    );
};

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

}

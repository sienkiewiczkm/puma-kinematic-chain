#include "CommonMath.hpp"

#include <cmath>
#include "glm/glm.hpp"
#include "fw/Common.hpp"

namespace application
{

float normalizeRadians(float angle)
{
    return fmod(angle, fw::pif());
}

float mixRadians(float a, float b, float t)
{
    auto pi2 = 2.0f * fw::pif();
    auto lengthDirect = std::abs(b - a);
    auto lengthAround = std::min(a, b) + (pi2 - std::max(a, b));

    if (lengthDirect < lengthAround)
    {
        return glm::mix(a, b, t);
    }

    if (a < b)
    {
        return glm::mix(a, b - pi2, t);
    }
    else
    {
        return glm::mix(a, b + pi2, t);
    }
}

float mixDegrees(float a, float b, float m)
{
    auto lengthDirect = std::abs(b - a);
    auto lengthAround = std::min(a, b) + (360.0f - std::max(a, b));

    if (lengthDirect < lengthAround)
    {
        return glm::mix(a, b, m);
    }

    if (a < b)
    {
        return glm::mix(a, b - 360.0f, m);
    }
    else
    {
        return glm::mix(a, b + 360.0f, m);
    }
}

}

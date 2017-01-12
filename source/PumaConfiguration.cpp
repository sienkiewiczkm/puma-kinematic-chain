#include "PumaConfiguration.hpp"

namespace application
{

PumaConfiguration::PumaConfiguration():
    alpha{},
    extension{1.0f}
{
}

PumaConfiguration PumaConfiguration::mix(
    const PumaConfiguration& lhs,
    const PumaConfiguration& rhs,
    float alpha
)
{
    PumaConfiguration output{};

    for (auto i = 0; i < 5; ++i)
    {
        output.alpha[i] = glm::mix(lhs.alpha[i], rhs.alpha[i], alpha);
    }

    output.extension = glm::mix(lhs.extension, rhs.extension, alpha);

    return output;
}

}


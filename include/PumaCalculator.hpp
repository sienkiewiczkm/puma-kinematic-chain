#pragma once

#include <array>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace application
{

struct PumaConfiguration
{
    PumaConfiguration();

    float alpha[5];
    float extension;
};

class PumaCalculator
{
public:
    PumaCalculator();

    void updateMatrices();
    bool solveIK(glm::vec3 position, glm::quat orientation);

    void setArmsProperties(const std::array<float, 3>& arms);
    const std::array<float, 3>& getArmsProperties() const;

    PumaConfiguration getConfiguration() const;
    void setConfiguration(const PumaConfiguration& configuration);

    const glm::mat4& getTransform() const;
    void setTransform(const glm::mat4& modelMatrix);

    const glm::mat4& getFirstArmMatrix() const;
    const glm::mat4& getSecondArmMatrix() const;
    const glm::mat4& getThirdArmMatrix() const;
    const glm::mat4& getFourthArmMatrix() const;
    const glm::mat4& getEffectorMatrix() const;

private:
    std::array<float, 3> _armLenghts;
    PumaConfiguration _configuration;

    glm::mat4 _transform;
    glm::mat4 _firstArmMtx, _secondArmMtx, _thirdArmMtx, _fourthArmMtx;
    glm::mat4 _effectorMtx;
};

}

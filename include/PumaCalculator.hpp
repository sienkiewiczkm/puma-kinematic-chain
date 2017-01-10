#pragma once

#include <array>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "PumaConfiguration.hpp"

namespace application
{

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

    const std::vector<glm::vec3>& getDebugPoints() const;
    std::vector<glm::vec3> getRotationPoints() const;

private:
    std::array<float, 3> _armLenghts;
    PumaConfiguration _configuration;

    glm::mat4 _transform;
    glm::mat4 _firstArmMtx, _secondArmMtx, _thirdArmMtx, _fourthArmMtx;
    glm::mat4 _effectorMtx;

    std::vector<glm::vec3> _debugPoints;
};

}

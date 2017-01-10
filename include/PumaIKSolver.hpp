#pragma once

#include <array>
#include <vector>

#include "glm/glm.hpp"

#include "PumaConfiguration.hpp"

namespace application
{

class PumaIKSolver
{
public:
    PumaIKSolver();

    void setArmsProperties(const std::array<float, 3>& arms);
    bool solve(
        glm::vec3 position,
        glm::quat orientation,
        PumaConfiguration &output
    );

    std::vector<glm::vec3> getLastHelperPoints() const;


private:
    glm::vec3 solveY4(glm::vec3 planeNormal, glm::vec3 x5) const;

    PumaConfiguration buildConfiguration(
        glm::vec3 p1,
        glm::vec3 p2,
        glm::vec3 p3,
        glm::vec3 p4,
        glm::vec3 p5,
        glm::vec3 planeNormal,
        glm::vec3 x5,
        glm::vec3 y5
    );

    float angle(glm::vec3 v, glm::vec3 w, glm::vec3 planeNormal) const;

    std::vector<glm::vec3> _helperPoints;
    std::array<float, 3> _arms;
};

}

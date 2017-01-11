#include "PumaIKSolver.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "fw/Common.hpp"
#include "CommonMath.hpp"

namespace application
{

PumaIKSolver::PumaIKSolver():
    _arms{},
    _lastConfigurationAvailable{false},
    _lastPumaConfiguration{}
{
}

void PumaIKSolver::setArmsProperties(const std::array<float, 3>& arms)
{
    _arms = arms;
}

void PumaIKSolver::setLastPumaConfiguration(
    const PumaConfiguration& pumaConfiguration
)
{
    _lastConfigurationAvailable = true;
    _lastPumaConfiguration = pumaConfiguration;
}

bool PumaIKSolver::solve(
    glm::vec3 position,
    glm::quat orientation,
    PumaConfiguration &configuration
)
{
    glm::vec3 p1{0.0f, 0.0f, 0.0f};
    glm::vec3 x1{1.0f, 0.0f, 0.0f};
    glm::vec3 y1{0.0f, 1.0f, 0.0f};
    glm::vec3 z1{0.0f, 0.0f, 1.0f};

    glm::vec3 p5{position};

    auto orientationMatrix = glm::mat3_cast(orientation);
    glm::vec3 x5 = glm::column(orientationMatrix, 0);
    glm::vec3 y5 = glm::column(orientationMatrix, 1);
    glm::vec3 z5 = glm::column(orientationMatrix, 2);

    glm::vec3 p2 = p1 + y1 * _arms[0];
    glm::vec3 p4 = p5 - x5 * _arms[2];

    glm::vec3 planeNormal = glm::cross(p4 - p1, p2 - p1);
    // todo: possible failure spot: planeNormal = 0
    planeNormal = glm::normalize(planeNormal);

    auto y4 = solveY4(planeNormal, x5);
    glm::vec3 p31 = p4 + y4 * _arms[1];
    glm::vec3 p32 = p4 - y4 * _arms[1];

    auto configurationCandidatePlus =
        buildConfiguration(p1, p2, p31, p4, p5, planeNormal, x5, y5);

    auto configurationCandidateMinus =
        buildConfiguration(p1, p2, p32, p4, p5, planeNormal, x5, y5);

    if (!_lastConfigurationAvailable)
    {
        // simplification, just force choosing plus option
        _lastPumaConfiguration = configurationCandidatePlus;
        _lastConfigurationAvailable = true;
    }

    auto plusCandidateDistance = scoreConfigurationDistance(
        _lastPumaConfiguration, configurationCandidatePlus
    );

    auto minusCandidateDistance = scoreConfigurationDistance(
        _lastPumaConfiguration, configurationCandidateMinus
    );

    glm::vec3 chosenPoint{};

    if (plusCandidateDistance <= minusCandidateDistance)
    {
        configuration = configurationCandidatePlus;
        chosenPoint = p31;
    }
    else
    {
        configuration = configurationCandidateMinus;
        chosenPoint = p32;
    }

    _lastPumaConfiguration = configuration;
    _lastConfigurationAvailable = true;

    _helperPoints.clear();
    _helperPoints.push_back(p1);
    _helperPoints.push_back(p2);
    _helperPoints.push_back(chosenPoint);
    _helperPoints.push_back(p4);
    _helperPoints.push_back(p5);

    return true;
}

std::vector<glm::vec3> PumaIKSolver::getLastHelperPoints() const
{
    return _helperPoints;
}

glm::vec3 PumaIKSolver::solveY4(
    glm::vec3 planeNormal,
    glm::vec3 x5
) const
{
    glm::vec3 n{planeNormal.x, planeNormal.z, planeNormal.y};
    glm::vec3 p{x5.x, x5.z, x5.y};

    auto nx2 = n.x * n.x;
    auto ny2 = n.y * n.y;
    auto nz2 = n.z * n.z;
    auto px2 = p.x * p.x;
    auto py2 = p.y * p.y;
    auto pz2 = p.z * p.z;

    auto denominator = sqrt(
        nz2 * (px2 + py2)
        - 2 * n.x * n.z * p.x * p.z
        - 2 * n.y * p.y * (n.x * p.x + n.z * p.z)
        + ny2 * (px2 + pz2)
        + nx2 * (py2 + pz2)
    );

    auto xnominator = -n.z * p.y + n.y * p.z;
    auto ynominator = +n.z * p.x - n.x * p.z;
    auto znominator = -n.y * p.x + n.x * p.y;

    auto x = xnominator / denominator;
    auto y = ynominator / denominator;
    auto z = znominator / denominator;

    return {x, z, y};
}

PumaConfiguration PumaIKSolver::buildConfiguration(
    glm::vec3 p1,
    glm::vec3 p2,
    glm::vec3 p3,
    glm::vec3 p4,
    glm::vec3 p5,
    glm::vec3 planeNormal,
    glm::vec3 x5,
    glm::vec3 y5
)
{
    PumaConfiguration configuration{};

    float pi = fw::pif();
    float halfPi = 0.5f * fw::pif();

    configuration.alpha[0] = atan2f(-p4.z, p4.x);

    auto baseRotationMatrix = glm::rotate(
        glm::mat4{},
        configuration.alpha[0],
        glm::vec3{0.0f, 1.0f, 0.0f}
    );

    auto baseNormalPlane =
        baseRotationMatrix * glm::vec4{0.0f, 0.0f, 1.0f, 1.0f};

    configuration.alpha[1] = angle(p2 - p1, p3 - p2, baseNormalPlane) + halfPi;
    configuration.extension = glm::length(p3 - p2);

    configuration.alpha[2] = angle(p3 - p2, p4 - p3, baseNormalPlane) + halfPi;

    auto previousArmVersor = glm::normalize(p4 - p3);
    configuration.alpha[3] =
        -(angle(planeNormal, x5, previousArmVersor) + halfPi);

    configuration.alpha[4] = angle(p3 - p4, y5, x5);

    return configuration;
}

float PumaIKSolver::angle(glm::vec3 v, glm::vec3 w, glm::vec3 planeNormal) const
{
    v = glm::normalize(v);
    w = glm::normalize(w);

    float cosa = glm::dot(v, w);
    float angle = acos(cosa);
    glm::vec3 crossProduct = glm::cross(v, w);
    if (glm::dot(planeNormal, crossProduct) < 0)
    {
        angle = -angle;
    }

    return angle;
}

float PumaIKSolver::scoreConfigurationDistance(
    const PumaConfiguration &lhs,
    const PumaConfiguration &rhs
) const
{
    float distance =
        fabs(normalizeRadians(lhs.alpha[0]) - normalizeRadians(rhs.alpha[0]))
        + fabs(normalizeRadians(lhs.alpha[1]) - normalizeRadians(rhs.alpha[1]))
        + fabs(normalizeRadians(lhs.alpha[2]) - normalizeRadians(rhs.alpha[2]))
        + fabs(normalizeRadians(lhs.alpha[3]) - normalizeRadians(rhs.alpha[3]))
        + fabs(normalizeRadians(lhs.alpha[4]) - normalizeRadians(rhs.alpha[4]))
        + fabs(lhs.extension - rhs.extension);

    return distance;
}

}

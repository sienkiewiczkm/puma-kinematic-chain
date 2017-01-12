#include "PumaCalculator.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace application
{

PumaCalculator::PumaCalculator()
{
    _armLenghts[0] = 1.0f;
    _armLenghts[1] = _armLenghts[2] = 0.5f;
    _configuration.extension = 0.5f;
}

void PumaCalculator::updateMatrices()
{
    _firstArmMtx = _transform * glm::rotate(
        glm::mat4{},
        _configuration.alpha[0],
        glm::vec3{0.0f, 1.0f, 0.0f}
    );

    _secondArmMtx =
        _firstArmMtx * glm::translate(
            glm::mat4{},
            glm::vec3{0.0f, _armLenghts[0], 0.0f}
        ) * glm::rotate(
            glm::mat4{},
            _configuration.alpha[1],
            glm::vec3{0.0f, 0.0f, 1.0f}
        );

    _thirdArmMtx =
        _secondArmMtx * glm::translate(
            glm::mat4{},
            glm::vec3{_configuration.extension, 0.0f, 0.0f}
        ) * glm::rotate(
            glm::mat4{},
            _configuration.alpha[2],
            glm::vec3{0.0f, 0.0f, 1.0f}
        );

    _fourthArmMtx =
        _thirdArmMtx * glm::translate(
            glm::mat4{},
            glm::vec3{0.0f, -_armLenghts[1], 0.0f}
        ) * glm::rotate(
            glm::mat4{},
            _configuration.alpha[3],
            glm::vec3{0.0f, 1.0f, 0.0f}
        );

    _effectorMtx =
        _fourthArmMtx * glm::translate(
            glm::mat4{},
            glm::vec3{_armLenghts[2], 0.0f, 0.0f}
        ) * glm::rotate(
            glm::mat4{},
            _configuration.alpha[4],
            glm::vec3{1.0f, 0.0f, 0.0f}
        );
}

bool PumaCalculator::solveIK(glm::vec3 position, glm::quat orientation)
{
    _ikSolver.setArmsProperties(_armLenghts);
    auto result = _ikSolver.solve(position, orientation, _configuration);
    _debugPoints = _ikSolver.getLastHelperPoints();
    updateMatrices();
    return result;
}

void PumaCalculator::setArmsProperties(const std::array<float, 3>& arms)
{
    _armLenghts = arms;
}

const std::array<float, 3>& PumaCalculator::getArmsProperties() const
{
    return _armLenghts;
}

PumaConfiguration PumaCalculator::getConfiguration() const
{
    return _configuration;
}

void PumaCalculator::setConfiguration(const PumaConfiguration& configuration)
{
    _configuration = configuration;
    _ikSolver.setLastPumaConfiguration(_configuration);
    updateMatrices();
}

const glm::mat4& PumaCalculator::getTransform() const
{
    return _transform;
}

void PumaCalculator::setTransform(const glm::mat4& modelMatrix)
{
    _transform = modelMatrix;
}

const glm::mat4& PumaCalculator::getFirstArmMatrix() const
{
    return _firstArmMtx;
}

const glm::mat4& PumaCalculator::getSecondArmMatrix() const
{
    return _secondArmMtx;
}

const glm::mat4& PumaCalculator::getThirdArmMatrix() const
{
    return _thirdArmMtx;
}

const glm::mat4& PumaCalculator::getFourthArmMatrix() const
{
    return _fourthArmMtx;
}

const glm::mat4& PumaCalculator::getEffectorMatrix() const
{
    return _effectorMtx;
}

const std::vector<glm::vec3>& PumaCalculator::getDebugPoints() const
{
    return _debugPoints;
}

std::vector<glm::vec3> PumaCalculator::getRotationPoints() const
{
    std::vector<glm::vec3> rotationPoints;
    glm::vec4 zero{0.0f, 0.0f, 0.0f, 1.0f};

    rotationPoints.push_back(getFirstArmMatrix() * zero);
    rotationPoints.push_back(getSecondArmMatrix() * zero);
    rotationPoints.push_back(getThirdArmMatrix() * zero);
    rotationPoints.push_back(getFourthArmMatrix() * zero);
    rotationPoints.push_back(getEffectorMatrix() * zero);

    return rotationPoints;
}

}

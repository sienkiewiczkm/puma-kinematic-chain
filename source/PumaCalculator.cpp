#include "PumaCalculator.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace application
{

PumaConfiguration::PumaConfiguration():
    alpha{},
    extension{1.0f}
{
}

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
    return false;
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

}

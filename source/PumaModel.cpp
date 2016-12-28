#include "PumaModel.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "fw/Common.hpp"
#include "fw/DebugShapes.hpp"

namespace application
{

const float PumaModel::cArmThickness = 0.05f;

PumaModel::PumaModel()
{
    _cylinder = std::make_shared<fw::Mesh<fw::VertexNormalTexCoords>>(
        fw::createCylinder(1.0f, 1.0f, 32)
    );

    _frameMarker = std::make_shared<fw::FrameMarker>();

    _redMaterial = std::make_shared<fw::Material>();
    _greenMaterial = std::make_shared<fw::Material>();

    _redMaterial->setBaseAlbedoColor({1.0f, 0.0f, 0.0f, 1.0f});
    _greenMaterial->setBaseAlbedoColor({0.0f, 1.0f, 0.0f, 1.0f});

    _armLenghts[0] = 1.0f;
    _armLenghts[1] = _armLenghts[2] = 0.5f;
    _configuration.extension = 0.5f;
}

glm::mat4 PumaModel::getModelMatrix() const
{
    return _modelMatrix;
}

void PumaModel::setModelMatrix(const glm::mat4& modelMatrix)
{
    _modelMatrix = modelMatrix;
}

PumaConfiguration PumaModel::getConfiguration() const
{
    return _configuration;
}

void PumaModel::setConfiguration(const PumaConfiguration& configuration)
{
    _configuration = configuration;
}

void PumaModel::setArms(const std::array<float, 3>& armLenghts)
{
    _armLenghts = armLenghts;
}

std::vector<fw::GeometryChunk> PumaModel::getGeometryChunks() const
{
    std::vector<fw::GeometryChunk> chunks{};

    glm::mat4 firstArmMatrix = _modelMatrix * glm::rotate(
        glm::mat4{},
        _configuration.alpha[0],
        glm::vec3{0.0f, 1.0f, 0.0f}
    );

    glm::mat4 firstArmRenderMatrix =
        glm::translate(
            glm::mat4{},
            glm::vec3{0.0f, _armLenghts[0] * 0.5f, 0.0f}
        ) * firstArmMatrix * glm::scale(
            glm::mat4{},
            glm::vec3{cArmThickness, _armLenghts[0], cArmThickness}
        );

    chunks.push_back({_cylinder, _redMaterial, firstArmRenderMatrix});

    glm::mat4 secondArmMatrix =
        firstArmMatrix * glm::translate(
            glm::mat4{},
            glm::vec3{0.0f, _armLenghts[0], 0.0f}
        ) * glm::rotate(
            glm::mat4{},
            _configuration.alpha[1],
            glm::vec3{0.0f, 0.0f, 1.0f}
        );

    glm::mat4 secondArmRenderMatrix =
        secondArmMatrix
        * glm::translate(
            glm::mat4{},
            glm::vec3{_configuration.extension * 0.5f, 0.0f, 0.0f}
        )
        * glm::rotate(
            glm::mat4{},
            fw::pif() * 0.5f,
            glm::vec3(0.f, 0.f, 1.f)
        )
        * glm::scale(
            glm::mat4{},
            glm::vec3{cArmThickness, _configuration.extension, cArmThickness}
        );

    chunks.push_back({_cylinder, _greenMaterial, secondArmRenderMatrix});

    glm::mat4 thirdArmMatrix =
        secondArmMatrix * glm::translate(
            glm::mat4{},
            glm::vec3{_configuration.extension, 0.0f, 0.0f}
        ) * glm::rotate(
            glm::mat4{},
            _configuration.alpha[2],
            glm::vec3{0.0f, 0.0f, 1.0f}
        );
    glm::mat4 thirdArmRenderMatrix =
        thirdArmMatrix
        * glm::translate(
            glm::mat4{},
            glm::vec3{0.0f, -_armLenghts[1] * 0.5f, 0.0f}
        )
        * glm::scale(
            glm::mat4{},
            glm::vec3{cArmThickness, _armLenghts[1], cArmThickness}
        );

    chunks.push_back({_cylinder, _redMaterial, thirdArmRenderMatrix});

    glm::mat4 fourthArmMatrix =
        thirdArmMatrix
        * glm::translate(
            glm::mat4{},
            glm::vec3{0.0f, -_armLenghts[1], 0.0f}
        ) * glm::rotate(
            glm::mat4{},
            _configuration.alpha[3],
            glm::vec3{0.0f, 1.0f, 0.0f}
        );

    glm::mat4 fourthArmRenderMatrix =
        fourthArmMatrix
        * glm::translate(
            glm::mat4{},
            glm::vec3{_armLenghts[2] * 0.5f, 0.0f, 0.0f}
        )
        * glm::rotate(
            glm::mat4{},
            fw::pif() * 0.5f,
            glm::vec3(0.f, 0.f, 1.f)
        )
        * glm::scale(
            glm::mat4{},
            glm::vec3{cArmThickness, _armLenghts[2], cArmThickness}
        );

    glm::mat4 fifthArmMatrix =
        fourthArmMatrix
        * glm::translate(
            glm::mat4{},
            glm::vec3{_armLenghts[2], 0.0f, 0.0f}
        ) * glm::rotate(
            glm::mat4{},
            _configuration.alpha[4],
            glm::vec3{1.0f, 0.0f, 0.0f}
        );

    chunks.push_back({_cylinder, _greenMaterial, fourthArmRenderMatrix});

    _frameMarker->setTransform(fifthArmMatrix);
    for (const auto& chunk: _frameMarker->getGeometryChunks())
    {
        chunks.push_back(chunk);
    }

    return chunks;
}

}

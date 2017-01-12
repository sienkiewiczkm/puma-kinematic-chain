#include "PumaModel.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "fw/Common.hpp"
#include "fw/DebugShapes.hpp"

namespace application
{

const float PumaModel::cArmThickness = 0.05f;

PumaModel::PumaModel(const std::shared_ptr<PumaCalculator>& pumaCalculator):
    _calculator{pumaCalculator}
{
    _cylinder = std::make_shared<fw::Mesh<fw::VertexNormalTexCoords>>(
        fw::createCylinder(1.0f, 1.0f, 32)
    );

    _frameMarker = std::make_shared<fw::FrameMarker>();

    _redMaterial = std::make_shared<fw::Material>();
    _greenMaterial = std::make_shared<fw::Material>();

    _redMaterial->setBaseAlbedoColor({1.0f, 0.0f, 0.0f, 1.0f});
    _greenMaterial->setBaseAlbedoColor({0.0f, 1.0f, 0.0f, 1.0f});
}

const std::shared_ptr<PumaCalculator>& PumaModel::getCalculator() const
{
    return _calculator;
}

std::vector<fw::GeometryChunk> PumaModel::getGeometryChunks() const
{
    std::vector<fw::GeometryChunk> chunks{};

    glm::mat4 firstArmMatrix = _calculator->getFirstArmMatrix();
    glm::mat4 secondArmMatrix = _calculator->getSecondArmMatrix();
    glm::mat4 thirdArmMatrix = _calculator->getThirdArmMatrix();
    glm::mat4 fourthArmMatrix = _calculator->getFourthArmMatrix();
    glm::mat4 fifthArmMatrix = _calculator->getEffectorMatrix();

    auto armLenghts = _calculator->getArmsProperties();
    auto configuration = _calculator->getConfiguration();

    glm::mat4 firstArmRenderMatrix =
        glm::translate(
            glm::mat4{},
            glm::vec3{0.0f, armLenghts[0] * 0.5f, 0.0f}
        ) * firstArmMatrix * glm::scale(
            glm::mat4{},
            glm::vec3{cArmThickness, armLenghts[0], cArmThickness}
        );

    glm::mat4 secondArmRenderMatrix =
        secondArmMatrix * glm::translate(
            glm::mat4{},
            glm::vec3{configuration.extension * 0.5f, 0.0f, 0.0f}
        ) * glm::rotate(
            glm::mat4{},
            fw::pif() * 0.5f,
            glm::vec3(0.f, 0.f, 1.f)
        ) * glm::scale(
            glm::mat4{},
            glm::vec3{cArmThickness, configuration.extension, cArmThickness}
        );

    glm::mat4 thirdArmRenderMatrix =
        thirdArmMatrix * glm::translate(
            glm::mat4{},
            glm::vec3{0.0f, -armLenghts[1] * 0.5f, 0.0f}
        ) * glm::scale(
            glm::mat4{},
            glm::vec3{cArmThickness, armLenghts[1], cArmThickness}
        );

    glm::mat4 fourthArmRenderMatrix =
        fourthArmMatrix * glm::translate(
            glm::mat4{},
            glm::vec3{armLenghts[2] * 0.5f, 0.0f, 0.0f}
        ) * glm::rotate(
            glm::mat4{},
            fw::pif() * 0.5f,
            glm::vec3(0.f, 0.f, 1.f)
        ) * glm::scale(
            glm::mat4{},
            glm::vec3{cArmThickness, armLenghts[2], cArmThickness}
        );

    chunks.push_back({_cylinder, _redMaterial, firstArmRenderMatrix});
    chunks.push_back({_cylinder, _greenMaterial, secondArmRenderMatrix});
    chunks.push_back({_cylinder, _redMaterial, thirdArmRenderMatrix});
    chunks.push_back({_cylinder, _greenMaterial, fourthArmRenderMatrix});

    _frameMarker->setTransform(fifthArmMatrix);
    for (const auto& chunk: _frameMarker->getGeometryChunks())
    {
        chunks.push_back(chunk);
    }

    return chunks;
}

}

#pragma once

#include "glm/glm.hpp"
#include "fw/FrameMarker.hpp"
#include "fw/GeometryChunk.hpp"
#include "fw/Mesh.hpp"

#include <array>
#include <vector>

namespace application
{

struct PumaConfiguration
{
    float alpha[5];
    float extension;
};

class PumaModel
{
public:
    PumaModel();

    glm::mat4 getModelMatrix() const;
    void setModelMatrix(const glm::mat4& modelMatrix);

    PumaConfiguration getConfiguration() const;
    void setConfiguration(const PumaConfiguration& configuration);

    void setArms(const std::array<float, 3>& armLenghts);

    std::vector<fw::GeometryChunk> getGeometryChunks() const;

private:
    static const float cArmThickness;
    std::array<float, 3> _armLenghts;
    std::shared_ptr<fw::FrameMarker> _frameMarker;
    std::shared_ptr<fw::Mesh<fw::VertexNormalTexCoords>> _cylinder;

    std::shared_ptr<fw::Material> _redMaterial;
    std::shared_ptr<fw::Material> _greenMaterial;

    glm::mat4 _modelMatrix;
    PumaConfiguration _configuration;
};

}

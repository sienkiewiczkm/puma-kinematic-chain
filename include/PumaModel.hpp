#pragma once

#include "glm/glm.hpp"
#include "fw/FrameMarker.hpp"
#include "fw/GeometryChunk.hpp"
#include "fw/Mesh.hpp"

#include <array>
#include <memory>
#include <vector>

#include "PumaCalculator.hpp"

namespace application
{

class PumaModel
{
public:
    PumaModel(const std::shared_ptr<PumaCalculator>& calculator);
    const std::shared_ptr<PumaCalculator>& getCalculator() const;
    std::vector<fw::GeometryChunk> getGeometryChunks() const;

private:
    static const float cArmThickness;
    std::shared_ptr<fw::FrameMarker> _frameMarker;
    std::shared_ptr<fw::Mesh<fw::VertexNormalTexCoords>> _cylinder;
    std::shared_ptr<fw::Material> _redMaterial;
    std::shared_ptr<fw::Material> _greenMaterial;
    std::shared_ptr<PumaCalculator> _calculator;
};

}

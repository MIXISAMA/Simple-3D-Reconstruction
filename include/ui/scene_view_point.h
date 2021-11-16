#pragma once

#include "pch.h"
#include "scene_view.h"

namespace mixi
{
namespace s3r
{


class SceneViewPoint : public SceneView
{
public:

    using Ptr = std::shared_ptr<SceneViewPoint>;

    SceneViewPoint(
        const VertexArray* vertexArray_ = nullptr,
        const Program* program_ = nullptr
    );
    ~SceneViewPoint();

    float pointSize;

protected:

    void onAdjustModel_() override;
    
    void onAdjustCamera_() override;

    void onUseProgram_() override;

};


}
} // namespace mixi
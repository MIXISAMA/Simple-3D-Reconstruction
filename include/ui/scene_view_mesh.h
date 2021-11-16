#pragma once

#include "pch.h"
#include "scene_view.h"

namespace mixi
{
namespace s3r
{


class SceneViewMesh : public SceneView
{
public:

    using Ptr = std::shared_ptr<SceneViewMesh>;

    SceneViewMesh(
        const VertexArray* vertexArray_ = nullptr,
        const Program* program_ = nullptr
    );
    ~SceneViewMesh() = default;

    float lightPos[3]; 
    float lightColor[3];
    float objectColor[3];

protected:

    glm::vec3 viewPos_;

    void onAdjustModel_() override;

    void onAdjustCamera_() override;

    void onUseProgram_() override;

};


}
} // namespace mixi
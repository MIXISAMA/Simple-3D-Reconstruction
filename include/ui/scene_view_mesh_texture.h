#pragma once

#include "pch.h"
#include "scene_view.h"

#include "render/texture.h"

namespace mixi
{
namespace s3r
{


class SceneViewMeshTexture : public SceneView
{
public:

    using Ptr = std::shared_ptr<SceneViewMeshTexture>;

    SceneViewMeshTexture(
        const VertexArray* vertexArray_ = nullptr,
        const Program* program_ = nullptr
    );
    ~SceneViewMeshTexture() = default;

    void updateTextureRgba(int width, int height, const void* data);

protected:

    Texture texture_;

    void onAdjustModel_() override;

    void onAdjustCamera_() override;

    void onUseProgram_() override;

};


} // namespace s3r
} // namespace mixi
#pragma once

#include "pch.h"

#include "app/imgui_window.h"

#include "ui/scene_view_mesh_texture.h"
#include "util/ply.h"

namespace mixi
{
namespace s3r
{

class PlyViewerMeshTextureWindow : public app::ImguiWindow
{
public:

    PlyViewerMeshTextureWindow();
    ~PlyViewerMeshTextureWindow();

    void render() override;

protected:

    SceneViewMeshTexture sceneView_;

    std::unique_ptr<VertexArray> vertexArray_;

    std::unique_ptr<Shader> meshTextureFrag_;
    std::unique_ptr<Shader> meshTextureVert_;
    std::unique_ptr<Program> meshTextureProgram_;

    Texture texture;

private:

    void renderPlyDnd_();

    void loadPly_(const PlyFile& ply, const fs::path& dir);

};

} // namespace s3r
} // namespace mixi
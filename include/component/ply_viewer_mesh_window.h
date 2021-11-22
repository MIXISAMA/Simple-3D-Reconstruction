#pragma once

#include "pch.h"

#include "app/imgui_window.h"

#include "ui/scene_view_mesh.h"
#include "file/ply.h"

namespace mixi
{
namespace s3r
{

class PlyViewerMeshWindow : public app::ImguiWindow
{
public:

    PlyViewerMeshWindow();
    ~PlyViewerMeshWindow();

    void render() override;

protected:

    SceneViewMesh sceneView_;

    std::unique_ptr<VertexArray> vertexArray_;

    std::unique_ptr<Shader> meshFrag_;
    std::unique_ptr<Shader> meshVert_;
    std::unique_ptr<Program> meshProgram_;

private:

    void renderPlyDnd_();

    void loadPly_(const PlyFile& ply);

};

}
} // namespace mixi
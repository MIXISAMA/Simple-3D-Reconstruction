#pragma once

#include "pch.h"

#include "app/imgui_window.h"

#include "ui/scene_view_point.h"
#include "file/ply.h"

namespace mixi
{
namespace s3r
{

class PlyViewerPointWindow : public app::ImguiWindow
{
public:

    PlyViewerPointWindow();
    ~PlyViewerPointWindow();

    void render() override;

protected:

    SceneViewPoint sceneView_;

    std::unique_ptr<VertexArray> vertexArray_;

    std::unique_ptr<Shader> pointsFrag_;
    std::unique_ptr<Shader> pointsVert_;
    std::unique_ptr<Program> pointsProgram_;

    std::unique_ptr<Shader> pointsColorFrag_;
    std::unique_ptr<Shader> pointsColorVert_;
    std::unique_ptr<Program> pointsColorProgram_;

private:

    void renderPlyDnd_();

    void loadPly_(const PlyFile& ply);

};

}
} // namespace mixi
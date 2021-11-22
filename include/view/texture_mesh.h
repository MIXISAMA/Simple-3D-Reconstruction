#pragma once

#include "pch.h"
#include "app/imgui_window.h"
#include "component/output_file_window.h"

#include "component/ply_viewer_mesh_texture_window.h"
#include "component/texture_mesh_operate_window.h"

#include "process/mvs.h"

namespace mixi
{
namespace s3r
{


class TextureMeshView :
    public app::ImguiWindow,
    public OutputFileWindow::IContext
{
public:

    TextureMeshView(OutputFileWindow::Context* outputContext);
    ~TextureMeshView() = default;

    void render() override;

private:

    TextureMeshOperateWindow operateWindow_;
    PlyViewerMeshTextureWindow plyViewerWindow_;

    MvsTextureMesh mvs_;

    std::future<void> mvsFuture_;

    void onMvsCallback_();

};

} // namespace s3r
} // namespace mixi
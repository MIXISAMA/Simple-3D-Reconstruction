#pragma once

#include "pch.h"
#include "app/imgui_window.h"
#include "component/output_file_window.h"

#include "component/ply_viewer_mesh_window.h"
#include "component/rough_mesh_operate_window.h"

#include "process/mvs.h"

namespace mixi
{
namespace s3r
{

class RoughMeshView :
    public app::ImguiWindow,
    public OutputFileWindow::IContext
{
public:

    RoughMeshView(OutputFileWindow::Context* outputContext);
    ~RoughMeshView() = default;

    void render() override;

private:

    RoughMeshOperateWindow operateWindow_;
    PlyViewerMeshWindow plyViewerWindow_;

    MvsRoughMesh mvs_;

    std::future<void> mvsFuture_;

    void onMvsCallback_();

};

} // namespace s3r
} // namespace mixi
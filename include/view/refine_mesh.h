#pragma once

#include "pch.h"
#include "app/imgui_window.h"
#include "component/output_file_window.h"

#include "component/ply_viewer_mesh_window.h"
#include "component/refine_mesh_operate_window.h"

#include "process/mvs.h"

namespace mixi
{
namespace s3r
{

class RefineMeshView :
    public app::ImguiWindow,
    public OutputFileWindow::IContext
{
public:

    RefineMeshView(OutputFileWindow::Context* outputContext);
    ~RefineMeshView() = default;

    void render() override;

private:

    RefineMeshOperateWindow operateWindow_;
    PlyViewerMeshWindow plyViewerWindow_;

    MvsRefineMesh mvs_;

    std::future<void> mvsFuture_;

    void onMvsCallback_();

};

} // namespace s3r
} // namespace mixi
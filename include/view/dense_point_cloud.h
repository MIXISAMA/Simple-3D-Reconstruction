#pragma once

#include "pch.h"
#include "app/imgui_window.h"
#include "component/output_file_window.h"

#include "component/ply_viewer_point_window.h"
#include "component/dense_point_cloud_operate_window.h"

#include "process/mvs.h"

namespace mixi
{
namespace s3r
{

class DensePointCloudView :
    public app::ImguiWindow,
    public OutputFileWindow::IContext
{

public:

    DensePointCloudView(OutputFileWindow::Context* outputContext);
    ~DensePointCloudView() = default;

    void render() override;

private:

    DensePointCloudOperateWindow operateWindow_;
    PlyViewerPointWindow plyViewerWindow_;

    MvsDensePointCloud mvs_;

    std::future<void> mvsFuture_;

    void onMvsCallback_();

};

} // namespace s3r
} // namespace mixi
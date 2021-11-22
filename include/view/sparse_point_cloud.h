#pragma once

#include "pch.h"
#include "app/imgui_window.h"
#include "component/output_file_window.h"

#include "process/sfm.h"
#include "file/mvs.h"

#include "component/sparse_point_cloud_input_list_window.h"
#include "component/sparse_point_cloud_operate_window.h"
#include "component/ply_viewer_point_window.h"

namespace mixi
{
namespace s3r
{

class SparsePointCloudView :
    public app::ImguiWindow,
    public OutputFileWindow::IContext
{
public:

    SparsePointCloudView(OutputFileWindow::Context* outputContext);
    ~SparsePointCloudView();

    void render() override;

private:

    SparsePointCloudInputListWindow inputListWindow_;
    SparsePointCloudOperateWindow operateWindow_;
    PlyViewerPointWindow plyViewerWindow_;

    Sfm sfm_;
    std::future<MvsSparsePointFile::Ptr> sfmFuture_;

    void onSfmCallback_();

};

}
}
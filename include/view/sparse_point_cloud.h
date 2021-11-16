#pragma once

#include "pch.h"

#include "app/imgui_window.h"

#include "component/sparse_point_cloud_input_list_window.h"
#include "component/sparse_point_cloud_operate_window.h"
#include "component/ply_viewer_point_window.h"

#include "process/sfm.h"

namespace mixi
{
namespace s3r
{

class SparsePointCloudView : public app::ImguiWindow
{
public:

    SparsePointCloudView();
    ~SparsePointCloudView();

    void render() override;

private:

    SparsePointCloudInputListWindow sparsePointCloudInputListWindow_;
    SparsePointCloudOperateWindow sparsePointCloudOperateWindow_;
    PlyViewerPointWindow plyViewerWindow_;

    Sfm sfm_;

    void onSfmCallback_();

};

}
}
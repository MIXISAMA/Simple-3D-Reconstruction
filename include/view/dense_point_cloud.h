#pragma once

#include "pch.h"
#include "app/imgui_window.h"

#include "component/ply_viewer_point_window.h"

namespace mixi
{
namespace s3r
{

class DensePointCloudView : public app::ImguiWindow
{

public:

    DensePointCloudView();
    ~DensePointCloudView() = default;

    void render() override;

private:

    PlyViewerPointWindow plyViewerWindow_;

};

} // namespace s3r
} // namespace mixi
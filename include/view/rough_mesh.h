#pragma once

#include "pch.h"
#include "app/imgui_window.h"

#include "component/ply_viewer_mesh_window.h"

namespace mixi
{
namespace s3r
{

class RoughMeshView : public app::ImguiWindow
{
public:

    RoughMeshView();
    ~RoughMeshView() = default;

    void render() override;

private:

    PlyViewerMeshWindow plyViewerWindow_;

};

} // namespace s3r
} // namespace mixi
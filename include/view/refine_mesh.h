#pragma once

#include "pch.h"
#include "app/imgui_window.h"

#include "component/ply_viewer_mesh_window.h"

namespace mixi
{
namespace s3r
{

class RefineMeshView : public app::ImguiWindow
{
public:

    RefineMeshView();
    ~RefineMeshView() = default;

    void render() override;

private:

    PlyViewerMeshWindow plyViewerWindow_;

};

} // namespace s3r
} // namespace mixi
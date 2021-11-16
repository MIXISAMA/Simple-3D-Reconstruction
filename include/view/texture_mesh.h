#pragma once

#include "pch.h"
#include "app/imgui_window.h"

#include "component/ply_viewer_mesh_texture_window.h"

namespace mixi
{
namespace s3r
{

class TextureMeshView : public app::ImguiWindow
{
public:

    TextureMeshView();
    ~TextureMeshView() = default;

    void render() override;

private:

    PlyViewerMeshTextureWindow plyViewerWindow_;

};

} // namespace s3r
} // namespace mixi
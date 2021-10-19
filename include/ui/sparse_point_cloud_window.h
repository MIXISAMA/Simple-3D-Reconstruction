#pragma once

#include "pch.h"
#include "app/imgui_window.h"

#include "ui/scene_view.h"

namespace mixi
{
namespace s3r
{

class SparsePointCloudWindow : public app::ImguiWindow
{
public:

    SparsePointCloudWindow();

    void render() override;

protected:

    std::vector<Shader::Ptr> shaders_;
    VertexArray::Ptr vertexArray_;
    SceneView::Ptr sceneView_;

private:

};

}
}
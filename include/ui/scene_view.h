#pragma once

#include "pch.h"
#include "app/imgui_window.h"
#include "element/camera.h"
#include "render/frame_buffer.h"
#include "render/program.h"
#include "render/vertex_array.h"
#include "render/shader.h"

namespace mixi
{
namespace s3r
{


class SceneView : public app::ImguiWindow
{
public:

    using Ptr = std::shared_ptr<SceneView>;

    SceneView(
        int width, int height,
        VertexArray* vertexArray_,
        std::vector<Shader::Ptr>& shaders
    );
    ~SceneView();

    void render() override;

    void resize(int width, int height);

private:

    int width_;
    int height_;

    Camera camera_;
    FrameBuffer::Ptr frameBuffer_;
    Program program_;
    VertexArray* vertexArray_;

    void adjustCamera();

};


} // namespace s3r
} // namespace mixi


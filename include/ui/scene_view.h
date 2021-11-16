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
        const VertexArray* vertexArray_ = nullptr,
        const Program* program_ = nullptr
    );
    ~SceneView();

    void render() override;

    void setVertexArray(const VertexArray* vertexArray);
    void setInitTran(const glm::mat4& tran);
    void setProgram(const Program* program);

protected:

    int width_;
    int height_;

    Camera camera_;
    FrameBuffer::Ptr frameBuffer_;
    
    const VertexArray* vertexArray_;
    const Program* program_;

    glm::mat4 projection_;
    glm::mat4 view_;
    glm::mat4 model_;

    void resize_(int width, int height);

    void adjustCamera_(float left, float down, float forward);
    void adjustModel_(float rx, float ry);

    virtual void onUseProgram_();

    virtual void onAdjustModel_() = 0;

    virtual void onAdjustCamera_() = 0;

};


} // namespace s3r
} // namespace mixi


#include "ui/scene_view.h"

#include "file/ply.h"

namespace mixi
{

namespace s3r
{

SceneView::SceneView(
    const VertexArray* vertexArray_,
    const Program* program_
) :
    width_(400),
    height_(400),
    camera_(glm::vec3(0.0f, 0.0f, 100.0f)),
    frameBuffer_(new FrameBuffer(width_, height_)),
    vertexArray_(vertexArray_),
    program_(program_),
    projection_(
        glm::perspective(
            glm::radians(45.0f), (float)width_ / (float)height_, 1.0f, 10000.0f
        )
    ),
    view_(camera_.getViewMatrix()),
    model_(1.0f)
{

}

SceneView::~SceneView()
{

}

void SceneView::resize_(int width, int height)
{
    width_ = width;
    height_ = height;
    frameBuffer_ = FrameBuffer::Ptr(new FrameBuffer(width, height));
    projection_ = glm::perspective(
        glm::radians(45.0f), (float)width_ / (float)height_, 1.0f, 10000.0f
    );
}

void SceneView::render()
{
    ImVec2 contentRegionSize = ImGui::GetContentRegionAvail();
    if (contentRegionSize.x != width_ || contentRegionSize.y != height_) {
        resize_(contentRegionSize.x, contentRegionSize.y);
    }

    frameBuffer_->bind();
    
    glViewport(0, 0, width_, height_);
    glClearColor(0.1f, 0.0f, 0.2f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (program_ != nullptr) {
        program_->use();
        onUseProgram_();
        if (vertexArray_ != nullptr) {
            vertexArray_->draw();
        }
    }

    frameBuffer_->unbind();

    ImGui::Image(
        (void*)(intptr_t)frameBuffer_->texture()->id(),
        contentRegionSize
    );

    if (!ImGui::IsItemHovered()) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    if (
        ImGui::IsMouseDown(ImGuiMouseButton_Left) ||
        io.MouseWheel != 0.0f
    ) {
        adjustCamera_(
            io.MouseDelta.x * 0.3f,
            io.MouseDelta.y * 0.3f,
            io.MouseWheel
        );
        onAdjustCamera_();
    }
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        adjustModel_(
            io.MouseDelta.x * 0.01f,
            io.MouseDelta.y * 0.01f
        );
        onAdjustModel_();
    }
}

void SceneView::setVertexArray(const VertexArray* vertexArray)
{
    vertexArray_ = vertexArray;
}

void SceneView::setInitTran(const glm::mat4& tran)
{
    model_ = tran;
}

void SceneView::setProgram(const Program* program)
{
    program_ = program;
}

void SceneView::adjustCamera_(float left, float down, float forward)
{
    camera_.moveDown(down);
    camera_.moveLeft(left);
    camera_.moveForward(forward);
    view_ = camera_.getViewMatrix();
}

void SceneView::adjustModel_(float rx, float ry)
{
    glm::mat4 rotate(1.0f);
    rotate = glm::rotate(rotate, rx, glm::vec3( 0.0f, 1.0f, 0.0f));
    rotate = glm::rotate(rotate, ry, glm::vec3(-1.0f, 0.0f, 0.0f));
    model_ = rotate * model_;
}

void SceneView::onUseProgram_()
{
    program_->setMat4(glm::value_ptr(view_), "view");
    program_->setMat4(glm::value_ptr(model_), "model");
    program_->setMat4(glm::value_ptr(projection_), "projection");
}

} // namespace s3r

} // namespace mixi

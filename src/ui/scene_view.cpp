#include "ui/scene_view.h"

namespace mixi
{

namespace s3r
{

SceneView::SceneView(
    int width, int height,
    VertexArray* vertexArray_,
    std::vector<Shader::Ptr>& shaders
) :
    width_(width),
    height_(height),
    camera_(glm::vec3(0.0f, 0.0f, 100.0f)),
    frameBuffer_(new FrameBuffer(width, height)),
    program_(shaders),
    vertexArray_(vertexArray_)
{
	
}

SceneView::~SceneView()
{

}

void SceneView::resize(int width, int height)
{
    width_ = width;
    height_ = height;
    frameBuffer_ = FrameBuffer::Ptr(new FrameBuffer(width, height));
}

void SceneView::render()
{
    ImGui::Begin("Scene View");

    ImVec2 contentRegionSize = ImGui::GetContentRegionAvail();
    if (contentRegionSize.x != width_ || contentRegionSize.y != height_) {
        resize(contentRegionSize.x, contentRegionSize.y);
    }

    frameBuffer_->bind();
    
    glViewport(0, 0, width_, height_);
    glClearColor(0.1f, 0.0f, 0.2f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program_.use();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width_ / (float)height_, 1.0f, 10000.0f);
    glm::mat4 view = camera_.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    program_.setMat4(glm::value_ptr(projection), "projection");
    program_.setMat4(glm::value_ptr(view), "view");
    program_.setMat4(glm::value_ptr(model), "model");
    
    // frameBuffer_->texture()->bind();
    vertexArray_->draw(GL_POINTS);
    // frameBuffer_->texture()->unbind();

    frameBuffer_->unbind();

    ImGui::Image(
        (void*)(intptr_t)frameBuffer_->texture()->id(),
        // reinterpret_cast<void*>(frameBuffer_->texture()->id()),
        ImVec2{ (float)width_, (float)height_ }
    );

    if (ImGui::IsItemHovered()) {
        adjustCamera();
    }

    ImGui::End();
}

void SceneView::adjustCamera()
{
    ImGuiIO& io = ImGui::GetIO();

    if (ImGui::IsMouseDown(0)) { // Mouse left button down.
        camera_.moveUp(io.MouseDelta.y);
        camera_.moveRight(io.MouseDelta.x);
    }
    if (ImGui::IsMouseDown(1)) { // Mouse right button down.
        camera_.yawRight(io.MouseDelta.x);
        camera_.pitchUp(io.MouseDelta.y);
    }
    camera_.moveForward(io.MouseWheel);
}



} // namespace s3r

} // namespace mixi

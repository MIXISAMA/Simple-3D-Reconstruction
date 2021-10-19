#include "ui/sparse_point_cloud_window.h"

#include "imgui.h"


namespace mixi
{
namespace s3r
{

SparsePointCloudWindow::SparsePointCloudWindow() :
    sceneView_(nullptr),
    vertexArray_(nullptr),
    shaders_()
{

}

float points[12] = {
     5.0,  5.0, 0,
    -5.0, -5.0, 0,
     0,    5.0, 0,
     0, 0, 0,
};

void SparsePointCloudWindow::render()
{
    ImGui::Begin("Sparse Point Cloud Window");

    if (ImGui::Button("show scene view"))
    {
        std::vector<GLint> attributeNumbers = {3};
        vertexArray_ = VertexArray::Ptr(
            new VertexArray(points, sizeof(points), attributeNumbers)
        );
        fs::path vsPath("glsl/points.vert");
        fs::path fsPath("glsl/points.frag");
        Shader::Ptr vsShader(new Shader(vsPath, GL_VERTEX_SHADER));
        Shader::Ptr fsShader(new Shader(fsPath, GL_FRAGMENT_SHADER));
        shaders_ = {vsShader, fsShader};
        sceneView_ = SceneView::Ptr(
            new SceneView(400, 400, vertexArray_.get(), shaders_)
        );
    }

    if (sceneView_ != nullptr) {
        sceneView_->render();
    }

    ImGui::End();
}


} // namespace s3r
} // namespace mixi

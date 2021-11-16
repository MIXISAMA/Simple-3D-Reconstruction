#include "ui/scene_view_mesh_texture.h"

namespace mixi
{
namespace s3r
{

SceneViewMeshTexture::SceneViewMeshTexture(
    const VertexArray* vertexArray_,
    const Program* program_
) :
    SceneView(),
    texture_()
{

}

void SceneViewMeshTexture::updateTextureRgba(int width, int height, const void* data)
{
    texture_.updateRgbaImage2D(width, height, data);
}

void SceneViewMeshTexture::onAdjustModel_()
{

}

void SceneViewMeshTexture::onAdjustCamera_()
{

}

void SceneViewMeshTexture::onUseProgram_()
{
    SceneView::onUseProgram_();
    program_->setInt(0, "texture0");
    glActiveTexture(GL_TEXTURE0);
    texture_.bind();
}

} // namespace s3r
} // namespace mixi
#include "ui/scene_view_mesh.h"

namespace mixi
{
namespace s3r
{

SceneViewMesh::SceneViewMesh(
    const VertexArray* vertexArray_,
    const Program* program_
) :
    SceneView(),
    lightPos{-15.0f, -50.0f, 100.0f},
    lightColor{1.0f, 1.0f, 1.0f},
    objectColor{0.6f, 0.6f, 0.6f},
    viewPos_(0.0f, 0.0f, 100.0f)
{

}

void SceneViewMesh::onAdjustModel_()
{
    
}

void SceneViewMesh::onAdjustCamera_()
{
    viewPos_ = camera_.getViewPoint();
}

void SceneViewMesh::onUseProgram_()
{
    SceneView::onUseProgram_();
    program_->setVec3(lightPos, "lightPos");
    program_->setVec3(lightColor, "lightColor");
    program_->setVec3(objectColor, "objectColor");
}

}
}
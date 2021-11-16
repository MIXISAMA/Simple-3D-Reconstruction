#include "ui/scene_view_point.h"

namespace mixi
{
namespace s3r
{

SceneViewPoint::SceneViewPoint(
    const VertexArray* vertexArray_,
    const Program* program_
) :
    SceneView(vertexArray_, program_),
    pointSize(100.0f)
{

}

SceneViewPoint::~SceneViewPoint()
{

}

void SceneViewPoint::onAdjustModel_()
{

}

void SceneViewPoint::onAdjustCamera_()
{

}

void SceneViewPoint::onUseProgram_()
{
    SceneView::onUseProgram_();
    program_->setFloat(pointSize, "pointSize");
}

}
}
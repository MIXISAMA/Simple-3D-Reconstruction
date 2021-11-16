#include "view/dense_point_cloud.h"

namespace mixi
{
namespace s3r
{

DensePointCloudView::DensePointCloudView() :
    plyViewerWindow_()
{

}

void DensePointCloudView::render()
{
    ImGui::Begin("Dense Point Cloud Window");

    plyViewerWindow_.render();

    ImGui::End();
}

} // namespace s3r
} // namespace mixi

#include "view/rough_mesh.h"

namespace mixi
{
namespace s3r
{

RoughMeshView::RoughMeshView() :
    plyViewerWindow_()
{

}

void RoughMeshView::render()
{
    ImGui::Begin("Rough Mesh Window");

    plyViewerWindow_.render();

    ImGui::End();
}

} // namespace s3r
} // namespace mixi

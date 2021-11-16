#include "view/refine_mesh.h"

namespace mixi
{
namespace s3r
{

RefineMeshView::RefineMeshView() :
    plyViewerWindow_()
{

}

void RefineMeshView::render()
{
    ImGui::Begin("Refine Mesh Window");

    plyViewerWindow_.render();

    ImGui::End();
}

} // namespace s3r
} // namespace mixi

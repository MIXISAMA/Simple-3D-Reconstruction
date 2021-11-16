#include "view/texture_mesh.h"

namespace mixi
{
namespace s3r
{

TextureMeshView::TextureMeshView() :
    plyViewerWindow_()
{

}

void TextureMeshView::render()
{
    ImGui::Begin("Texture Mesh View");

    plyViewerWindow_.render();

    ImGui::End();
}

} // namespace s3r
} // namespace mixi
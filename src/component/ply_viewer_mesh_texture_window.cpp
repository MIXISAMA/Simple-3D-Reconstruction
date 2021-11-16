#include "component/ply_viewer_mesh_texture_window.h"

#include "util/utils.h"
#include "util/image.h"

namespace mixi
{
namespace s3r
{

PlyViewerMeshTextureWindow::PlyViewerMeshTextureWindow() :
    sceneView_(),
    vertexArray_(nullptr),
    meshTextureFrag_(nullptr),
    meshTextureVert_(nullptr),
    meshTextureProgram_(nullptr)
{

}

PlyViewerMeshTextureWindow::~PlyViewerMeshTextureWindow()
{

}

void PlyViewerMeshTextureWindow::render()
{
    ImGui::Begin("Scene View");
    sceneView_.render();
    renderPlyDnd_();
    ImGui::End();
}

void PlyViewerMeshTextureWindow::renderPlyDnd_()
{
    if (!ImGui::BeginDragDropTarget()) {
        return;
    }

    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PATH")) {
        IM_ASSERT(payload->DataSize == sizeof(fs::path));
        const fs::path& path = *(fs::path*)payload->Data;
        try {
            loadPly_(PlyFile(path), path.parent_path());
        }
        catch (std::runtime_error e) {
            // Todo: error tip
        }
    }

    ImGui::EndDragDropTarget();
}


void PlyViewerMeshTextureWindow::loadPly_(const PlyFile& ply, const fs::path& dir)
{
    if (!ply.hasTexcoord()) {
        return;
    }

    VertexArray* vertexArray = nullptr;
    ply.parseVertexArray(&vertexArray);
    vertexArray_.reset(vertexArray);
    sceneView_.setVertexArray(vertexArray_.get());

    auto [minX, maxX] = ply.minmaxX();
    auto [minY, maxY] = ply.minmaxY();
    auto [minZ, maxZ] = ply.minmaxZ();

    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;
    float centerZ = (minZ + maxZ) / 2.0f;

    float min = std::min({minX, minY, minZ});
    float max = std::max({maxX, maxY, maxZ});
    float scale = 80.0f / max - min;

    glm::mat4 trans(1.0f);
    trans = glm::scale(trans, {scale, scale, scale});
    trans = glm::translate(trans, {-centerX, -centerY, -centerZ});
    sceneView_.setInitTran(trans);

    ResetIfNullptr(meshTextureVert_, "glsl/mesh_texture.vert", GL_VERTEX_SHADER);
    ResetIfNullptr(meshTextureFrag_, "glsl/mesh_texture.frag", GL_FRAGMENT_SHADER);
    ResetIfNullptr(
        meshTextureProgram_,
        std::vector<Shader*>{meshTextureVert_.get(), meshTextureFrag_.get()}
    );
    sceneView_.setProgram(meshTextureProgram_.get());

    aiString textureFilename;
    ply.getTextureName(&textureFilename);
    Image image(dir / textureFilename.C_Str());
    sceneView_.updateTextureRgba(image.width(), image.height(), image.data());
    glGenerateMipmap(GL_TEXTURE_2D);

}

}
} // namespace mixi
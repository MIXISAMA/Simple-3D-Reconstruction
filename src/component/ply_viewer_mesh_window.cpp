#include "component/ply_viewer_mesh_window.h"

#include "util/utils.h"

namespace mixi
{
namespace s3r
{

PlyViewerMeshWindow::PlyViewerMeshWindow() :
    sceneView_(),
    vertexArray_(nullptr),
    meshFrag_(nullptr),
    meshVert_(nullptr),
    meshProgram_(nullptr)
{

}

PlyViewerMeshWindow::~PlyViewerMeshWindow()
{

}

void PlyViewerMeshWindow::render()
{
    ImGui::Begin("Scene View");

    ImGui::ColorEdit3("Light Color", sceneView_.lightColor);
    ImGui::ColorEdit3("Object Color", sceneView_.objectColor);
    ImGui::SliderFloat3("Light Pos", sceneView_.lightPos, -150.0f, 150.0f);

    sceneView_.render();
    renderPlyDnd_();

    ImGui::End();
}

void PlyViewerMeshWindow::renderPlyDnd_()
{
    if (!ImGui::BeginDragDropTarget()) {
        return;
    }

    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PATH")) {
        IM_ASSERT(payload->DataSize == sizeof(fs::path));
        const fs::path& path = *(fs::path*)payload->Data;
        try {
            loadPly_(PlyFile(path));
        }
        catch (std::runtime_error e) {
            // Todo: error tip
        }
    }

    ImGui::EndDragDropTarget();
}

void PlyViewerMeshWindow::loadPly_(const PlyFile& ply)
{
    if (!ply.hasIndices()) {
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

    ResetIfNullptr(meshVert_, "glsl/mesh.vert", GL_VERTEX_SHADER);
    ResetIfNullptr(meshFrag_, "glsl/mesh.frag", GL_FRAGMENT_SHADER);
    ResetIfNullptr(meshProgram_, std::vector<Shader*>{meshVert_.get(), meshFrag_.get()});
    sceneView_.setProgram(meshProgram_.get());

}

}
}
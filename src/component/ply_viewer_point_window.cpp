#include "component/ply_viewer_point_window.h"

#include "util/utils.h"

namespace mixi
{
namespace s3r
{

PlyViewerPointWindow::PlyViewerPointWindow() :
    sceneView_(),
    vertexArray_(nullptr),
    pointsFrag_(nullptr),
    pointsVert_(nullptr),
    pointsProgram_(nullptr),
    pointsColorFrag_(nullptr),
    pointsColorVert_(nullptr),
    pointsColorProgram_(nullptr)
{

}

PlyViewerPointWindow::~PlyViewerPointWindow()
{

}

void PlyViewerPointWindow::render()
{
    ImGui::Begin("Scene View");

    ImGui::SliderFloat("Point Size", &sceneView_.pointSize, 1.0f, 1000.0f, "%.1f", ImGuiSliderFlags_Logarithmic);

    sceneView_.render();
    renderPlyDnd_();

    ImGui::End();
}

void PlyViewerPointWindow::renderPlyDnd_()
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

void PlyViewerPointWindow::loadPly_(const PlyFile& ply)
{
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

    if (ply.hasColors()) {
        ResetIfNullptr(pointsColorVert_, "glsl/points_color.vert", GL_VERTEX_SHADER);
        ResetIfNullptr(pointsColorFrag_, "glsl/points_color.frag", GL_FRAGMENT_SHADER);
        ResetIfNullptr(pointsColorProgram_, std::vector<Shader*>{pointsColorVert_.get(), pointsColorFrag_.get()});
        sceneView_.setProgram(pointsColorProgram_.get());
    }
    else {
        ResetIfNullptr(pointsVert_, "glsl/points.vert", GL_VERTEX_SHADER);
        ResetIfNullptr(pointsFrag_, "glsl/points.frag", GL_FRAGMENT_SHADER);
        ResetIfNullptr(pointsProgram_, std::vector<Shader*>{pointsVert_.get(), pointsFrag_.get()});
        sceneView_.setProgram(pointsProgram_.get());
    }
}

}
}
#include "component/texture_mesh_operate_window.h"

namespace mixi
{
namespace s3r
{

TextureMeshOperateWindow::TextureMeshOperateWindow(
    std::function<void(void)> onRunCallback
) :
    onRunCallback_(onRunCallback),
    progress_(0.0f),
    runnable_(false),
    mvsInputFilepath_(),
    pkgName_("model")
{

}


void TextureMeshOperateWindow::render()
{
    ImGui::Begin("Dense Point Cloud Operate");

    ImGui::InputText("Model Name", pkgName_, IM_ARRAYSIZE(pkgName_));

    ImGui::Text("Sparse Point Cloud Mvs File: ");

    bool empty = mvsInputFilepath_.empty();

    ImGui::SameLine();
    std::string selectableText = empty ? "no file" : mvsInputFilepath_.filename();
    ImGui::Selectable(selectableText.c_str());
    renderPathSelectableDnd_();

    ImGui::BeginDisabled(empty);
    if (ImGui::Button("generate dense point cloud")) {
        onRunCallback_();
    }
    ImGui::EndDisabled();

    ImGui::SameLine();
    ImGui::ProgressBar(progress_);

    ImGui::End();
}

void TextureMeshOperateWindow::renderPathSelectableDnd_()
{
    if (!ImGui::BeginDragDropTarget()) {
        return;
    }
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PATH")) {
        IM_ASSERT(payload->DataSize == sizeof(fs::path));
        fs::path* path = (fs::path*)payload->Data;
        if (path->extension().string() == ".mvs") {
            mvsInputFilepath_ = *path;
        }
    }
    ImGui::EndDragDropTarget();
}

const fs::path& TextureMeshOperateWindow::mvsInputFilepath() const
{
    return mvsInputFilepath_;
}

void TextureMeshOperateWindow::setProgress(float progress)
{
    progress_ = progress;
}

const char* TextureMeshOperateWindow::pkgName()
{
    return pkgName_;
}

} // namespace s3r
} // namespace mixi
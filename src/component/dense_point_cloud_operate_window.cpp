#include "component/dense_point_cloud_operate_window.h"

namespace mixi
{
namespace s3r
{

DensePointCloudOperateWindow::DensePointCloudOperateWindow(
    std::function<void(void)> onRunCallback
) :
    onRunCallback_(onRunCallback),
    progress_(0.0f),
    runnable_(false),
    mvsInputFilepath_(),
    pkgName_("model")
{

}


void DensePointCloudOperateWindow::render()
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

void DensePointCloudOperateWindow::renderPathSelectableDnd_()
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

const fs::path& DensePointCloudOperateWindow::mvsInputFilepath() const
{
    return mvsInputFilepath_;
}

void DensePointCloudOperateWindow::setProgress(float progress)
{
    progress_ = progress;
}

const char* DensePointCloudOperateWindow::pkgName()
{
    return pkgName_;
}

} // namespace s3r
} // namespace mixi
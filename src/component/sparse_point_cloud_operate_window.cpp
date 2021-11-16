#include "component/sparse_point_cloud_operate_window.h"

namespace mixi
{
namespace s3r
{

SparsePointCloudOperateWindow::SparsePointCloudOperateWindow(std::function<void(void)> onSfmCallback) :
    onSfmCallback_(onSfmCallback),
    progress_(0.0f),
    runnable_(false)
{

}

void SparsePointCloudOperateWindow::render()
{
    ImGui::Begin("Sparse Point Cloud Operate");

    if (!runnable_) {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button("SfM")) {
        onSfmCallback_();
    }
    if (!runnable_) {
        ImGui::EndDisabled();
    }

    ImGui::SameLine();

    ImGui::ProgressBar(progress_, ImVec2(0.0f, 0.0f));

    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
    
    ImGui::Text("Progress");

    ImGui::End();
}

void SparsePointCloudOperateWindow::setProgress(float progress)
{
    progress_ = progress;
}

void SparsePointCloudOperateWindow::setRunnable(bool runnable)
{
    runnable_ = runnable;
}

} // namespace s3r
} // namespace mixi
#include "ui/main_window.h"

#include "ui/camera_window.h"
#include "ui/sparse_point_cloud_window.h"
#include "ui/dense_point_cloud_window.h"

namespace mixi
{
namespace s3r
{

MainWindow::MainWindow() :
    usageSelectorWindow_(),
    workspaceWindow_(),
    usageWindow_(nullptr)
{

}

void MainWindow::render()
{
    usageSelectorWindow_.render();

    workspaceWindow_.render();

    renderUsageWindow_();
}

void MainWindow::renderUsageWindow_()
{
    switch (usageSelectorWindow_.selected())
    {
    case UsageSelectorWindow::Usage::CAMERA:
        if(dynamic_cast<CameraWindow*>(usageWindow_.get()) == nullptr) {
            usageWindow_ = ImguiWindow::Ptr(new CameraWindow());
        }
        break;
    case UsageSelectorWindow::Usage::SPARSE_POINT_CLOUD:
        if(dynamic_cast<SparsePointCloudWindow*>(usageWindow_.get()) == nullptr) {
            usageWindow_ = ImguiWindow::Ptr(new SparsePointCloudWindow());
        }
        break;
    case UsageSelectorWindow::Usage::DENSE_POINT_CLOUD:
        if(dynamic_cast<DensePointCloudWindow*>(usageWindow_.get()) == nullptr) {
            usageWindow_ = ImguiWindow::Ptr(new DensePointCloudWindow());
        }
        break;
    };
    if (usageWindow_ != nullptr) {
        usageWindow_->render();
    }
}

} // namespace s3r
} // namespace mixi

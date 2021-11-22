#include "ui/main_window.h"

#include "ui/camera_window.h"
#include "view/sparse_point_cloud.h"
#include "view/dense_point_cloud.h"
#include "view/rough_mesh.h"
#include "view/refine_mesh.h"
#include "view/texture_mesh.h"

#include "util/utils.h"

namespace mixi
{
namespace s3r
{

MainWindow::MainWindow() :
    outputFileWindow_(),
    usageSelectorWindow_(),
    workspaceWindow_(),
    usageWindow_(nullptr)
{

}

void MainWindow::render()
{
    workspaceWindow_.render();
    outputFileWindow_.render();
    usageSelectorWindow_.render();
    renderUsageWindow_();
}

void MainWindow::renderUsageWindow_()
{
    switch (usageSelectorWindow_.selected())
    {
    case UsageSelectorWindow::Usage::CAMERA:
        ResetIfCanNotCast<CameraWindow          >(usageWindow_, outputFileWindow_.context());
        break;
    case UsageSelectorWindow::Usage::SPARSE_POINT_CLOUD:
        ResetIfCanNotCast<SparsePointCloudView  >(usageWindow_, outputFileWindow_.context());
        break;
    case UsageSelectorWindow::Usage::DENSE_POINT_CLOUD:
        ResetIfCanNotCast<DensePointCloudView   >(usageWindow_, outputFileWindow_.context());
        break;
    case UsageSelectorWindow::Usage::ROUGH_MESH:
        ResetIfCanNotCast<RoughMeshView         >(usageWindow_, outputFileWindow_.context());
        break;
    case UsageSelectorWindow::Usage::REFINE_MESH:
        ResetIfCanNotCast<RefineMeshView        >(usageWindow_, outputFileWindow_.context());
        break;
    case UsageSelectorWindow::Usage::TEXTURE_MESH:
        ResetIfCanNotCast<TextureMeshView       >(usageWindow_, outputFileWindow_.context());
        break;
    };
    if (usageWindow_ != nullptr) {
        usageWindow_->render();
    }
}

} // namespace s3r
} // namespace mixi

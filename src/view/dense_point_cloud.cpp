#include "view/dense_point_cloud.h"

namespace mixi
{
namespace s3r
{

DensePointCloudView::DensePointCloudView(OutputFileWindow::Context* outputContext) :
    IContext(outputContext),
    plyViewerWindow_(),
    operateWindow_(std::bind(&DensePointCloudView::onMvsCallback_, this)),
    mvs_(),
    mvsFuture_()
{

}

void DensePointCloudView::render()
{
    operateWindow_.setProgress(mvs_.progress());
    operateWindow_.render();

    plyViewerWindow_.render();
}


void DensePointCloudView::onMvsCallback_()
{
    const fs::path& path = operateWindow_.mvsInputFilepath();
    std::string pkgName = operateWindow_.pkgName();
    if (pkgName.empty()) {
        pkgName = "model";
    }
    
    mvsFuture_ = std::async(std::launch::async, [this, path, pkgName] {
        mvs_.run(pkgName, path);
    });
}


} // namespace s3r
} // namespace mixi

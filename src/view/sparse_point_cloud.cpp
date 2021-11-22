#include "view/sparse_point_cloud.h"

namespace mixi
{
namespace s3r
{

SparsePointCloudView::SparsePointCloudView(OutputFileWindow::Context* outputContext) :
    IContext(outputContext),
    inputListWindow_(),
    operateWindow_(std::bind(&SparsePointCloudView::onSfmCallback_, this)),
    plyViewerWindow_(),
    sfm_(),
    sfmFuture_()
{
    // sfm_.setTmpDirInfix(PROJECT_NAME);
}

SparsePointCloudView::~SparsePointCloudView()
{

}

void SparsePointCloudView::render()
{
    inputListWindow_.render();

    operateWindow_.setProgress(sfm_.progress());
    operateWindow_.setRunnable(inputListWindow_.checkRunnable());
    operateWindow_.render();

    plyViewerWindow_.render();

    if (sfmFuture_.valid() &&
        sfmFuture_.wait_for(std::chrono::seconds(0)) == std::future_status::ready
    ) {
        MvsSparsePointFile::Ptr file = sfmFuture_.get();
        MemoryDirectory::Ptr dir(nullptr);
        outputContext_->add(dir, "mvs");
        dir->files.push_back(file);
    }
}

void SparsePointCloudView::onSfmCallback_()
{
    std::string pkgName = operateWindow_.pkgName();
    if (pkgName.empty()) {
        pkgName = "model";
    }

    std::vector<fs::path> imagePathes = inputListWindow_.imagePathes();
    std::string param = inputListWindow_.paramFile()->formatIntrinsics();

    sfmFuture_ = std::async(std::launch::async, [this, pkgName, imagePathes, param]{
        MvsSparsePointFile::Ptr file(new MvsSparsePointFile(pkgName));
        sfm_.run(file->tmpDir(), imagePathes, param);
        return file;
    });
}

}
}
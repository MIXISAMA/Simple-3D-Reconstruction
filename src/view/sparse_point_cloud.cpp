#include "view/sparse_point_cloud.h"

namespace mixi
{
namespace s3r
{

SparsePointCloudView::SparsePointCloudView() :
    sparsePointCloudInputListWindow_(),
    sparsePointCloudOperateWindow_(
        std::bind(&SparsePointCloudView::onSfmCallback_, this)
    ),
    plyViewerWindow_(),
    sfm_()
{
    sfm_.setTmpDirInfix(PROJECT_NAME);
}

SparsePointCloudView::~SparsePointCloudView()
{

}

void SparsePointCloudView::render()
{
    sparsePointCloudInputListWindow_.render();

    sparsePointCloudOperateWindow_.setProgress(
        sfm_.progress() / (float)sfm_.MaxProgress
    );
    sparsePointCloudOperateWindow_.setRunnable(
        sparsePointCloudInputListWindow_.checkRunnable()
    );
    sparsePointCloudOperateWindow_.render();

    plyViewerWindow_.render();
}

void SparsePointCloudView::onSfmCallback_()
{
    sfm_.run(
        sparsePointCloudInputListWindow_.imagePathes(),
        sparsePointCloudInputListWindow_.paramFile()
    );
}

}
}
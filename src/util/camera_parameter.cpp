#include "util/camera_parameter.h"

namespace mixi
{

CameraParameterFile::CameraParameterFile(fs::path& filepath) :
    MemoryFile(filepath.filename()),
    intrinsic_(),
    distCoeffs_()
{
    cv::FileStorage xml(filepath, cv::FileStorage::READ);
    if (!xml.isOpened()) {
        throw std::runtime_error("Camera parameter file open failed.");
    }
    xml["Intrinsic-Matrix"] >> intrinsic_;
    xml["Distortion-Coefficients"] >> distCoeffs_;
}

CameraParameterFile::CameraParameterFile(fs::path& filename, cv::Mat& intrinsic, cv::Mat& distCoeffs) :
    MemoryFile(filename),
    intrinsic_(intrinsic),
    distCoeffs_(distCoeffs)
{

}

void CameraParameterFile::save(const fs::path& parentPath) const
{
    cv::FileStorage xml(parentPath / filename_, cv::FileStorage::WRITE);
    xml << "Intrinsic-Matrix" << intrinsic_;
    xml << "Distortion-Coefficients" << distCoeffs_;
}

cv::Mat CameraParameterFile::intrinsic() const
{
    return intrinsic_;
}

cv::Mat CameraParameterFile::distCoeffs() const
{
    return distCoeffs_;
}

} // namespace mixi

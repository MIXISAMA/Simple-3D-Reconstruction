#include "file/camera_parameter.h"

namespace mixi
{

CameraParameterFile::CameraParameterFile(fs::path& filepath) :
    MemoryFile(filepath.filename()),
    intrinsic_(cv::Mat_<double>::zeros(3, 3)),
    distCoeffs_(cv::Mat_<double>::zeros(1, 5))
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

void CameraParameterFile::save_(const fs::path& parentPath)
{
    cv::FileStorage xml(parentPath / filename_, cv::FileStorage::WRITE);
    xml << "Intrinsic-Matrix" << intrinsic_;
    xml << "Distortion-Coefficients" << distCoeffs_;
}

std::string CameraParameterFile::formatIntrinsics() const
{
    std::ostringstream oss;
    double* intrinsicData = (double*)intrinsic_.data;
    for(int i = 0; i < intrinsic_.rows; i++){
        for(int j = 0; j < intrinsic_.cols; j++){
            if ( i != 0 || j != 0 ) {
                oss << ";";
            }
            oss << intrinsicData[intrinsic_.rows * i + j];
        }
    }
    return oss.str();
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

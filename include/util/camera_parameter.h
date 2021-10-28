#pragma once

#include "pch.h"
#include "file_util.h"

namespace mixi
{

class CameraParameterFile : public MemoryFile
{
public:

    using Ptr = std::shared_ptr<CameraParameterFile>;

    CameraParameterFile(fs::path& filepath);
    CameraParameterFile(fs::path& filename, cv::Mat& intrinsic, cv::Mat& distCoeffs);

    ~CameraParameterFile() = default;

    void save(const fs::path& parentPath) const override;

    cv::Mat intrinsic() const;
    cv::Mat distCoeffs() const;

protected:

    cv::Mat intrinsic_;
    cv::Mat distCoeffs_;

};

} // namespace mixi

#pragma once

#include "pch.h"
#include "base.h"

namespace mixi
{

class CameraParameterFile : public MemoryFile
{
public:

    using Ptr = std::shared_ptr<CameraParameterFile>;

    CameraParameterFile(fs::path& filepath);
    CameraParameterFile(fs::path& filename, cv::Mat& intrinsic, cv::Mat& distCoeffs);

    ~CameraParameterFile() = default;

    std::string formatIntrinsics() const;

    cv::Mat intrinsic() const;
    cv::Mat distCoeffs() const;

protected:

    cv::Mat intrinsic_;
    cv::Mat distCoeffs_;

    virtual void save_(const fs::path& parentPath) override;

};

} // namespace mixi

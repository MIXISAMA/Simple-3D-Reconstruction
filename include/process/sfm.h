#pragma once

#include "pch.h"

#include "util/image.h"
#include "util/camera_parameter.h"


namespace mixi
{

class Sfm
{
public:

    Sfm();
    ~Sfm();

    void setTmpDirInfix(const fs::path& infix);

    void run(
        const std::vector<fs::path>& imageFiles,
        const CameraParameterFile* param
    );

    void terminate();

    int progress();

    std::future_status state();

    static const int MaxProgress;

private:

    fs::path tmpDir_;

    std::atomic<bool> toTerminate_;
    std::future<void> future_;

    std::atomic<int> progress_;

    void initTmpDir_();
    void removeTmpDir_();

    void sequentialPipeline_(const CameraParameterFile* param);

};


} // namespace mixi
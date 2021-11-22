#pragma once

#include "pch.h"

#include "file/image.h"
#include "file/camera_parameter.h"


namespace mixi
{

class Sfm
{
public:

    Sfm();
    ~Sfm();

    void run(
        const fs::path& sfmDir, // sfmDir must be empty and exists.
        const std::vector<fs::path>& imageFiles,
        const std::string& param
    );

    // void terminate();

    float progress();

    static float ProgressParser(
        std::vector<std::string> keywords,
        std::string line,
        float defaultProgress
    );

private:

    std::atomic<float> progress_;

    void initSfmDir_(
        const fs::path& sfmDir,
        const std::vector<fs::path>& imageFiles
    );

};


} // namespace mixi
#include "file/mvs.h"

#include "util/child_process.h"
#include "process/sfm.h"

namespace mixi
{

MvsSparsePointFile::MvsSparsePointFile(const std::string& pkgName) :
    FakeMemoryFile(pkgName),
    saveFuture_(),
    pkgName_(pkgName)
{

}

void MvsSparsePointFile::save_(const fs::path& parentPath)
{
    const boost::filesystem::path inputDir(tmpDir_ / "reconstruction");
    const boost::filesystem::path outputDir(parentPath);
    const boost::filesystem::path undistortedDir(outputDir / "distorted_images");
    std::vector<std::string> keywords{"PROCESS VIEWS", "UNDISTORT IMAGES"};

    fs::create_directory(undistortedDir.string());

    ChildProcess child(
        bp::search_path("openMVG_main_openMVG2openMVS"),
        "-i", inputDir / "sfm_data.bin",
        "-d", undistortedDir,
        "-o", outputDir / (pkgName_ + ".mvs")
    );

    fs::copy_file(
        inputDir.string() + "/colorized.ply",
        outputDir.string() + "/" + pkgName_ + "_colorized.ply"
    );

    progress_ = 0.09f;

    bp::ipstream& ips = child.eips();
    std::string line;
    float localProgress = 0.0f;

    while (child.running() && std::getline(ips, line)) {
        localProgress = Sfm::ProgressParser(keywords, line, localProgress);
        progress_ = 0.09f + localProgress * 0.9f;
    }

    child.wait();
}

/* log sample:
INFO: [main_openMVG2openMVS.cpp:122] Cannot read the corresponding pose or intrinsic of view 0
INFO: [main_openMVG2openMVS.cpp:122] Cannot read the corresponding pose or intrinsic of view 1
INFO: [main_openMVG2openMVS.cpp:122] Cannot read the corresponding pose or intrinsic of view 2
INFO: [loggerprogress.hpp:79] [- PROCESS VIEWS -] 50%
INFO: [loggerprogress.hpp:79] [- PROCESS VIEWS -] 100%
INFO: [loggerprogress.hpp:79] [- UNDISTORT IMAGES ] 50%
INFO: [loggerprogress.hpp:79] [- UNDISTORT IMAGES ] 100%
INFO: [main_openMVG2openMVS.cpp:236] Scene saved to OpenMVS interface format:
 #platforms: 1
INFO: [main_openMVG2openMVS.cpp:241]   platform ( 0 ) #cameras: 1
INFO: [main_openMVG2openMVS.cpp:243]   5 images (5 calibrated)
  132 Landmarks
*/

// 0% - 100%
float MvsSparsePointFile::ParseProgress(const std::string& line, float defaultProgress)
{
    // PROCESS VIEWS 0% - 50%
    {
        std::smatch result;
        std::regex pattern(R"(\[- PROCESS VIEWS -\] ([0-9]+)%)");

        if (std::regex_search(line, result, pattern) && result.size() == 2) {
            std::istringstream iss(result[1].str());
            int percent;
            iss >> percent;
            return percent / 100.0f * 0.45f;
        }
    }
    
    // UNDISTORT IMAGES 50% - 100%
    {
        std::smatch result;
        std::regex pattern(R"(\[- UNDISTORT IMAGES -\] ([0-9]+)%)");

        if (std::regex_search(line, result, pattern) && result.size() == 2) {
            std::istringstream iss(result[1].str());
            int percent;
            iss >> percent;
            return percent / 100.0f * 0.45f + 0.45f;
        }
    }

    return defaultProgress;

}

}  // namespace mixi
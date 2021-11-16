#include "process/sfm.h"

#include "util/child_process.h"
#include "util/file_util.h"


namespace mixi
{

const int Sfm::MaxProgress = 7;

Sfm::Sfm() :
    tmpDir_(fs::temp_directory_path() / "sfm"),
    toTerminate_(false),
    future_(),
    progress_(0)
{
    
}

Sfm::~Sfm()
{
    removeTmpDir_();
}

void Sfm::setTmpDirInfix(const fs::path& infix)
{
    removeTmpDir_();
    tmpDir_ = fs::temp_directory_path() / infix / "sfm";
}

void Sfm::run(
    const std::vector<fs::path>& imageFiles,
    const CameraParameterFile* param
)
{
    initTmpDir_();
    fs::path inputDir(tmpDir_ / "input");
    for (const fs::path& imageFile : imageFiles) {
        fs::copy_file(imageFile, inputDir / imageFile.filename());
    }
    future_ = std::async(std::launch::async, [this, param] {
        sequentialPipeline_(param);
    });
}

std::future_status Sfm::state()
{
    return future_.wait_for(std::chrono::seconds(0));
}

int Sfm::progress()
{
    return progress_;
}

void Sfm::terminate()
{
    toTerminate_ = true;
    future_.wait();
    toTerminate_ = false;
}

void Sfm::initTmpDir_()
{
    removeTmpDir_();
    fs::create_directories(tmpDir_);
    fs::create_directory(tmpDir_ / "input");
    fs::create_directory(tmpDir_ / "output");
    fs::create_directory(tmpDir_ / "matches");
    fs::create_directory(tmpDir_ / "reconstruction");
}

void Sfm::removeTmpDir_()
{
    if (fs::exists(tmpDir_)) {
        fs::remove_all(tmpDir_);
    }
}

void Sfm::sequentialPipeline_(const CameraParameterFile* param)
{
    progress_ = 0;
    const boost::filesystem::path inputDir(tmpDir_ / "input");
    const boost::filesystem::path outputDir(tmpDir_ / "output");
    const boost::filesystem::path matchesDir(tmpDir_ / "matches");
    const boost::filesystem::path reconstructionDir(tmpDir_ / "reconstruction");
    ChildProcess(
        toTerminate_,
        bp::search_path("openMVG_main_SfMInit_ImageListing"),
        "-i", inputDir,
        "-o", matchesDir,
        "-k", param->formatIntrinsics()
    );
    progress_++;
    ChildProcess(
        toTerminate_,
        bp::search_path("openMVG_main_ComputeFeatures"),
        "-i", matchesDir / "sfm_data.json",
        "-o", matchesDir,
        "-m", "SIFT"
    );
    progress_++;
    ChildProcess(
        toTerminate_,
        bp::search_path("openMVG_main_PairGenerator"),
        "-i", matchesDir / "sfm_data.json",
        "-o", matchesDir / "pairs.bin"
    );
    progress_++;
    ChildProcess(
        toTerminate_,
        bp::search_path("openMVG_main_ComputeMatches"),
        "-i", matchesDir / "sfm_data.json",
        "-p", matchesDir / "pairs.bin",
        "-o", matchesDir / "matches.putative.bin"
    );
    progress_++;
    ChildProcess(
        toTerminate_,
        bp::search_path("openMVG_main_GeometricFilter"),
        "-i", matchesDir / "sfm_data.json",
        "-m", matchesDir / "matches.putative.bin" ,
        "-g", "f",
        "-o", matchesDir / "matches.f.bin"
    );
    progress_++;
    ChildProcess(
        toTerminate_,
        bp::search_path("openMVG_main_SfM"),
        "--sfm_engine", "INCREMENTAL",
        "--input_file", matchesDir / "sfm_data.json",
        "--match_dir", matchesDir,
        "--output_dir", reconstructionDir
    );
    progress_++;
    ChildProcess(
        toTerminate_,
        bp::search_path("openMVG_main_ComputeSfM_DataColor"),
        "-i", reconstructionDir / "sfm_data.bin",
        "-o", reconstructionDir / "colorized.ply"
    );
    progress_++;
}


} // namespace mixi

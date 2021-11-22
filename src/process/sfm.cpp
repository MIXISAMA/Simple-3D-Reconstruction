#include "process/sfm.h"

#include "util/child_process.h"


namespace mixi
{

// const float Sfm::InitSfmDirProgressWeight = 0.01;

Sfm::Sfm() :
    progress_(0.0f)
{
    
}

Sfm::~Sfm()
{

}

void Sfm::run(
    const fs::path& sfmDir,
    const std::vector<fs::path>& imageFiles,
    const std::string& param
)
{
    progress_ = 0.0f;

    initSfmDir_(sfmDir, imageFiles);

    progress_ = 0.01f;
    
    const boost::filesystem::path inputDir(sfmDir / "input");
    const boost::filesystem::path outputDir(sfmDir / "output");
    const boost::filesystem::path matchesDir(sfmDir / "matches");
    const boost::filesystem::path reconstructionDir(sfmDir / "reconstruction");

    float doneProgress = progress_;

    auto dealChild = [this, &doneProgress](
        ChildProcess&& c,
        const std::vector<std::string>& keywords
    ) {
        bp::ipstream& ips = c.eips();
        std::string line;
        float localProgress = 0.0f;

        while (c.running() && std::getline(ips, line)) {
            localProgress = ProgressParser(keywords, line, localProgress);
            progress_ = doneProgress + localProgress * 0.14f;
        }
        c.wait();

        doneProgress += 0.14f;
        progress_ = doneProgress;
    };

    auto ForEachCommand = [dealChild](auto&&... commands) {
		(
            dealChild(
                std::make_from_tuple<ChildProcess>(commands.first),
                commands.second
            ), ...
        );
	};

    ForEachCommand(
        std::make_pair(std::make_tuple(
            bp::search_path("openMVG_main_SfMInit_ImageListing"),
            "-i", inputDir,
            "-o", matchesDir,
            "-k", param
        ),  std::vector<std::string>{
            "Listing images"
        }),
        std::make_pair(std::make_tuple(
            bp::search_path("openMVG_main_ComputeFeatures"),
            "-i", matchesDir / "sfm_data.json",
            "-o", matchesDir,
            "-p", "HIGH",
            // "-m", "SIFT"
            "-m", "AKAZE_FLOAT"
        ),  std::vector<std::string> {
            "EXTRACT FEATURES"
        }),
        std::make_pair(std::make_tuple(
            bp::search_path("openMVG_main_PairGenerator"),
            "-i", matchesDir / "sfm_data.json",
            "-o", matchesDir / "pairs.bin"
        ),  std::vector<std::string> {
            // Empty
        }),
        std::make_pair(std::make_tuple(
            bp::search_path("openMVG_main_ComputeMatches"),
            "-i", matchesDir / "sfm_data.json",
            "-p", matchesDir / "pairs.bin",
            "-o", matchesDir / "matches.putative.bin",
            // "-n", "CASCADEHASHINGL2"
            "-n", "ANNL2"
            // "-r", "1.2"
        ),  std::vector<std::string> {
            "Regions Loading",
            "Matching"
        }),
        std::make_pair(std::make_tuple(
            bp::search_path("openMVG_main_GeometricFilter"),
            "-i", matchesDir / "sfm_data.json",
            "-m", matchesDir / "matches.putative.bin" ,
            "-g", "f",
            "-o", matchesDir / "matches.f.bin"
        ),  std::vector<std::string> {
            "Regions Loading",
            "Geometric filtering"
        }),
        std::make_pair(std::make_tuple(
            bp::search_path("openMVG_main_SfM"),
            "--sfm_engine", "INCREMENTAL",
            "--input_file", matchesDir / "sfm_data.json",
            "--match_dir", matchesDir,
            "--output_dir", reconstructionDir
            // "-f", "NONE"
            // "--resection_method", "3"
        ),  std::vector<std::string> {
            // Empty
        }),
        std::make_pair(std::make_tuple(
            bp::search_path("openMVG_main_ComputeSfM_DataColor"),
            "-i", reconstructionDir / "sfm_data.bin",
            "-o", reconstructionDir / "colorized.ply"
        ), std::vector<std::string> {
            "Compute scene structure color"
        })
    );

    progress_ = 1.0f;

}

float Sfm::progress()
{
    return progress_;
}

// void Sfm::terminate()
// {
//     toTerminate_ = true;
//     future_.wait();
//     toTerminate_ = false;
// }

void Sfm::initSfmDir_(
    const fs::path& sfmDir,
    const std::vector<fs::path>& imageFiles
)
{
    fs::path inputDir(sfmDir / "input");

    fs::create_directory(inputDir);
    fs::create_directory(sfmDir / "output");
    fs::create_directory(sfmDir / "matches");
    fs::create_directory(sfmDir / "reconstruction");

    for (int i = 0; i < imageFiles.size(); i++) {
        std::ostringstream oss;
        oss << i << imageFiles[i].filename().extension().string();
        fs::copy_file(imageFiles[i], inputDir / oss.str());
    }
}

float Sfm::ProgressParser(
    std::vector<std::string> keywords,
    std::string line,
    float defaultProgress
)
{
    float doneProgress = 0.0f;
    for (const std::string& keyword : keywords) {
        std::smatch result;
        std::regex pattern(R"(\[- )" + keyword + R"( -\] ([0-9]+)%)");
        if (std::regex_search(line, result, pattern) && result.size() == 2) {
            std::istringstream iss(result[1].str());
            int percent;
            iss >> percent;
            return percent / 100.0f / keywords.size() + doneProgress;
        }
        doneProgress += 1.0f / keywords.size();
    }
    return defaultProgress;
}

} // namespace mixi

#include "process/mvs.h"

#include "util/child_process.h"

namespace mixi
{

MvsDensePointCloud::MvsDensePointCloud() :
    IProgress()
{

}

void MvsDensePointCloud::run(
    const std::string& outputName,
    const fs::path& mvsInputFilePath
)
{
    progress_ = 0.2f;

    ChildProcess c(
        bp::search_path("DensifyPointCloud"),
        "-i", mvsInputFilePath.string(),
        "-o", outputName + "_dense_point_cloud",
        "-w", fs::absolute(mvsInputFilePath).parent_path().string()
    );

    bp::ipstream& ips = c.ips();
    std::string line;
    // float localProgress = 0.0f;

    while (c.running() && std::getline(ips, line)) {
        // std::cout << line << std::endl;
        // localProgress = ProgressParser(keywords, line, localProgress);
        // progress_ = doneProgress + localProgress * 0.14f;
    }
    c.wait();

    progress_ = 1.0f;
}

MvsRoughMesh::MvsRoughMesh() :
    IProgress()
{

}

void MvsRoughMesh::run(
    const std::string& outputName,
    const fs::path& mvsInputFilePath
)
{
    progress_ = 0.2f;

    ChildProcess c(
        bp::search_path("ReconstructMesh"),
        "-i", mvsInputFilePath.string(),
        "-o", outputName + "_rough_mesh",
        "-w", fs::absolute(mvsInputFilePath).parent_path().string()
    );

    bp::ipstream& ips = c.ips();
    std::string line;

    while (c.running() && std::getline(ips, line)) {
        // std::cout << line << std::endl;
    }
    c.wait();

    progress_ = 1.0f;
}

MvsRefineMesh::MvsRefineMesh() :
    IProgress()
{

}

void MvsRefineMesh::run(
    const std::string& outputName,
    const fs::path& mvsInputFilePath
)
{
    progress_ = 0.2f;

    ChildProcess c(
        bp::search_path("RefineMesh"),
        "-i", mvsInputFilePath.string(),
        "-o", outputName + "_refine_mesh",
        "-w", fs::absolute(mvsInputFilePath).parent_path().string(),
        "--max-face-area", "16"
    );

    bp::ipstream& ips = c.ips();
    std::string line;

    while (c.running() && std::getline(ips, line)) {
        // std::cout << line << std::endl;
    }
    c.wait();

    progress_ = 1.0f;
}

MvsTextureMesh::MvsTextureMesh() :
    IProgress()
{

}

void MvsTextureMesh::run(
    const std::string& outputName,
    const fs::path& mvsInputFilePath
)
{
    progress_ = 0.2f;

    ChildProcess c(
        bp::search_path("TextureMesh"),
        "-i", mvsInputFilePath.string(),
        "-o", outputName + "_texture_mesh",
        "-w", fs::absolute(mvsInputFilePath).parent_path().string(),
        "--export-type", "obj"
    );

    bp::ipstream& ips = c.ips();
    std::string line;

    while (c.running() && std::getline(ips, line)) {
        // std::cout << line << std::endl;
    }
    c.wait();

    progress_ = 1.0f;
}

} // namespace mixi
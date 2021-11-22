#pragma once

#include "pch.h"

#include "app/imgui_window.h"

#include "file/image.h"
#include "file/camera_parameter.h"

namespace mixi
{
namespace s3r
{

class SparsePointCloudInputListWindow : public app::ImguiWindow
{
public:
    
    SparsePointCloudInputListWindow();
    ~SparsePointCloudInputListWindow() = default;

    void render() override;

    bool checkRunnable() const;

    const CameraParameterFile* paramFile() const;
    std::vector<fs::path> imagePathes() const;

private:

    CameraParameterFile::Ptr paramFile_;
    std::set<fs::path> imagePathes_;

    void renderParamSelectableDnd_();
    void renderImageSelectableDnd_();

    void addImageSet_(const fs::path& path);
};

} // namespace s3r
} // namespace mixi



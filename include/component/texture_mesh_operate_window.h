#pragma once

#include "pch.h"

#include "app/imgui_window.h"

namespace mixi
{
namespace s3r
{


class TextureMeshOperateWindow : public app::ImguiWindow
{
public:

    TextureMeshOperateWindow(std::function<void(void)> onRunCallback);
    ~TextureMeshOperateWindow() = default;

    void render() override;

    void setProgress(float progress);

    const fs::path& mvsInputFilepath() const;

    const char* pkgName();

private:

    std::function<void(void)> onRunCallback_;

    float progress_;

    bool runnable_;

    fs::path mvsInputFilepath_;

    char pkgName_[50];

    void renderPathSelectableDnd_();

};


} // namespace s3r
} // namespace mixi

#pragma once

#include "pch.h"

#include "app/imgui_window.h"

namespace mixi
{
namespace s3r
{

class SparsePointCloudOperateWindow : public app::ImguiWindow
{
public:

    SparsePointCloudOperateWindow(std::function<void(void)> onSfmCallback);
    ~SparsePointCloudOperateWindow() = default;

    void render() override;

    void setProgress(float progress);

    void setRunnable(bool runnable);

    const char* pkgName();

private:

    std::function<void(void)> onSfmCallback_;

    float progress_;

    bool runnable_;

    char pkgName_[50];

};

} // namespace s3r
} // namespace mixi

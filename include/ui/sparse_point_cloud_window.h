#pragma once

#include "pch.h"
#include "app/imgui_window.h"

namespace mixi
{
namespace s3r
{
class SparsePointCloudWindow : public ImguiWindow
{
public:
    SparsePointCloudWindow();
    void render() override;
};

}
}
#pragma once

#include "pch.h"
#include "app/imgui_window.h"

namespace mixi
{
namespace s3r
{
class DensePointCloudWindow : public ImguiWindow
{
public:
    DensePointCloudWindow();
    void render() override;
};

}
}
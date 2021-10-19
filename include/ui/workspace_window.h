#pragma once

#include "pch.h"
#include "app/imgui_window.h"

namespace mixi
{
namespace s3r
{
class WorkspaceWindow : public app::ImguiWindow
{
public:
    WorkspaceWindow();
    void render() override;
};

}
}




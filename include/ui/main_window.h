#pragma once

#include "pch.h"
#include "app/imgui_window.h"
#include "component/usage_selector_window.h"
#include "workspace_window.h"
#include "output_file_window.h"


namespace mixi
{
namespace s3r // Simple 3D Reconstruction
{

class MainWindow : public app::ImguiWindow
{
public:

    MainWindow();
    ~MainWindow() = default;

    void render() override;

private:

    OutputFileWindow outputFileWindow_;
    UsageSelectorWindow usageSelectorWindow_;
    WorkspaceWindow workspaceWindow_;

    std::unique_ptr<ImguiWindow> usageWindow_;

    void renderUsageWindow_();

};


}
}



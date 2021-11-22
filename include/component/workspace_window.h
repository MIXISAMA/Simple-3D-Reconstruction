#pragma once

#include "pch.h"
#include "app/imgui_window.h"
#include "imfilebrowser.h"
#include "util/workspace_file_system.h"

#include "file/base.h"

namespace mixi
{
namespace s3r
{


class DndIntoWorkspaceBundle
{
public:
    void* data;
    int dataSize;
    fs::path relativePath;
};



class WorkspaceWindow : public app::ImguiWindow
{
public:

    WorkspaceWindow();

    void render() override;

private:

    vfs::Workspace::Ptr rootNode_;

    // std::set<fs::path> fileSelectedSet_;

    ImGui::FileBrowser fileDialog_;

    fs::path dndPath_;

    std::future<void> saveFuture_;
    ISaveable::Ptr saveableFile_;

    void renderFileDialog_();

    void renderWorkspace_();

    void renderFiles_(
        std::vector<vfs::Node*>& files,
        const fs::path& parentPath
    );
    void renderDirsRecursive_(
        std::vector<vfs::Node*>& dirs,
        const fs::path& parentPath
    );

    void renderSaveableDnd_(const fs::path& parentPath);

    void renderPathDnd_(const fs::path& path);

    void renderProgress_();

    bool isRenderProgress_();

    // void renderFileDnd_(const fs::path& parentPath);

};

}
}




#include "ui/workspace_window.h"

#include "util/file_util.h"


namespace mixi
{
namespace s3r
{

WorkspaceWindow::WorkspaceWindow() :
    rootNode_(nullptr),
    dndPath_(),
    // fileSelectedSet_(),
    fileDialog_(ImGuiFileBrowserFlags_SelectDirectory)
{
    fileDialog_.SetTitle("Choose a directory as the workspace");
}

void WorkspaceWindow::render()
{
    ImGui::Begin("Workspace");

    if (rootNode_ == nullptr ||
        !fs::exists(rootNode_->path())) {
        renderFileDialog_();
    }
    else {
        renderWorkspace_();
    }
    
    ImGui::End();
}


void WorkspaceWindow::renderFileDialog_()
{
    if (ImGui::Button("Open File Dialog")) {
        fileDialog_.Open();
    }

    fileDialog_.Display();

    if (!fileDialog_.HasSelected()) {
        return;
    }

    rootNode_ = vfs::Workspace::Ptr(
        new vfs::Workspace(fileDialog_.GetSelected())
    );
    fileDialog_.ClearSelected();
}

void WorkspaceWindow::renderWorkspace_()
{
    std::vector<vfs::Node*> childDirs = rootNode_->dirs();
    std::vector<vfs::Node*> childFiles = rootNode_->files();
    renderDirsRecursive_(childDirs, rootNode_->path());
    renderFiles_(childFiles, rootNode_->path());
}

void WorkspaceWindow::renderFiles_(
    std::vector<vfs::Node*>& files,
    const fs::path& parentPath)
{
    for (vfs::Node* file : files) {
        ImGui::Selectable(file->nodeName().c_str());
        renderPathDnd_(parentPath / file->nodeName());
    }
}

void WorkspaceWindow::renderDirsRecursive_(
    std::vector<vfs::Node*>& dirs,
    const fs::path& parentPath)
{
    for (vfs::Node* dir : dirs) {
        const fs::path& dirName = dir->nodeName();
        const fs::path dirPath = parentPath / dirName;
        bool isOpen = ImGui::TreeNode(dirName.c_str());
        renderSaveableDnd_(dirPath);
        renderPathDnd_(dirPath);

        if (!isOpen) {
            if (dir->isOpening()) {
                dir->close();
            }
            continue;
        }
        
        if (!dir->isOpening()) {
            dir->open();
        }
        
        std::vector<vfs::Node*> childDirs = dir->dirs();
        std::vector<vfs::Node*> childFiles = dir->files();
        renderDirsRecursive_(childDirs, dirPath);
        renderFiles_(childFiles, dirPath);

        ImGui::TreePop();
    }
}

void WorkspaceWindow::renderSaveableDnd_(const fs::path& parentPath)
{
    if (!ImGui::BeginDragDropTarget()) {
        return;
    }
    
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_SAVEABLE")) {
        IM_ASSERT(payload->DataSize == sizeof(ISaveable::Ptr));
        ISaveable::Ptr& dir = *(ISaveable::Ptr*)payload->Data;
        dir->save(parentPath);
    }

    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PATH")) {
        IM_ASSERT(payload->DataSize == sizeof(fs::path*));
        const fs::path& path = *(fs::path*)payload->Data;
        // rootDir_ = Directory::Ptr(new Directory(path.filename()));
    }

    ImGui::EndDragDropTarget();
}

void WorkspaceWindow::renderPathDnd_(const fs::path& path)
{
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
        dndPath_ = path;
        ImGui::SetDragDropPayload("DND_PATH", &dndPath_, sizeof(fs::path));
        ImGui::Text("Move or Use %s", path.filename().c_str());
        ImGui::EndDragDropSource();
    }
}

} // namespace s3r
} // namespace mixi

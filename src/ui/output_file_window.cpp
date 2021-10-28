#include "ui/output_file_window.h"

namespace mixi
{
namespace s3r
{


OutputFileWindow::Context::Context() :
    dirs_()
{

}

OutputFileWindow::Context::~Context()
{

}

void OutputFileWindow::Context::add(MemoryDirectory::Ptr& memoryDir)
{
    dirs_.push_back(memoryDir);
}

OutputFileWindow::OutputFileWindow() :
    context_()
{

}

OutputFileWindow::~OutputFileWindow()
{

}

void OutputFileWindow::render()
{
    ImGui::Begin("Output Files");

    for (MemoryDirectory::Ptr& dir : context_.dirs_) {
        renderFileTreeRecursive_(dir);
    }
    
    ImGui::End();
}

OutputFileWindow::Context* OutputFileWindow::context()
{
    return &context_;
}

OutputFileWindow::IContext::IContext(Context* outputContext) :
    outputContext_(outputContext)
{

}

OutputFileWindow::IContext::~IContext()
{

}

void OutputFileWindow::renderFileTreeRecursive_(MemoryDirectory::Ptr& dir)
{
    bool isOpen = ImGui::TreeNode(dir->filename().c_str());
    renderSaveableDnd_((ISaveable::Ptr*)&dir);
    if (!isOpen) {
        return;
    }
    for (MemoryDirectory::Ptr& childDir : dir->dirs) {
        renderFileTreeRecursive_(childDir);
    }
    for (MemoryFile::Ptr& file : dir->files) {
        ImGui::Text("%s", file->filename().c_str());
        renderSaveableDnd_((ISaveable::Ptr*)&file);
    }
    ImGui::TreePop();
}

void OutputFileWindow::renderSaveableDnd_(ISaveable::Ptr* dir)
{
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
        ImGui::SetDragDropPayload("DND_SAVEABLE", dir, sizeof(ISaveable::Ptr));
        ImGui::Text("Save or Use %s", (*dir)->filename().c_str());
        ImGui::EndDragDropSource();
    }
}

} // namespace s3r
} // namespace mixi

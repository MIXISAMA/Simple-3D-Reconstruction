#include "component/sparse_point_cloud_input_list_window.h"

namespace mixi
{
namespace s3r
{

SparsePointCloudInputListWindow::SparsePointCloudInputListWindow() :
    paramFile_(nullptr),
    imagePathes_()
{

}

void SparsePointCloudInputListWindow::render()
{
    ImGui::Begin("Input List");

    ImGui::Text("Camera Parameter:");
    std::string paramSelectableName = 
        paramFile_ == nullptr ?
        "no parameter" :
        paramFile_->filename();

    ImGui::Selectable(paramSelectableName.c_str());
    renderParamSelectableDnd_();

    ImGui::Separator();

    ImGui::Text("Image List:");
    if (ImGui::BeginListBox("##image listbox", ImGui::GetContentRegionAvail())) {
        for (const fs::path& imageFile : imagePathes_) {
            ImGui::Selectable(imageFile.filename().c_str());
        }
        ImGui::EndListBox();
        renderImageSelectableDnd_();
    }

    ImGui::End();
}

void SparsePointCloudInputListWindow::renderParamSelectableDnd_()
{
    if (!ImGui::BeginDragDropTarget()) {
        return;
    }
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PATH")) {
        IM_ASSERT(payload->DataSize == sizeof(fs::path));
        fs::path* path = (fs::path*)payload->Data;
        try {
            paramFile_ = CameraParameterFile::Ptr(
                new CameraParameterFile(*path)
            );
        }
        catch (cv::Exception e) {
            // Todo: error tip
        }
    }
    ImGui::EndDragDropTarget();
}

void SparsePointCloudInputListWindow::renderImageSelectableDnd_()
{
    if (!ImGui::BeginDragDropTarget()) {
        return;
    }
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PATH")) {
        IM_ASSERT(payload->DataSize == sizeof(fs::path));
        fs::path* path = (fs::path*)payload->Data;
        addImageSet_(*path);
    }
    ImGui::EndDragDropTarget();
}

void SparsePointCloudInputListWindow::addImageSet_(const fs::path& path)
{
    if (!fs::is_directory(path)) {
        if (path.extension() == ".png") {
            imagePathes_.insert(path);
        }
        return;
    }

    for (const auto& iter : fs::directory_iterator(path)) {
        addImageSet_(iter.path());
    }
}

bool SparsePointCloudInputListWindow::checkRunnable() const
{
    return paramFile_ != nullptr && imagePathes_.size() >= 2;
}

const CameraParameterFile* SparsePointCloudInputListWindow::paramFile() const
{
    return paramFile_.get();
}

std::vector<fs::path> SparsePointCloudInputListWindow::imagePathes() const
{
    return std::vector<fs::path>(imagePathes_.begin(), imagePathes_.end());
}

}
}
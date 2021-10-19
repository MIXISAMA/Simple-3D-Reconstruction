#include "ui/usage_selector_window.h"

#include "render/texture.h"
#include "util/image.h"

namespace mixi
{
namespace s3r
{

UsageSelectorWindow::UsageSelectorWindow() :
    selected_(Usage::CAMERA)
{
    addItem(
        Usage::CAMERA,
        "Camera and Calibration",
        "assets/camera.png",
        true
    );
    addItem(
        Usage::SPARSE_POINT_CLOUD,
        "Sparse Point Cloud",
        "assets/sparse_points.png",
        true
    );
    addItem(
        Usage::DENSE_POINT_CLOUD,
        "Dense Point Cloud",
        "assets/dense_points.png",
        true
    );
}

void UsageSelectorWindow::addItem(
    Usage usage,
    const char* tip,
    const char* imagePath,
    bool invertedColor)
{
    Image icon(imagePath);
    if (invertedColor) {
        icon.invertedColor();
    }
    Texture::Ptr texture(new Texture());
    texture->updateRgbaImage2D(icon.width(), icon.height(), icon.data());
    items_.push_back({usage, tip, texture});
}

void UsageSelectorWindow::render()
{
    ImGui::Begin("Usage");

    for (int i = 0; i < items_.size(); i++) {
        Item& item = items_[i];

        ImGui::PushID(i);

        bool click = ImGui::ImageButton(
            (void*)(intptr_t)item.texture->id(),
            ImVec2(64, 64)
        );
        if (click) {
            selected_ = item.usage;
        }

        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("%s", item.tip);
        }

        ImGui::PopID();
    }

    ImGui::End();
}

UsageSelectorWindow::Usage UsageSelectorWindow::selected() const
{
    return selected_;
}

} // namespace s3r
} // namespace mixi


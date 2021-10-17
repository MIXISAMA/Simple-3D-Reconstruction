#pragma

#include "pch.h"
#include "app/imgui_window.h"

#include "render/texture.h"

namespace mixi
{
namespace s3r
{

class UsageSelectorWindow : public ImguiWindow
{
public:

    enum class Usage
    {
        CAMERA,
        SPARSE_POINT_CLOUD,
        DENSE_POINT_CLOUD,
    };

    UsageSelectorWindow();

    void render() override;

    Usage selected() const;

private:

    void addItem(
        Usage usage,
        const char* tip,
        const char* imagePath,
        bool invertedColor
    );

    struct Item
    {
        Usage usage;
        const char* tip;
        Texture::Ptr texture;
    };

    Usage selected_;

    std::vector<Item> items_;

};
    
}
}


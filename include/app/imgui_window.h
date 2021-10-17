#ifndef _IMGUI_WINDOW_H
#define _IMGUI_WINDOW_H

class ImguiWindow
{
public:
    using Ptr = std::shared_ptr<ImguiWindow>;
    virtual void render() = 0;
};

#endif
#ifndef _IMGUI_WINDOW_H
#define _IMGUI_WINDOW_H

#include "util/image_util.h"

class ImguiWindow
{
public:
    virtual void init() = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;
};

#endif
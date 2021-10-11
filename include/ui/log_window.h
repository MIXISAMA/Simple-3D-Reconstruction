#ifndef _LOG_WINDOW_H
#define _LOG_WINDOW_H

#include "app/imgui_window.h"

class LogWindow : public ImguiWindow
{
public:
    void init() override;
    void render() override;
    void destroy() override;
};

#endif

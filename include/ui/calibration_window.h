#ifndef _CALIBRATION_WINDOW_H
#define _CALIBRATION_WINDOW_H

#include "app/imgui_window.h"

class CalirationWindow : public ImguiWindow
{
public:
    void init() override;
    void render() override;
    void destroy() override;
};

#endif

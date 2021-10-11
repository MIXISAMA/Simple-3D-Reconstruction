#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include "pch.h"
#include "app/imgui_window.h"
#include "ui/calibration_window.h"
#include "ui/camera_window.h"

class MainWindow : public ImguiWindow
{
public:
    MainWindow();
    void init() override;
    void render() override;
    void destroy() override;
private:
    Texture menuBtnTexCalibration;
    Texture menuBtnTexCamera;
    CalirationWindow calirationWindow;
    CameraWindow cameraWindow;
    ImguiWindow* currentWindow;
};

#endif

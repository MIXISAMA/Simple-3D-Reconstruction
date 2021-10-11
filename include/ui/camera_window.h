#ifndef _CAMERA_WINDOW_H
#define _CAMERA_WINDOW_H

#include "app/imgui_window.h"
#include "util/my_usb_camera.h"

class CameraWindow : public ImguiWindow
{
public:
    CameraWindow();
    void init() override;
    void render() override;
    void destroy() override;
private:
    GLuint frameTexture_;
};

#endif

#include "ui/camera_window.h"

#include "imgui.h"

CameraWindow::CameraWindow() :
    frameTexture_(0)
{

}

void CameraWindow::init()
{
    glGenTextures(1, &frameTexture_);
    glBindTexture(GL_TEXTURE_2D, frameTexture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void CameraWindow::destroy()
{
    glDeleteTextures(1, &frameTexture_);
}

void CameraWindow::render()
{
    MyUsbCamera::usbCam_ptr camera = MyUsbCamera::Instance();
    ImGui::Begin("Camera");
    ImGui::Text("Camera from window!");
    if (ImGui::Button("Find Camera")) {
        camera->refresh();
    }
    ImGui::SameLine();

    if (camera->isFound()) {
        MyUsbCamera::Info cameraInfo = camera->getRecentInfo();
        // std::string cameraHintStr = std::to_string(cameraInfo.terminalId);
        // const char* cameraHint = cameraHintStr.c_str();
        ImGui::Text(
            "camera %d %dx%d fps:%d",
            cameraInfo.terminalId,
            cameraInfo.width,
            cameraInfo.height,
            cameraInfo.fps
        );
        ImGui::NewLine();
        if (ImGui::Button("Start") && !camera->isStarting()) {
            camera->start();
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop") && camera->isStarting()) {
            camera->stop();
        }
        if (camera->isStarting()) {
            camera->loadFrameRgb(&frameTexture_);
            int width = cameraInfo.width * 300.0 / cameraInfo.height;
            int height = 300;
            ImGui::Image((void*)(intptr_t)frameTexture_, ImVec2(width, height));
        }
    }
    else {
        ImGui::Text("no camera found");
    }

    ImGui::End();
}

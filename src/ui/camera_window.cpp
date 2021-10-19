#include "ui/camera_window.h"

#include "util/usb_camera.h"

namespace mixi
{
namespace s3r
{

CameraWindow::CameraWindow() :
    cameraTexture_()
{

}

void CameraWindow::render()
{
    ImGui::Begin("Camera");
    ImGui::Text("Camera from window!");
    renderOriginalCamera_();
    ImGui::End();
}

void CameraWindow::renderOriginalCamera_()
{
    
    UsbCamera& camera = Singleton<UsbCamera>::Instance();

    if (ImGui::Button("Find Camera")) {
        camera.refresh();
    }
    
    UsbCamera::Status status = camera.getStatus();

    if (status != UsbCamera::Status::NOT_STARTED &&
        status != UsbCamera::Status::STARTING) {
        ImGui::SameLine();
        ImGui::Text("no camera found");
        return;
    }

    UsbCamera::Info cameraInfo = camera.getRecentInfo();
    ImGui::SameLine();
    ImGui::Text(
        "camera %d %dx%d fps:%d",
        cameraInfo.terminalId,
        cameraInfo.width,
        cameraInfo.height,
        cameraInfo.fps
    );
    ImGui::NewLine();
    if (status == UsbCamera::Status::STARTING) {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button("Start")) {
        camera.start();
    }
    if (status == UsbCamera::Status::STARTING) {
        ImGui::EndDisabled();
    }
    ImGui::SameLine();
    if (status == UsbCamera::Status::NOT_STARTED) {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button("Stop")) {
        camera.stop();
    }
    if (status == UsbCamera::Status::NOT_STARTED) {
        ImGui::EndDisabled();
    }
    if (status == UsbCamera::Status::STARTING &&
        camera.getFrameCounter() > 2
    ) {
        const void* data = camera.fetchFrameRgbData();
        cameraTexture_.updateRgbImage2D(cameraInfo.width, cameraInfo.height, data);
        int height = 300;
        int width = cameraInfo.width * 300.0 / cameraInfo.height;
        ImGui::Image((void*)(intptr_t)cameraTexture_.id(), ImVec2(width, height));
    }

}


} // namespace s3r
} // namespace mixi


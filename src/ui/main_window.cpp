#include "ui/main_window.h"
#include "ui/calibration_window.h"
#include "util/image_util.h"

MainWindow::MainWindow() :
    currentWindow(&cameraWindow)
{

}

void MainWindow::init()
{
    ImVec4 color = ImGui::GetStyleColorVec4(0);
    float light = color.x * 0.299f + color.y * 0.587f + color.z * 0.114f;
    bool revertColor = light > 0.5f;

    menuBtnTexCamera = LoadTextureFromFile("assets/211634_camera_icon.png", revertColor);
    menuBtnTexCalibration = LoadTextureFromFile("assets/6602457_deform_design_graphic_icon.png", revertColor);
    calirationWindow.init();
    cameraWindow.init();
}

void MainWindow::destroy()
{
    calirationWindow.destroy();
    cameraWindow.destroy();
}

void MainWindow::render()
{
    ImGui::Begin("Usage");

    ImGui::PushID(0);
    if (ImGui::ImageButton(
            (void*)(intptr_t)menuBtnTexCalibration.texture,
            ImVec2(menuBtnTexCalibration.width, menuBtnTexCalibration.height)
        )) {
        currentWindow = &calirationWindow;
    }
    ImGui::PopID();
    ImGui::PushID(1);
    if (ImGui::ImageButton(
            (void*)(intptr_t)menuBtnTexCamera.texture,
            ImVec2(menuBtnTexCamera.width, menuBtnTexCamera.height)
        )) {
        currentWindow = &cameraWindow;
    }
    ImGui::PopID();

    // ImGui::NewLine();
    // ImGui::Text("Pressed %d times.", pressed_count);
    ImGui::End();

    currentWindow->render();
}
#include "ui/calibration_window.h"
#include "imgui.h"

void CalirationWindow::init()
{

}

void CalirationWindow::destroy()
{

}

void CalirationWindow::render()
{
    ImGui::Begin("Caliration");
    ImGui::Text("Caliration from window!");

    ImGui::End();
}
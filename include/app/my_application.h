#ifndef _MY_APPLICATION_H
#define _MY_APPLICATION_H

#include "pch.h"
#include "app/imgui_window.h"

class MyApplication
{
public:
    MyApplication(char* arg, ImguiWindow& mainWindow);
    ~MyApplication();
    void loop() const;
private:
    GLFWwindow* glfwWindow;
    ImguiWindow& mainWindow;
    bool appShouldClose() const;
    void appGlogInit(char* arg) const;
    void appGlogShutdown() const;
    void appGlfwInit();
    void appGlfwPreRender() const;
    void appGlfwPostRender() const;
    void appGlfwTerminate() const;
    void appImguiInit() const;
    void appImguiPreRender() const;
    void appImguiPostRender() const;
    void appImguiShutdown() const;
    static void AppGlfwErrorCallback(int error, const char* description);
};
#endif
#pragma once

#include "pch.h"
#include "imgui_window.h"

namespace mixi
{
namespace app
{

class Glog
{
public:
    Glog(const char* arg, const char* log_dir);
    ~Glog();
};


class Glfw
{
public:
    Glfw();
    ~Glfw();
    GLFWwindow* glfwWindow() const;
    void preRender() const;
    void postRender() const;
private:
    GLFWwindow* glfwWindow_;
    static void ErrorCallback_(int error, const char* description);
};


class Imgui
{
public:
    Imgui(GLFWwindow* glfwWindow);
    ~Imgui();
    void preRender() const;
    void postRender() const;
};



class Application
{
public:
    Application(const char* arg);
    ~Application();
    void loop(ImguiWindow* mainWindow) const;
private:
    Glog glog_;
    Glfw glfw_;
    Imgui imgui_;
    ImguiWindow* mainWindow_;
    bool shouldClose_() const;
    static void AppGlfwErrorCallback(int error, const char* description);
};

}
}


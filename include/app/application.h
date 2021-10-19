#pragma once

#include "pch.h"
#include "imgui_window.h"

namespace mixi
{
namespace app
{


class Application
{
public:

    Application(const char* arg);
    ~Application();

    void loop(ImguiWindow* mainWindow) const;

private:

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

        static void MouseButtonCallback_(GLFWwindow* window, int button, int action, int mods);
        static void CursorPosCallback_(GLFWwindow* window, double xpos, double ypos);
        static void ScrollCallback_(GLFWwindow* window, double xoffset, double yoffset);

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

    Glog glog_;
    Glfw glfw_;
    Imgui imgui_;

    ImguiWindow* mainWindow_;

    bool shouldClose_() const;

};

}
}


#include "app/application.h"

namespace mixi
{
namespace app
{

Application::Glog::Glog(const char* arg, const char* log_dir)
{
    FLAGS_log_dir = log_dir;
    google::InitGoogleLogging(arg);
}

Application::Glog::~Glog()
{
    google::ShutdownGoogleLogging();
}

Application::Glfw::Glfw()
{
    glfwSetErrorCallback(ErrorCallback_);
    if (!glfwInit()) {
        throw std::runtime_error("glfw init error!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only

#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#endif

    glfwWindow_ = (GLFWwindow*)glfwCreateWindow(1280, 720, "Simple 3D Reconstruction", NULL, NULL);
    if (glfwWindow_ == nullptr) {
        glfwTerminate();
        throw std::runtime_error("create main window error!");
    }
    glfwMakeContextCurrent(glfwWindow_);
    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD!");
    }
    glfwSwapInterval(1); // Enable vsync

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

Application::Glfw::~Glfw()
{
    glfwTerminate();
}

GLFWwindow* Application::Glfw::glfwWindow() const
{
    return glfwWindow_;
}

void Application::Glfw::preRender() const
{
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    int display_w, display_h;
    glfwGetFramebufferSize(glfwWindow_, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Application::Glfw::postRender() const
{
    glfwPollEvents();
    glfwSwapBuffers(glfwWindow_);
}

void Application::Glfw::ErrorCallback_(int error, const char* description)
{
    LOG(ERROR) << "Glfw Error " << error << ": " << description;
}


Application::Imgui::Imgui(GLFWwindow* glfwWindow)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigWindowsMoveFromTitleBarOnly=true;
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
#if defined(__APPLE__)
    // GL 3.3 + GLSL 150
    const char* glsl_version = "#version 150";
#else
    // GL 3.3 + GLSL 130
    const char* glsl_version = "#version 130";
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);
}

Application::Imgui::~Imgui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::Imgui::preRender() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
}

void Application::Imgui::postRender() const
{
    ImGui::Render();
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Application::Application(const char* arg) :
    glog_(arg, "./log"),
    glfw_(),
    imgui_(glfw_.glfwWindow()),
    mainWindow_(nullptr)
{
    LOG(INFO) << "Application Started";
}

Application::~Application()
{
    LOG(INFO) << "Application Stopped";
}

void Application::loop(ImguiWindow* mainWindow) const
{
    while (!shouldClose_()) {
        glfw_.preRender();
        imgui_.preRender();
        mainWindow->render();
        imgui_.postRender();
        glfw_.postRender();
    }
}

bool Application::shouldClose_() const
{
    return glfwWindowShouldClose(glfw_.glfwWindow());
}


} // namespace app
} // namespace mixi


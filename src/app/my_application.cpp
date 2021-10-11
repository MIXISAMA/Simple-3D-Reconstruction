#include "app/my_application.h"

MyApplication::MyApplication(char* arg, ImguiWindow& mainWindow) :
    mainWindow(mainWindow),
    glfwWindow(nullptr)
{
    appGlogInit(arg);
    appGlfwInit();
    appImguiInit();
    mainWindow.init();
}

MyApplication::~MyApplication()
{
    mainWindow.destroy();
    appImguiShutdown();
    appGlfwTerminate();
    appGlogShutdown();
}

void MyApplication::loop() const
{
    while (!appShouldClose()) {
        appGlfwPreRender();
        appImguiPreRender();
        mainWindow.render();
        appImguiPostRender();
        appGlfwPostRender();
    }
}

bool MyApplication::appShouldClose() const
{
    return glfwWindowShouldClose(glfwWindow);
}

void MyApplication::appGlogInit(char* arg) const
{
    FLAGS_log_dir = "./log";
    google::InitGoogleLogging(arg);
}

void MyApplication::appGlogShutdown() const
{
    google::ShutdownGoogleLogging();
}

void MyApplication::appGlfwInit()
{
    glfwSetErrorCallback(AppGlfwErrorCallback);
    if (!glfwInit()) {
        throw std::runtime_error("glfw init error!");
    }

#if defined(__APPLE__)
    // GL 3.2 + GLSL 150
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    glfwWindow = (GLFWwindow*)glfwCreateWindow(1280, 720, "Simple 3D Reconstruction", NULL, NULL);
    if (glfwWindow == nullptr) {
        glfwTerminate();
        throw std::runtime_error("create main window error!");
    }
    glfwMakeContextCurrent(glfwWindow);
    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD!");
    }
    glfwSwapInterval(1); // Enable vsync
}

void MyApplication::appGlfwPreRender() const
{
    // Todo: Create and destroy the same object repeatedly.
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    int display_w, display_h;
    glfwGetFramebufferSize(glfwWindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void MyApplication::appGlfwPostRender() const
{
    glfwPollEvents();
    glfwSwapBuffers(glfwWindow);
}

void MyApplication::appGlfwTerminate() const
{
    glfwTerminate();
}

void MyApplication::appImguiInit() const
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
#if defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void MyApplication::appImguiPreRender() const
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

void MyApplication::appImguiPostRender() const
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

void MyApplication::appImguiShutdown() const
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void MyApplication::AppGlfwErrorCallback(int error, const char* description)
{
    LOG(ERROR) << "Glfw Error " << error << ": " << description;
}
#include "Application.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>

Application::Application(int width, int height, const char *title) :
graphLayout(Position{500.f, 500.f})
{
    if (!InitGLFW(width, height, title)) {
        std::exit(1);
    }

    InitImGui();

    // testing/debug
    graphLayout.AddNode({0.0f, 0.0f}, true, "home");
    graphLayout.AddChildInOrbit(0, 60.f, -90.f, false, "text.txt");
    graphLayout.AddChildInOrbit(0, 60.f, -10.f, true, "test_dir");
    graphLayout.AddChildInOrbit(2, 60.f, -40.f, false, "test_file");
    graphLayout.PrintAllNodes();
}

Application::~Application() {
    Shutdown();
}

bool Application::InitGLFW(const int width, const int height, const char *title) {
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW." << std::endl;
        return false;
    }

    const int platform = glfwGetPlatform();
    std::cout << "is GLFW Platform == X11: " << (platform == GLFW_PLATFORM_X11 ? "yes" : "no") << std::endl;

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync
    return true;
}

void Application::InitImGui() const {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //const ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Application::Shutdown() const {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void Application::HandleInput() const {
    const ImVec2 mousePos = ImGui::GetMousePos();

    if (ImGui::IsMouseClicked(0)) {
        if ( std::optional<size_t> clickedIndex = graphLayout.GetNodeIndexAtPosition({mousePos.x, mousePos.y}) ) {
            const auto& clickedNode = graphLayout.GetNodes()[*clickedIndex];
            std::cout << "click on " << clickedNode.GetPath() << std::endl;
        }
        else {
            std::cout << "click on empty space" << std::endl;
        }
    }
}

void Application::Run() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // ImGui frame start
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        HandleInput();
        // graph rendering
        graphRenderer.Render(graphLayout);

        // End of ImGui frame, OpenGL rendering
        ImGui::Render();
        glClearColor(0.05098f, 0.10196f, 0.30196f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}
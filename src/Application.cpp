#include "Application.h"
#include "FileSystemScanner.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>

#include "FileOperations.h"

Application::Application(const int width, const int height, const char *title) {
    if (!InitGLFW(width, height, title)) {
        std::exit(1);
    }

    InitImGui();

    const std::string home = std::getenv("HOME");
    graphLayout.AddMainNode(home);
    FileSystemScanner::BuildFromDirectory(home, graphLayout, 0);
    //graphLayout.PrintAllNodes();
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

void Application::HandleInput(const ImGuiIO& io) {
    if (!nodeNamePopup.IsOpen()) {
        const auto mousePos = Position{ImGui::GetMousePos().x, ImGui::GetMousePos().y};

        if (ImGui::IsMouseClicked(0)) {
            if (const std::optional<size_t> clickedIndex = graphLayout.GetNodeIndexAtPosition(camera.ScreenToWorld(mousePos)) ) {
                auto& clickedNode = graphLayout.GetNodes()[*clickedIndex];
                graphLayout.PrintNodeInfo(clickedNode);

                graphLayout.SetSelectedNode(*clickedIndex);
                if (clickedNode.IsDirectory() && !clickedNode.IsOpen()) {
                    const std::string clickedNodePath = clickedNode.GetPath();
                    FileSystemScanner::BuildFromDirectory(clickedNodePath, graphLayout, *clickedIndex);
                    graphLayout.SetOpened(*clickedIndex, true);
                }
            } else {
                std::cout << "click on empty space" << std::endl;
            }
        }

        if (const auto selectIdxOpt = graphLayout.GetSelectedNodeIndex()) {
            const Node& selectedNode = graphLayout.GetNodes()[*selectIdxOpt];

            if (ImGui::IsKeyPressed(ImGuiKey_N, false) && selectedNode.IsDirectory()) {
                nodeNamePopup.Open(PopupMode::Create);
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_F2, false)) {
                const std::string selectedNodeName = selectedNode.GetName();
                nodeNamePopup.Open(PopupMode::Rename, selectedNodeName);
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_Delete, false)) {
                const std::string selectedNodePath = selectedNode.GetPath();
                FileOperations::DeleteFile(selectedNodePath, graphLayout);
            }
        }
    } else {
        if (ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
            nodeNamePopup.Close();
        }
    }

    constexpr float WHEEL_SENSITIVITY = 8.f;
    const float wheelV = io.MouseWheel * WHEEL_SENSITIVITY;
    const float wheelH = io.MouseWheelH * WHEEL_SENSITIVITY;
    if (wheelV != 0.f) {
        if (io.KeyShift) {
            camera.MoveBy(-wheelV, 0.f);
        } else {
            camera.MoveBy(0.f, wheelV);
        }
    }
    if (wheelH != 0.f) {
        camera.MoveBy(wheelH, 0.f);
    }

}

void Application::Run() {
    const ImGuiIO& io = ImGui::GetIO();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // ImGui frame start
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        HandleInput(io);

        graphLayout.UpdatePhysics(io.DeltaTime);

        windowManager.DrawWindows(graphLayout, camera, nodeNamePopup, io.DisplaySize);

        // End of ImGui frame, OpenGL rendering
        ImGui::Render();
        glClearColor(0.05098f, 0.10196f, 0.30196f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

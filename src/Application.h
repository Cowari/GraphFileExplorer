#pragma once
#include <GLFW/glfw3.h>

#include "GraphLayout.h"
#include "WindowManager.h"

class Application {
    public:
    Application(int width, int height, const char* title);
    ~Application();

    // Infinite Loop
    void Run();

    private:
    bool InitGLFW(int width, int height, const char *title);
    void InitImGui() const;
    void Shutdown() const;

    void HandleInput(const ImGuiIO& io);

    GLFWwindow* window = nullptr;

    GraphLayout graphLayout;
    WindowManager windowManager;
    Camera camera;
};

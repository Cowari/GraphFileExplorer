#pragma once
#include <GLFW/glfw3.h>

#include "GraphLayout.h"
#include "GraphRenderer.h"

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

    void HandleInput() const;

    GLFWwindow* window = nullptr;

    GraphLayout graphLayout;
    GraphRenderer graphRenderer;
};

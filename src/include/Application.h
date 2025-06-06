#pragma once

#include "VulkanRenderer.h"
#include "Game2048.h"
#include "View2048.h"
#include "Scene.h"
#include "Resources.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Application {
public:
	void run();
    void initWindow();
    void initVulkan();
    void initGame();
    void mainLoop();
    void cleanup();

    void onFramebufferResized();

private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    int fpsCounter = 0;
    int lastSec = 0;

    GLFWwindow* window;
    VulkanRenderer vulkan;
    Game2048 game;
    Resources resources;
    Scene scene;
    View2048 view;
};
#pragma once

#include "VulkanRenderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Application {
public:
	void run();
    void initWindow();
    void initVulkan();
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
};
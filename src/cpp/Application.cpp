#include "Application.h"

#include <iostream>

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->onFramebufferResized();
}

void Application::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void Application::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "2048 Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Application::initVulkan() {
    vulkan.setup(window);
    vulkan.init();
}

void Application::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        float time = static_cast<float>(glfwGetTime());
        fpsCounter++;

        if ((int)std::floor(time) > lastSec) {
            lastSec++;
            std::cout << fpsCounter << std::endl;
            fpsCounter = 0;
        }

        glfwPollEvents();
        vulkan.render();
    }
}

void Application::cleanup() {
    vulkan.cleanup();
}

void Application::onFramebufferResized() {
    vulkan.framebufferResized = true;
}

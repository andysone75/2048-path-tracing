#include "Application.h"

#include <iostream>

void Application::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void Application::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "2048 Vulkan", nullptr, nullptr);
}

void Application::initVulkan() {
    vulkan.init();
}

void Application::mainLoop() {
    vulkan.render();
}

void Application::cleanup() {
    vulkan.cleanup();
}

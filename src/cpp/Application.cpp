#include "Application.h"

#include <iostream>

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->onFramebufferResized();
}

void Application::run() {
    initWindow();
    initVulkan();
    initGame();
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

void Application::initGame() {
    game.init(10);
    view.init(&game, &scene);

    resources.initialize(vulkan);
    scene.initialize(&resources);
    game.reset();
    view.updateBoardFast();
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
        
        std::vector<std::pair<Model, glm::vec3>> objects;
        for (const int objectId : scene.getOpaqueObjects()) {
            SceneObject& object = scene.getObject(objectId);
            if (object.isActive) {
                objects.push_back(std::make_pair(object.model, object.position));
            }
        }
        
        vulkan.render(objects);
    }
}

void Application::cleanup() {
    vulkan.cleanup();
}

void Application::onFramebufferResized() {
    vulkan.framebufferResized = true;
}

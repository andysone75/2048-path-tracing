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

    void onFramebufferResized(int width, int height);
    void keyCallback(int key, int action);

private:
    const uint32_t WIDTH = 576;
    const uint32_t HEIGHT = 1024;

    float lastTime = 0.0f;
    int fpsCounter = 0;
    int lastSec = 0;

    GLFWwindow* window;
    VulkanRenderer vulkan;
    Game2048 game;
    Resources resources;
    Scene scene;
    View2048 view;

    Camera camera;
    float cameraAngleOffset = 0.0f;
    float cameraStartAngle = 25.0f;
    float cameraAngle;
    float cameraSpeed = 160.0f;
    float cameraRadius = 10;
    float cameraHeight = 8;
    glm::vec3 cameraOffset = glm::vec3(1.6f, 0.0f, 1.6f);

    int moveInputs[4] = {
        GLFW_KEY_UP,
        GLFW_KEY_RIGHT,
        GLFW_KEY_DOWN,
        GLFW_KEY_LEFT };

    void moveCameraRight();
    void moveCameraLeft();
    void go(MoveDirection direction);
    void undoMove();
};
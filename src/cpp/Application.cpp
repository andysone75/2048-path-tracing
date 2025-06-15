#include "Application.h"
#include "Utils.h"

#include <iostream>

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->onFramebufferResized(width, height);
}

void keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->keyCallback(key, action);
}

inline glm::vec3 getCameraPos(float angle, float radius, float height) {
    return { cos(glm::radians(angle)) * radius, height, sin(glm::radians(angle)) * radius };
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
    window = glfwCreateWindow(WIDTH, HEIGHT, "2048 Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetKeyCallback(window, keyCallbackWrapper);
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

    cameraOffset = glm::vec3(1.6, 0, 1.6);
    camera.target = cameraOffset;
    camera.up = glm::vec3(0, 1, 0);
    camera.fovY = 11;
    camera.aspect = (float)WIDTH / HEIGHT;
    camera.nearZ = .01f;
    camera.farZ = 50.0f;
    cameraAngle = cameraStartAngle;
}

void Application::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        float time = static_cast<float>(glfwGetTime());
        float dt = time - lastTime;
        lastTime = time;

        fpsCounter++;
        if ((int)std::floor(time) > lastSec) {
            lastSec++;
            std::cout << fpsCounter << std::endl;
            fpsCounter = 0;
        }

        glfwPollEvents();
        
        std::vector<SceneObject> objects;
        for (const int objectId : scene.getOpaqueObjects()) {
            SceneObject& object = scene.getObject(objectId);
            if (object.isActive) {
                objects.push_back(object);
            }
        }

        cameraAngle = Utils::lerp(cameraAngle, cameraStartAngle + cameraAngleOffset, dt * 15.0f);
        glm::vec3 oldCameraPos = camera.position;
        camera.position = cameraOffset + getCameraPos(cameraAngle, cameraRadius, cameraHeight);
        glm::vec3 cameraDeltaPos = camera.position - oldCameraPos;
        float cameraDeltaPosDistSqr = glm::dot(cameraDeltaPos, cameraDeltaPos);

        view.update(dt);

        if (cameraDeltaPosDistSqr > .001f || view.getAnimationTimer() > .001f) {
            vulkan.resetAccumulation();
        }

        vulkan.render(objects, camera);
    }
}

void Application::cleanup() {
    vulkan.cleanup();
    resources.unload();
}

void Application::onFramebufferResized(int width, int height) {
    vulkan.framebufferResized = true;
    camera.aspect = (float)width / height;
}

void Application::keyCallback(int key, int action) {
    if (action == GLFW_PRESS) {
        bool u = moveInputs[0] == key;
        bool l = moveInputs[1] == key;
        bool d = moveInputs[2] == key;
        bool r = moveInputs[3] == key;

        if (u || r || d || l) {
            MoveDirection direction =
                u ? MoveDirection::Up :
                r ? MoveDirection::Right :
                d ? MoveDirection::Down :
                MoveDirection::Left;
            go(direction);
        }

        if (key == GLFW_KEY_D) {
            moveCameraRight();
        }
        else if (key == GLFW_KEY_A) {
            moveCameraLeft();
        }
        else if (key == GLFW_KEY_BACKSPACE) {
            undoMove();
        }
    }
}

void Application::moveCameraRight() {
    cameraAngleOffset -= 90.0f;

    int first = moveInputs[0];
    for (int i = 0; i < 3; ++i)
        moveInputs[i] = moveInputs[i + 1];
    moveInputs[3] = first;
}

void Application::moveCameraLeft() {
    cameraAngleOffset += 90.0f;

    int last = moveInputs[3];
    for (int i = 3; i > 0; --i)
        moveInputs[i] = moveInputs[i - 1];
    moveInputs[0] = last;
}

void Application::go(MoveDirection direction) {
    bool boardChanged = false;
    switch (direction) {
        case MoveDirection::Left: boardChanged = game.goLeft(); break;
        case MoveDirection::Up: boardChanged = game.goUp(); break;
        case MoveDirection::Right: boardChanged = game.goRight(); break;
        case MoveDirection::Down: boardChanged = game.goDown(); break;
    }

    if (boardChanged) {
        view.updateBoard();
    }
}

void Application::undoMove() {
    game.undoMove();
    view.updateBoardFast();
    vulkan.resetAccumulation();
}
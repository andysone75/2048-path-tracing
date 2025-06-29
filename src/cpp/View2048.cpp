#include "View2048.h"
#include <cmath>
#include <stdexcept>
#include <glm/glm.hpp>

void View2048::init(Game2048* game, Scene* scene) {
    this->game = game;
    this->scene = scene;
}

void View2048::update(float dt) {
    if (animationTimer <= 0.0f) {
        return;
    }

    animationTimer -= dt;

    if (animationTimer <= 0.0f) {
        updateBoardFast();
        if (animationCallback != nullptr)
            (*animationCallback)();
        return;
    }

    float t = 1.0f - animationTimer / ANIMATION_TIME;
    t = 1.0f - (float)std::pow(1.0f - t, 3); // out cubic easing

    for (size_t i = 0; i < animationTargets.size(); i++) {
        glm::vec3 pos = glm::mix(
            animationStartPositions[i],
            animationTargetPositions[i],
            t
        );

        const auto& target = animationTargets[i];
        int cubesCount = target.level + 1;

        for (int i = 0; i < cubesCount; i++) {
            scene->getObject(target.cubeIds[i]).position = glm::vec3(pos.x, .2f * i, pos.z);
        }
    }
}

void View2048::updateBoardFast() {
    const auto& board = game->getBoard();
    poolObjects();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) continue;
            placeObject(board[i][j] - 1, i, j);
        }
    }
}

void View2048::updateBoard(const std::function<void()>& callback) {
    updateBoard();
    animationCallback.reset();
    animationCallback = std::make_unique<std::function<void()>>(callback);
}

void View2048::updateBoard() {
    animationCallback.reset();

    animationTargets.clear();
    animationStartPositions.clear();
    animationTargetPositions.clear();

    const auto& previousBoard = game->getPreviousBoard();
    const auto& lastMoves = game->getLastMoves();
    poolObjects();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (previousBoard[i][j] == 0) continue;
            View2048_Object object = placeObject(previousBoard[i][j] - 1, i, j);

            for (auto& move : lastMoves) {
                if (move.fromX == i && move.fromY == j) {
                    glm::vec3 srcPosition = { move.fromX * 1.05f, 0.0f, move.fromY * 1.05f };
                    glm::vec3 dstPosition = { move.toX * 1.05f, 0.0f, move.toY * 1.05f };
                    animationTargets.push_back(object.cube);
                    animationStartPositions.push_back(srcPosition);
                    animationTargetPositions.push_back(dstPosition);
                    break;
                }
            }
        }
    }

    animationTimer = ANIMATION_TIME;
}

void View2048::poolObjects() {
    for (auto& object : placedObjects) {
        while (object.cube.level + 1 > objectsPools.size()) {
            std::stack<View2048_Cube> pool;
            objectsPools.push_back(pool);
        }
        
        std::stack<View2048_Cube>& pool = objectsPools[object.cube.level];
        pool.push(object.cube);

        int cubesCount = object.cube.level + 1;

        for (int i = 0; i < cubesCount; i++) {
            scene->getObject(object.cube.cubeIds[i]).isActive = false;
        }
    }

    placedObjects.clear();
}

ModelType getCubeModelByLevel(int level) {
    switch (level) {
        case 0: return ModelType::Level0;
        case 1: return ModelType::Level1;
        case 2: return ModelType::Level2;
        case 3: return ModelType::Level3;
        case 4: return ModelType::Level4;
        case 5: return ModelType::Level5;
        case 6: return ModelType::Level6;
        case 7: return ModelType::Level7;
        case 8: return ModelType::Level8;
        case 9: return ModelType::Level9;
    }

    throw std::runtime_error("Model not found");
}

glm::vec4 getCubeColorByLevel(int level) {
    glm::vec4 gray = glm::vec4{ 186, 202, 211, 255 } / 255.0f;
    glm::vec4 lightGreen = glm::vec4{ 146, 255, 124, 255 } / 255.0f;
    glm::vec4 darkGreen = glm::vec4{ 5,   181, 119, 255 } / 255.0f;
    glm::vec4 blue = glm::vec4{ 10,  226, 255, 255 } / 255.0f;
    glm::vec4 darkBlue = glm::vec4{ 42,  120, 255, 255 } / 255.0f;
    glm::vec4 purple = glm::vec4{ 202, 103, 255, 255 } / 255.0f;
    glm::vec4 pink = glm::vec4{ 158, 51,  216, 255 } / 255.0f;
    glm::vec4 red = glm::vec4{ 255, 110, 112, 255 } / 255.0f;
    glm::vec4 orange = glm::vec4{ 255, 159, 124, 255 } / 255.0f;
    glm::vec4 yellow = glm::vec4{ 255, 238, 124, 255 } / 255.0f;

    switch (level) {
        case 0: return gray;
        case 1: return lightGreen;
        case 2: return darkGreen;
        case 3: return blue;
        case 4: return darkBlue;
        case 5: return purple;
        case 6: return pink;
        case 7: return red;
        case 8: return orange;
        case 9: return yellow;
    }

    throw std::runtime_error("Model not found");
}

View2048_Object View2048::placeObject(int level, int row, int col) {
    View2048_Object object;
    bool poolHit = false;

    if (level < objectsPools.size()) {
        std::stack<View2048_Cube>& pool = objectsPools[level];

        if (!pool.empty()) {
            object.cube = pool.top();
            pool.pop();
            poolHit = true;
        }
    }

    if (!poolHit) {
        object.cube.cubeIds = new int[level + 1];

        for (int i = 0; i < level + 1; i++) {
            object.cube.cubeIds[i] = scene->createObjectOpaque(getCubeModelByLevel(i), getCubeColorByLevel(i));
        }

        object.cube.level = level;
    }

    int cubesCount = level + 1;

    for (int i = 0; i < cubesCount; i++) {
        SceneObject& sceneObjectCube = scene->getObject(object.cube.cubeIds[i]);
        sceneObjectCube.position = { row * 1.05f, .2f * i, col * 1.05f };
        sceneObjectCube.isActive = true;
    }

    object.row = row;
    object.col = col;
    placedObjects.push_back(object);
    return object;
}
#pragma once

#include "Engine.h"

struct SceneObject {
    const Model& model;
    glm::vec3 position;
    glm::vec4 color;
    bool isActive = true;

    SceneObject(const Model& _model, glm::vec3 _position, glm::vec4 _color)
        : model(_model), position(_position), color(_color) {
    };
};
#pragma once

#include "SceneObject.h"
#include "Resources.h"

#include <vector>
#include <string>
#include <memory>

using namespace std;

class Scene {
public:
    void initialize(const Resources* resources);

    int createObjectOpaque(ModelType model, glm::vec4 color, glm::vec3 position = {});

    SceneObject& getObject(int index) { return objects[index]; }
    const SceneObject& getObject(int index) const { return objects[index]; }
    const vector<int>& getOpaqueObjects() const { return opaqueObjects; }

private:
    const Resources* resources;
    vector<int> opaqueObjects;
    vector<SceneObject> objects;
};

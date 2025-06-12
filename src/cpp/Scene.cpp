#include "Scene.h"
#include <stdexcept>
#include <glm/glm.hpp>

using namespace std;

void Scene::initialize(const Resources* resources) {
    this->resources = resources;

    float shift = 1.05f;

    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {0 * shift, -0.15f, 0 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {1 * shift, -0.15f, 0 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {2 * shift, -0.15f, 0 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {3 * shift, -0.15f, 0 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {0 * shift, -0.15f, 1 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {1 * shift, -0.15f, 1 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {2 * shift, -0.15f, 1 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {3 * shift, -0.15f, 1 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {0 * shift, -0.15f, 2 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {1 * shift, -0.15f, 2 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {2 * shift, -0.15f, 2 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {3 * shift, -0.15f, 2 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {0 * shift, -0.15f, 3 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {1 * shift, -0.15f, 3 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {2 * shift, -0.15f, 3 * shift});
    createObjectOpaque(ModelType::GridCell, glm::vec4(1.0f), {3 * shift, -0.15f, 3 * shift});
}

int Scene::createObjectOpaque(ModelType model, glm::vec4 color, glm::vec3 position) {
    SceneObject object = SceneObject(resources->getModel(model), position, color);
    objects.push_back(object);
    int index = (int)objects.size() - 1;
    opaqueObjects.push_back(index);
    return index;
}

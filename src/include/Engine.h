#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#define WHITE Color{ 255, 255, 255, 255 }

struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct Mesh {
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    size_t indexCount;

	static void GenCube(std::vector<float>& vertices, std::vector<uint16_t>& indices, std::vector<float>& normals, std::vector<unsigned char>& colors, std::vector<float>& uvs, Color color = WHITE);
};

struct Model {
    std::string name;
    Mesh mesh;
	glm::mat4 transform;

	static Model Load(const Mesh& mesh, std::string name);
};

struct Camera {
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    float fovY;
    float aspect;
    float nearZ;
    float farZ;

    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 target = glm::vec3(0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float fovY = 45.0f,
        float aspect = 4.0f / 3.0f,
        float nearZ = 0.1f,
        float farZ = 100.0f
    )
        : position(position), target(target), up(up),
        fovY(fovY), aspect(aspect), nearZ(nearZ), farZ(farZ)
    {
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, target, up);
    }

    glm::mat4 getProjectionMatrix() const {
        return glm::perspective(glm::radians(fovY), aspect, nearZ, farZ);
    }

    glm::mat4 getProjectionMatrixOrtho() const {
        float halfH = fovY * 0.5f;
        float halfW = halfH * aspect;
        return glm::ortho(-halfW, halfW, -halfH, halfH, nearZ, farZ);
    }
};

struct Light : Camera {
    inline glm::vec3 getDirection() const {
        return target - position;
    }
};

class RenderPass {
public:
    virtual void initialize(GLsizei width, GLsizei height, const void* arg = nullptr) {
        this->width = width;
        this->height = height;
    };

    virtual void render(const void* arg = nullptr) const = 0;
    virtual void unload() {};

    inline const GLuint getTexture() const { return texture; }
    inline const GLsizei getWidth() const { return width; }
    inline const GLsizei getHeight() const { return height; }

protected:
    GLsizei width, height;
    GLuint texture;
    GLuint fbo;
};
#include "Engine.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void Mesh::GenCube(std::vector<float>& vertices, std::vector<uint16_t>& indices, std::vector<float>& normals, std::vector<unsigned char>& colors, std::vector<float>& uvs, Color color) {
    float hs = 0.5f;

    float vert[] = {
        -hs, -hs,  hs,  hs, -hs,  hs,  hs,  hs,  hs,
         hs,  hs,  hs, -hs,  hs,  hs, -hs, -hs,  hs,
        -hs, -hs, -hs, -hs,  hs, -hs,  hs,  hs, -hs,
         hs,  hs, -hs,  hs, -hs, -hs, -hs, -hs, -hs,
        -hs, -hs, -hs, -hs, -hs,  hs, -hs,  hs,  hs,
        -hs,  hs,  hs, -hs,  hs, -hs, -hs, -hs, -hs,
         hs, -hs,  hs,  hs, -hs, -hs,  hs,  hs, -hs,
         hs,  hs, -hs,  hs,  hs,  hs,  hs, -hs,  hs,
        -hs,  hs,  hs,  hs,  hs,  hs,  hs,  hs, -hs,
         hs,  hs, -hs, -hs,  hs, -hs, -hs,  hs,  hs,
        -hs, -hs, -hs,  hs, -hs, -hs,  hs, -hs,  hs,
         hs, -hs,  hs, -hs, -hs,  hs, -hs, -hs, -hs
    };

    vertices.resize(sizeof(vert) / sizeof(float));
    memcpy(vertices.data(), vert, sizeof(vert));

    indices.resize(36);
    for (size_t i = 0; i < 36; i++) {
        indices[i] = i;
    }

    float norm[] = {
        0, 0,  1,  0,  0,  1,  0,  0,  1,
        0, 0,  1,  0,  0,  1,  0,  0,  1,
        0, 0, -1,  0,  0, -1,  0,  0, -1,
        0, 0, -1,  0,  0, -1,  0,  0, -1,
       -1, 0,  0, -1,  0,  0, -1,  0,  0,
       -1, 0,  0, -1,  0,  0, -1,  0,  0,
        1, 0,  0,  1,  0,  0,  1,  0,  0,
        1, 0,  0,  1,  0,  0,  1,  0,  0,
        0, 1,  0,  0,  1,  0,  0,  1,  0,
        0, 1,  0,  0,  1,  0,  0,  1,  0,
        0, -1, 0,  0, -1,  0,  0, -1,  0,
        0, -1, 0,  0, -1,  0,  0, -1,  0
    };

    normals.resize(sizeof(norm) / sizeof(float));
    memcpy(normals.data(), norm, sizeof(norm));

    unsigned char col[36 * 4];
    for (int i = 0; i < 36; i++) {
        col[i * 4 + 0] = color.r;
        col[i * 4 + 1] = color.g;
        col[i * 4 + 2] = color.b;
        col[i * 4 + 3] = color.a;
    }

    colors.resize(sizeof(col) / sizeof(unsigned char));
    memcpy(colors.data(), col, sizeof(col));

    float uv[] = {
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f,
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f,
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f,
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f,
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f,
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f
    };

    uvs.resize(sizeof(uv) / sizeof(float));
    memcpy(uvs.data(), uv, sizeof(uv));
}

//Mesh GenPlane(float width, float height, int slices) {
//    int vertexCount = (slices + 1) * (slices + 1);
//    int indexCount = slices * slices * 6;
//
//    std::vector<glm::vec3> vertices(vertexCount);
//    std::vector<GLuint> indices(indexCount);
//
//    float halfWidth = width * 0.5f;
//    float halfHeight = height * 0.5f;
//    float step = width / slices;
//
//    int vertexIndex = 0;
//    for (int z = 0; z <= slices; ++z) {
//        for (int x = 0; x <= slices; ++x) {
//            float xPos = -halfWidth + x * step;
//            float zPos = -halfHeight + z * step;
//            vertices[vertexIndex++] = glm::vec3(xPos, 0.0f, zPos);
//        }
//    }
//
//    int indexIndex = 0;
//    for (int z = 0; z < slices; ++z) {
//        for (int x = 0; x < slices; ++x) {
//            int topLeft = z * (slices + 1) + x;
//            int topRight = z * (slices + 1) + (x + 1);
//            int bottomLeft = (z + 1) * (slices + 1) + x;
//            int bottomRight = (z + 1) * (slices + 1) + (x + 1);
//
//            indices[indexIndex++] = topLeft;
//            indices[indexIndex++] = bottomLeft;
//            indices[indexIndex++] = topRight;
//
//            indices[indexIndex++] = topRight;
//            indices[indexIndex++] = bottomLeft;
//            indices[indexIndex++] = bottomRight;
//        }
//    }
//
//    Mesh mesh;
//    mesh.vao = VAO;
//    mesh.vertexCount = vertexCount;
//
//    return mesh;
//}

//Mesh Mesh::GenFullscreenQuad() {
//    float quadVertices[] = {
//        // positions   // texCoords
//        -1.0f,  1.0f,  0.0f, 1.0f,
//        -1.0f, -1.0f,  0.0f, 0.0f,
//         1.0f, -1.0f,  1.0f, 0.0f,
//
//        -1.0f,  1.0f,  0.0f, 1.0f,
//         1.0f, -1.0f,  1.0f, 0.0f,
//         1.0f,  1.0f,  1.0f, 1.0f
//    };
//
//    Mesh mesh;
//    mesh.vao = quadVAO;
//    mesh.vertexCount = 6;
//    return mesh;
//}

Model Model::Load(const Mesh& mesh) {
    Model model;
    model.mesh = mesh;
    return model;
}
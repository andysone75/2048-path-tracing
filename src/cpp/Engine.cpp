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
    for (uint16_t i = 0; i < 36; i++) {
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

Model Model::Load(const Mesh& mesh, std::string name) {
    Model model;
    model.mesh = mesh;
    model.name = name;
    return model;
}
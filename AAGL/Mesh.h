#pragma once
#include <string>
#include <vector>

class Mesh {
    public:
    unsigned int vbo;
    unsigned int vao;
    size_t vertexCount;
    size_t indexCount;
    std::string name;
    Mesh(std::string name, std::vector<float> &verts, bool hasTexCoords = true);
    ~Mesh();
};
#pragma once
#include <string>
#include <vector>

class Mesh {
    public:
    bool built;
    unsigned int vbo;
    unsigned int vao;
    size_t vertexCount;
    size_t indexCount;
    std::string name;

    void build(std::vector<float> &verts, bool hasTexCoords = true);
    Mesh(std::string name, std::vector<float> &verts, bool hasTexCoords = true);
    Mesh(std::string name);
    ~Mesh();
};
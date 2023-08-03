#include <glad/glad.h>
#include "Mesh.h"
#include <iostream>

void Mesh::build(std::vector<float> &verts, bool hasTexCoords) {
    vao = 0;
    vbo = 0;
    vertexCount = verts.size();
    int valuesPerVertex = hasTexCoords ? 5 : 3;
    indexCount = vertexCount / valuesPerVertex; //5 values per index
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
    //std::cout << "Registered VBO: " << name << " (" << verts.size() << ") -> (id: " << vbo << ")" << std::endl;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //vertex
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, valuesPerVertex * sizeof(float), 0);
    //texcoords
    if(hasTexCoords) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, valuesPerVertex * sizeof(float), (void*)(3 * sizeof(float)));
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //std::cout << "Registered VAO: " << name << " (" << verts.size() << ") -> (id: " << vao << ")" << std::endl;
    built = true;
}

Mesh::Mesh(std::string name, std::vector<float> &verts, bool hasTexCoords) :name(name) {
    build(verts, hasTexCoords);
}

Mesh::Mesh(std::string name): name(name) {
    built = false;
    vbo = 0;
    vao = 0;
    vertexCount = 0;
    indexCount = 0;
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
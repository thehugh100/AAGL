#include <glad/glad.h>
#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::string name, std::vector<float> &verts) :name(name) {
    vao = 0;
    vbo = 0;
    vertexCount = verts.size();
    indexCount = vertexCount / 5; //5 values per index
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
    std::cout << "Registered VBO: " << name << " (" << verts.size() << ") -> (id: " << vbo << ")" << std::endl;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //vertex
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    //texcoords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    std::cout << "Registered VAO: " << name << " (" << verts.size() << ") -> (id: " << vao << ")" << std::endl;
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <iostream>

#include "Shape.h"
#include "Graphics.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

Shape::Shape(Graphics* g, Mesh* mesh) :mesh(mesh), graphics(g) {
    model = glm::identity<glm::mat4>();
    view = glm::identity<glm::mat4>();
    texture = nullptr;
    shader = g->defaultShader;
    col = glm::vec4(1.);
    drawType = GL_TRIANGLE_STRIP;
    if(!mesh) {
        std::cout << "Shape::Shape -> Input mesh is nullptr" << std::endl;
    }
}

void Shape::render(glm::mat4 projection) {
    glUseProgram(shader->id);
    glUniformMatrix4fv(glGetUniformLocation(shader->id, "mvp"), 1, false, glm::value_ptr(projection * view * model));
    glUniform4fv(glGetUniformLocation(shader->id, "col"), 1, glm::value_ptr(col));
    if(mesh) {
        glBindVertexArray(mesh->vao);
        if(texture) {
            glActiveTexture(0);
            glBindTexture(texture->textureType, texture->id);
        }
        glDrawArrays(drawType, 0, mesh->indexCount);
    }
}
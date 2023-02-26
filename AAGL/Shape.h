#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

class Graphics;
class Mesh;
class Texture;
class Shader;

class Shape {
    public:
    Shape(Graphics* g, Mesh* mesh);
    void render(glm::mat4 projection);

    glm::mat4 model;
    glm::mat4 view;
    glm::vec4 col;

    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Graphics* graphics;
};
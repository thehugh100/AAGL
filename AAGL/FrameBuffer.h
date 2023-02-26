#pragma once
#include <glm/vec2.hpp>

class Texture;

class FrameBuffer {
    public:
    FrameBuffer(glm::vec2 size, bool multisample = true, int samples = 4);
    void begin();
    void end();

    void loadMultisample();
    void loadSinglesample();

    unsigned int framebuffer;
    unsigned int rbo;

    bool multisample;
    int samples;

    Texture* texture;
    glm::vec2 size;
};
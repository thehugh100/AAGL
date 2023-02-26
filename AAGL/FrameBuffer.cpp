#include "FrameBuffer.h"
#include <glad/glad.h>
#include <iostream>
#include "Texture.h"

FrameBuffer::FrameBuffer(glm::vec2 size, bool multisample, int samples) :size(size), multisample(multisample), samples(samples) {
    if(multisample) {
        loadMultisample();
    } else {
        loadSinglesample();
    }
}

void FrameBuffer::loadMultisample() {
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); 

    std::cout << "Generated framebuffer -> (" << framebuffer << ")" << std::endl;

    texture = new Texture();
    texture->width = size.x;
    texture->height = size.y;
    texture->components = 3;

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture->id);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, size.x, size.y, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0); 

    std::cout << "Generated multisample framebuffer texture -> (" << texture->id << ")" << std::endl;

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture->id, 0); 

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, size.x, size.y);  

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    std::cout << "Generated renderbuffer -> (" << rbo << ")" << std::endl;

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer is not complete." << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 

    texture->textureType = GL_TEXTURE_2D_MULTISAMPLE;
}

void FrameBuffer::loadSinglesample() {
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); 

    std::cout << "Generated framebuffer -> (" << framebuffer << ")" << std::endl;

    texture = new Texture();
    texture->width = size.x;
    texture->height = size.y;
    texture->components = 3;
    
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "Generated framebuffer texture -> (" << texture->id << ")" << std::endl;

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);  

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    std::cout << "Generated renderbuffer -> (" << rbo << ")" << std::endl;

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer is not complete." << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

void FrameBuffer::begin() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); 
}

void FrameBuffer::end() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
#pragma once
#include <filesystem>

class Texture {
    public:
    Texture(std::filesystem::path path, const char* data, size_t size);
    Texture();
    void loadTexture(const char* data, size_t size);
    std::filesystem::path path;
    int width;
    int height;
    int components;
    int textureType;
    unsigned int id;
};
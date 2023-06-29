#pragma once
#include <filesystem>
#include <vector>

class Graphics;

class Texture {
    public:
    Texture(std::filesystem::path path, const char* data, size_t size);
    Texture();
    void loadTexture(const char* data, size_t size);
    void loadCubemap(Graphics* graphics, std::vector<std::filesystem::path> faces);
    std::filesystem::path path;
    int width;
    int height;
    int components;
    int textureType;
    unsigned int id;
};
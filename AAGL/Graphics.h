#pragma once
#include <vector>
#include <string>
#include "glm/ext/matrix_float4x4.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <filesystem>
#include <functional>

class Mesh;
class Shader;
class Texture;
class AssetFilesystem;

class Graphics {
    public:
    Graphics(
        std::filesystem::path assetBasePath,
        std::function<char*(std::filesystem::path, size_t&, bool)> filesystemAdapterLoad = nullptr,
        std::function<void(char* data)> filesystemAdapterFree = nullptr
    );
    Mesh* findMesh(std::string name);
    Shader* findShader(std::string name);
    Texture* findTexture(std::string name);

    Shader* lazyLoadShader(std::string path);
    Texture* lazyLoadTexture(std::string path);

    std::vector<Mesh*> meshes;
    std::vector<Shader*> shaders;
    std::vector<Texture*> textures;
    Shader* defaultShader;
    Shader* textureShader;
    AssetFilesystem* assetFilesystem;
};
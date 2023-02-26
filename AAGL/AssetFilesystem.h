#pragma once
#include <filesystem>
#include <functional>

class AssetFilesystem {
    public:
    AssetFilesystem(
        std::filesystem::path basePath, 
        std::function<char*(std::filesystem::path, size_t&, bool)> filesystemAdapterLoad = nullptr,
        std::function<void(char* data)> filesystemAdapterFree = nullptr
    );
    char* loadAssetFromFilesystem(std::filesystem::path path, size_t &size, bool nullTerminate = true);
    char* loadAsset(std::filesystem::path path, size_t &size, bool nullTerminate = true);
    void freeFile(char* data);

    std::filesystem::path basePath;
    std::function<char*(std::filesystem::path, size_t&, bool)> filesystemAdapterLoad;
    std::function<void(char* data)> filesystemAdapterFree;
};
#include "AssetFilesystem.h"
#include <iostream>
#include <fstream>

AssetFilesystem::AssetFilesystem(
    std::filesystem::path basePath, 
    std::function<char*(std::filesystem::path, size_t&, bool)> filesystemAdapterLoad,
    std::function<void(char* data)> filesystemAdapterFree
) : basePath(basePath), filesystemAdapterLoad(filesystemAdapterLoad), filesystemAdapterFree(filesystemAdapterFree) {
}

char* AssetFilesystem::loadAssetFromFilesystem(std::filesystem::path path, size_t &size, bool nullTerminate) {
    std::filesystem::path realPath = basePath/path;

    if(!std::filesystem::exists(realPath)) {
        std::cout << "Error loading asset: " << realPath << " does not exist." << std::endl;
        size = 0;
        return nullptr;
    }

    std::ifstream file(realPath, std::ios::ate | std::ios::binary);

    if(file) {
        size = file.tellg();
        char *fileData = new char[size + (int)nullTerminate]; memset(fileData, 0, size + (int)nullTerminate);
        file.seekg(std::ios::beg);
        file.read(fileData, size);
        file.close();
        return fileData;        
    }
    
    std::cout << "Could not open " << realPath << " for reading." << std::endl;
    return nullptr;
}

char* AssetFilesystem::loadAsset(std::filesystem::path path, size_t &size, bool nullTerminate) {
    char* file = nullptr;
    
    if(filesystemAdapterLoad) {
        file = filesystemAdapterLoad(path, size, nullTerminate);
    } else {
        file = loadAssetFromFilesystem(path, size, nullTerminate);
    }
    
    if(file) {
        std::cout << "Loaded Asset " << path << " successfully, " << size << " bytes." << std::endl;
    }

    return file;
}

void AssetFilesystem::freeFile(char* data) {
    if(filesystemAdapterFree) {
        filesystemAdapterFree(data);
    } else {
        delete[] data;
    }
}
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <filesystem>
#include <glad/glad.h>
#include <iostream>

#include "Graphics.h"
#include "Mesh.h"
#include "Shape.h"
#include "Shader.h"
#include "Texture.h"
#include "Shape.h"
#include "AssetFilesystem.h"

Graphics::Graphics(
	std::filesystem::path assetBasePath,
	std::function<char* (std::filesystem::path, size_t&, bool)> filesystemAdapterLoad,
	std::function<void(char* data)> filesystemAdapterFree
) {
	assetFilesystem = new AssetFilesystem(assetBasePath, filesystemAdapterLoad, filesystemAdapterFree);

	defaultShader = lazyLoadShader("shaders/generic");
	textureShader = lazyLoadShader("shaders/texture");

	// X, Y, Z, S, T
	std::vector<float> q = {
		-0.5, -0.5, 0, 0, 0,
		 0.5, -0.5, 0, 1, 0,
		-0.5,  0.5, 0, 0, 1,
		 0.5,  0.5, 0, 1, 1,
	};
	meshes.push_back(new Mesh("quadc", q));
	q = {
		0, 0, 0, 0, 0,
		0, 1, 0, 0, 1,
		1, 0, 0, 1, 0,
		1, 1, 0, 1, 1,
	};
	meshes.push_back(new Mesh("quad", q));
}

Mesh* Graphics::findMesh(std::string name) {
	for (auto& i : meshes) {
		if (i->name == name) {
			return i;
		}
	}
	return nullptr;
}

Shader* Graphics::findShader(std::string name) {
	for (auto& i : shaders) {
		if (i->name == name) {
			return i;
		}
	}
	return nullptr;
}

Shader* Graphics::lazyLoadShader(std::string path) {
	Shader* t = findShader(path);
	if (!t) {
		std::filesystem::path basePath = path;
		std::string name = basePath.filename().string();
		std::filesystem::path frag = basePath;
		std::filesystem::path vert = basePath;
		vert.concat("_v.glsl");
		frag.concat("_f.glsl");

		size_t vertSize = 0;
		char* vertData = assetFilesystem->loadAsset(vert, vertSize);

		size_t fragSize = 0;
		char* fragData = assetFilesystem->loadAsset(frag, fragSize);

		t = new Shader(name, vertData, fragData);

		assetFilesystem->freeFile(vertData);
		assetFilesystem->freeFile(fragData);

		if (t->loadedSuccessfully) {
			shaders.push_back(t);
			return t;
		}
		else {
			std::cout << "Warning: shader " << path << " did not load correctly." << std::endl;
			return nullptr;
		}
	}
	return t;
}

Texture* Graphics::findTexture(std::string name) {
	for (auto& i : textures) {
		if (i->path == name) {
			return i;
		}
	}
	return nullptr;
}

Texture* Graphics::lazyLoadTexture(std::string path) {
	Texture* t = findTexture(path);
	if (!t) {
		size_t textureSize = 0;
		char* textureData = assetFilesystem->loadAsset(path, textureSize);
		t = new Texture(path, textureData, textureSize);
		assetFilesystem->freeFile(textureData);

		textures.push_back(t);
		return t;
	}
	return t;
}
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <glad/glad.h>
#include "Texture.h"
#include "Graphics.h"
#include "AssetFilesystem.h"

Texture::Texture(std::filesystem::path path, const char* data, size_t size) :path(path) {
    textureType = GL_TEXTURE_2D;
    loadTexture(data, size);
}

Texture::Texture() {
    textureType = GL_TEXTURE_2D;
}

void Texture::loadCubemap(Graphics* graphics, std::vector<std::filesystem::path> faces)
{
	textureType = GL_TEXTURE_CUBE_MAP;
	glGenTextures(1, &id);
	glBindTexture(textureType, id);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		size_t size = 0;
		char* fsData = graphics->assetFilesystem->loadAsset(faces[i], size);
    	unsigned char* data = stbi_load_from_memory((unsigned char*)fsData, size, &width, &height, &components, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			std::cout << "Loaded Cubemap Texture" << path << " (" << width << " x " << height << " x " << components << ")" << std::endl;
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
		}

		graphics->assetFilesystem->freeFile(fsData);
		stbi_image_free(data);
	}

	//glGenerateTextureMipmap(textureID);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::loadTexture(const char* data, size_t size) {
    unsigned char* imageData = stbi_load_from_memory((unsigned char*)data, size, &width, &height, &components, 0);

    if(imageData) {
        std::cout << "Loaded Texture" << path << " (" << width << " x " << height << " x " << components << ")" << std::endl;

	    glGenTextures(1, &id);
    	glBindTexture(GL_TEXTURE_2D, id);
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, components == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if(!glIsTexture(id)) {
            std::cout << "An error occured while loading " << path << std::endl;
        } else {
            std::cout << "Registered Texture: " << path << " -> (id: " << id << ")" << std::endl;
        }
    } else {
        std::cout << "Failed to load " << path << ", invalid image file." << std::endl;
    }

    stbi_image_free(imageData);
}
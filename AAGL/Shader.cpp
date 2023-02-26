#include <iostream>
#include <fstream>
#include <filesystem>
#include <glad/glad.h>
#include "Shader.h"

Shader::Shader(std::string name, const char* vertex, const char* fragment) : name(name) {
    loadedSuccessfully = false;
    loadShaderProgram(vertex, fragment);
}

int Shader::loadShaderProgram(const char* vertex, const char* fragment) {
    vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId, 1, &vertex, NULL);
    glCompileShader(vertexId);

    if(!checkShader(vertexId, GL_COMPILE_STATUS)) {
        outputShaderLog(vertexId);
    }

    fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentId, 1, &fragment, NULL);
    glCompileShader(fragmentId);

    if(!checkShader(fragmentId, GL_COMPILE_STATUS)) {
        outputShaderLog(fragmentId);
    }

    id = glCreateProgram();
    glAttachShader(id, fragmentId);
    glAttachShader(id, vertexId);
    glLinkProgram(id);

    if(!checkProgram(id, GL_LINK_STATUS)) {
        outputProgramLog(id);
        return false;
    } else {
        std::cout << "Registered Shader: " << name << " -> (pid: " << id << ", vid: " << vertexId << ", fid: " << fragmentId << ")" << std::endl;
    }
    loadedSuccessfully = true;
    return true;
}

int Shader::checkShader(unsigned int shaderId, int param) {
    int shaderStatus = 0;
    glGetShaderiv(shaderId, param, &shaderStatus);
    return shaderStatus;
}

int Shader::checkProgram(unsigned int programId, int param) {
    int programStatus = 0;
    glGetProgramiv(programId, param, &programStatus);
    return programStatus;
}

void Shader::outputShaderLog(unsigned int shaderId) {
    char output[2048];
    int actualSize = 0;
    glGetShaderInfoLog(shaderId, 2048, &actualSize, output);
    std::cout << "Shader " << name << (checkShader(shaderId, GL_SHADER_TYPE) == GL_VERTEX_SHADER ? "_v" : "_f") << ": " << output;
}

void Shader::outputProgramLog(unsigned int programId) {
    char output[2048];
    int actualSize = 0;
    glGetProgramInfoLog(programId, 2048, &actualSize, output);
    std::cout << "Shader Program '" << name << "' linker error: " << std::endl << output;
}
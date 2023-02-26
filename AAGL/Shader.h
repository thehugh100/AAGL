#pragma once
#include <filesystem>
#include <string>

class Shader {
    public:
    Shader(std::string name, const char* vertex, const char* fragment);

    void outputShaderLog(unsigned int shaderId);
    void outputProgramLog(unsigned int programId);
    int checkShader(unsigned int shaderId, int param);
    int checkProgram(unsigned int programId, int param);
    int loadShaderProgram(const char* vertex, const char* fragment);

    unsigned int id;
    unsigned int vertexId;
    unsigned int fragmentId;
    bool loadedSuccessfully;
    std::string name;
};
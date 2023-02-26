cmake_minimum_required(VERSION 3.10)
project(AAGL)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    AAGL
    STATIC
    AAGL/Graphics.cpp
    AAGL/Mesh.cpp
    AAGL/Shader.cpp
    AAGL/Shape.cpp
    AAGL/Texture.cpp
    AAGL/AssetFilesystem.cpp
    AAGL/FrameBuffer.cpp
    AAGL/glad/glad.c
)

target_include_directories(AAGL PUBLIC include/glm include)
target_compile_definitions(AAGL PUBLIC GL_SILENCE_DEPRECATION)
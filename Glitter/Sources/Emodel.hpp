#pragma once

#include <iostream>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * This class represents a model that is loaded from an obj file, loaded into
 * the GPU, then rendered.
 *
 * To load a model, simply instanciate this class with the 'new' keyword.
 * You can 'delete' this class once you are done to free up GPU memory.
 */
class Emodel {
private:
    // Methods
    bool loadFromObj();
    void loadToGPU();

    // Attributes
    std::string filename;
    GLuint vao = 0;
    GLsizei vao_size = 0;
    GLuint vbo = 0;
    GLsizei vbo_size = 0;

    // Create an instance of the Importer class
    Assimp::Importer importer;
    const aiScene* modelData = nullptr;

public:
    // Methods
    Emodel(std::string);
    void draw(GLuint shaderProgram);
    ~Emodel(); // Deconstructor
};

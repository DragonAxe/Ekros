#pragma once

#include <iostream>
#include <vector>

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
class Emodel
{
private:
    // Methods
    bool loadFromObj();

    void loadToGPU();

    // Attributes
    std::string filename;
    GLuint vao = 0;
    GLsizei vaoSize = 0;
    GLuint vbo = 0;
    GLsizei vboSize = 0;
    GLuint indexVBO;

    // Assimp attributes
    std::vector<float>* verts;
    unsigned int mNumVerticies;
    std::vector<unsigned int>* faces;
    unsigned int mNumFaces;

public:
    // Methods
    Emodel(std::string);

    void draw();

    ~Emodel(); // Deconstructor
};

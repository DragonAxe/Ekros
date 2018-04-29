#pragma once

#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include "../apathy/path.hpp"
#include "../shaders/StaticShader.hpp"

/**
 * This class represents a model that is loaded from an obj file, loaded into
 * the GPU, then rendered.
 *
 * To load a model, simply instanciate this class with the 'new' keyword.
 * You can 'delete' this class once you are done to free up GPU memory.
 */
class Emesh
{
public:
    // Methods
    Emesh(const aiScene* scene, unsigned int index, std::string filename);

    void draw(StaticShader* shader);

    ~Emesh(); // Deconstructor

private:
    // Methods
    void assimpMeshInfo(const aiScene* scene, unsigned int index);

    bool loadFromObj(const aiScene* scene, unsigned int index);

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

    bool hasTexture = false;
    std::string texturePath;
    unsigned int texture;

    std::vector<float>* uvs;
    std::vector<unsigned int>* faces;
    unsigned int mNumFaces;
};

#pragma once

#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "Emesh.hpp"
#include "../shaders/ShaderProgram.hpp"
#include "../Camera.cpp"
#include "../shaders/StaticShader.hpp"

/**
 * This class represents a model that is loaded from an obj file, loaded into
 * the GPU, then rendered.
 *
 * To load a model, simply instanciate this class with the 'new' keyword.
 * You can 'delete' this class once you are done to free up GPU memory.
 */
class Emodel
{
public:
    // Methods
    Emodel(std::string filename, StaticShader* shader);

    Emodel(std::string filename, StaticShader* shader, glm::vec3 pos);

    void draw(Camera camera);

    ~Emodel(); // Deconstructor

private:
    // Methods
    bool loadFromObj();

    // Attributes
    std::string filename;
    std::vector<Emesh*> multiMesh;

    glm::vec3 pos;

    StaticShader* shader;

    GLfloat xyzRot[3] = {0.0, 0.0, 0.0};

};

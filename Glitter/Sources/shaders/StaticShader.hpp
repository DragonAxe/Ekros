#pragma once

#include "ShaderProgram.hpp"

#include <iostream>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/type_ptr.hpp>

class StaticShader : public ShaderProgram
{
public:
    StaticShader();

    void setEverythingMatrix(glm::mat4 matrix);

    void setHasTexture(bool hasTexture);

protected:
    void bindAttributes();

private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;

    // Shader uniform parameters
    GLint xyzrot;
    GLint projectionMatrix;
    GLint hasTextureBool;
};

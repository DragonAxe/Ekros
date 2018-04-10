#pragma once

#include "ShaderProgram.hpp"

#include <iostream>

class StaticShader : public ShaderProgram
{
public:
    StaticShader();
    void setRotation(GLfloat rot[3]);

protected:
    void bindAttributes();

private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;

    // Shader uniform parameters
    GLint xyzrot;
};

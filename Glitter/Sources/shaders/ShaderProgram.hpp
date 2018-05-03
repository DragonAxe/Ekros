#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "../apathy/path.hpp"

/**
 * An abstract shader program class.
 *
 * Contains code for loading, using, and cleaning up shader programs
 * that consist of shaders bound to a shader program.
 */
class ShaderProgram
{
public:
    ShaderProgram(std::string vertexFile, std::string fragmentFile);

    void start();

    void stop();

    ~ShaderProgram();

protected:
    virtual void bindAttributes() = 0;

    GLint getUniformLoc(std::string);

    void init();

    void bindAttribute(GLuint attribute, std::string variableName);

private:
    GLuint shaderProgramID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;

    GLuint loadShader(std::string file, GLenum shaderType);
};

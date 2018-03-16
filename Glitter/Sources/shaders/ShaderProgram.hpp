#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

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
    GLuint shaderProgramID; // Temporarily public to allow for shader inputs

protected:
    virtual void bindAttributes() = 0;
    void init();
    void bindAttribute(GLuint attribute, std::string variableName);

private:
    GLuint vertexShaderID;
    GLuint fragmentShaderID;
    GLuint loadShader(std::string file, GLenum shaderType);
};

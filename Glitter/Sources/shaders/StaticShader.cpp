#include "StaticShader.hpp"

const std::string StaticShader::VERTEX_FILE = "Shaders/vertexShader.txt";
const std::string StaticShader::FRAGMENT_FILE = "Shaders/fragmentShader.txt";

StaticShader::StaticShader()
        : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
{
    // Calls super constructor with VERTEX_FILE and FRAGMENT file
    init(); // Finish Base class construction.

    // Get uniform variables
    projectionMatrix = getUniformLoc("everythingMatrix");
}

void StaticShader::bindAttributes()
{
    bindAttribute(0, "position");
}

void StaticShader::setEverythingMatrix(glm::mat4 matrix)
{
    glUniformMatrix4fv(projectionMatrix, // Uniform location
                       1,        // Number of matricies
                       GL_FALSE, // Column major transpose
                       glm::value_ptr(matrix));  // Matrix values
}

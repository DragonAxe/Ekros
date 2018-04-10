#include "StaticShader.hpp"

const std::string StaticShader::VERTEX_FILE = "Shaders/vertexShader.txt";
const std::string StaticShader::FRAGMENT_FILE = "Shaders/fragmentShader.txt";

StaticShader::StaticShader()
        : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
{
    // Calls super constructor with VERTEX_FILE and FRAGMENT file
    init(); // Finish Base class construction.

    // Get uniform variables
    xyzrot = getUniformLoc("xyzrot");
    projectionMatrix = getUniformLoc("projectionMatrix");
}

void StaticShader::bindAttributes()
{
    bindAttribute(0, "position");
}

void StaticShader::setRotation(GLfloat rot[3])
{
    glUniform3fv(xyzrot, 1, rot);
}

void StaticShader::setProjectionMatrix(glm::mat4 matrix)
{
    glUniformMatrix4fv(projectionMatrix, // Uniform location
                       1,        // Number of matricies
                       GL_FALSE, // Column major transpose
                       glm::value_ptr(matrix));  // Matrix values
}

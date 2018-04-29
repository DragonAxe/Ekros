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
    hasTextureBool = getUniformLoc("hasTexture");
}

/**
 * Bind data segments found in the currently bound VAO to shader variables.
 */
void StaticShader::bindAttributes()
{
    bindAttribute(0, "position");
    bindAttribute(2, "uvcoord");
}

/**
 * Set the "everything" view matrix that will be used
 * by this shader in the next draw call.
 *
 * @param matrix The view (or projection) matrix for the camera/object being
 *                rendered.
 */
void StaticShader::setEverythingMatrix(glm::mat4 matrix)
{
    glUniformMatrix4fv(projectionMatrix, // Uniform location
                       1,        // Number of matricies
                       GL_FALSE, // Column major transpose
                       glm::value_ptr(matrix));  // Matrix values
}

/**
 * Set the hasTexture flag for this shader. If true, this shader will use
 * the currently bound texture for each fragment pixel, otherwise a default
 * pattern will be applied to the object.
 *
 * @param hasTexture The flag
 */
void StaticShader::setHasTexture(bool hasTexture)
{
    glUniform1i(hasTextureBool, // Uniform location
                hasTexture);     // Value
}

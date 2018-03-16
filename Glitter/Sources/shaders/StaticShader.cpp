#include "StaticShader.hpp"

const std::string StaticShader::VERTEX_FILE = "Shaders/vertexShader.txt";
const std::string StaticShader::FRAGMENT_FILE = "Shaders/fragmentShader.txt";

StaticShader::StaticShader()
        : ShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
{
    // Calls super constructor with VERTEX_FILE and FRAGMENT file
    init(); // Finish Base class construction.
}

void StaticShader::bindAttributes()
{
    bindAttribute(0, "position");
}

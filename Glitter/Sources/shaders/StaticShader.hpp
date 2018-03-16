#pragma once

#include "ShaderProgram.hpp"

#include <iostream>

class StaticShader : public ShaderProgram
{
public:
    StaticShader();

protected:
    void bindAttributes();

private:
    static const std::string VERTEX_FILE;
    static const std::string FRAGMENT_FILE;
};

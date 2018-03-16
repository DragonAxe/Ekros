#include "ShaderProgram.hpp"

/**
 * WARN: You must call init() in the constructor of any child
 *       ShaderProgram class (subclass).
 *
 * Creates a new shader program from the given vertex and fragment files.
 *
 * @param vertexFile The path to the vertex shader file to load.
 * @param fragmentFile The path to the fragment shader file to load.
 */
ShaderProgram::ShaderProgram(std::string vertexFile, std::string fragmentFile)
{
    // Load shaders from file into the GPU
    vertexShaderID = loadShader(vertexFile, GL_VERTEX_SHADER);
    fragmentShaderID = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
}

/**
 * WARN: You must call this method in the constructor of any child
 *       ShaderProgram class (subclass).
 *
 * This method finishes setting up the ShaderProgram class.
 * This code couldn't be included in the constructor because of the call
 * to the virtual method 'bindAttributes()' which is not yet defined when
 * this Base class is constructed by the subclass. (This is a problem with
 * how c++ handles the order of class constructor calls on instantiation.)
 */
void ShaderProgram::init()
{
    // Link shaders to a shader program
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);

    // Bind attributes from a VAO to this shader program
    bindAttributes(); // This method must be implemented by the concrete class

    glLinkProgram(shaderProgramID);

    GLint success;
    char infoLog[512];

    // Check for linking errors
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);
        std::ostringstream errorStr;
        errorStr << "Shader program linking failed:" << infoLog << std::endl;
        throw std::runtime_error(errorStr.str());
    }
}


/**
 * Start using this shader program.
 */
void ShaderProgram::start()
{
    glUseProgram(shaderProgramID);
}

/**
 * Stop using this shader program.
 */
void ShaderProgram::stop()
{
    glUseProgram(0);
}

/**
 * A helper method for subclasses to call glBindAttribLocation
 * using this program's ID.
 *
 * @param attribute The attribute ID (see OpenGL docs for glBindAtrribLocation.
 * @param variableName The name of the variable in the shader code.
 */
void
ShaderProgram::bindAttribute(GLuint attribute, std::string variableName)
{
    glBindAttribLocation(shaderProgramID, attribute, variableName.c_str());
}

/**
 * Loads the specified shader from a file into the graphics card.
 *
 * @throws std::runtime_error if shader failed to compile.
 * @param file The shader file path to load.
 * @param shaderType One of GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
 * @return The id of the loaded shader.
 */
GLuint
ShaderProgram::loadShader(std::string file, GLenum shaderType)
{
    // Load the entire file into a char array
    std::ifstream in(file);
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    const char* shaderSrc = contents.c_str();

    // Build and compile our shader program
    int success;
    char infoLog[512];

    // Create and compile our shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    // Check for shader compile errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::ostringstream errorStr;
        errorStr << "Shader compilation failed:" << infoLog << std::endl;
        throw std::runtime_error(errorStr.str());
    }

    return shader;
}

/**
 * Detaches shaders from their program and deletes them from GPU memory.
 * Also deletes the shader program from memory.
 */
ShaderProgram::~ShaderProgram()
{
    stop();
    glDetachShader(shaderProgramID, vertexShaderID);
    glDetachShader(shaderProgramID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    glDeleteProgram(shaderProgramID);
}

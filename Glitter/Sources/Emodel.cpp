#include "Emodel.hpp"

/**
 * Creates and loads an object from the specified .obj
 * file and loads it into the GPU, ready for rendering.
 * @param filename The path to a .obj file.
 */
Emodel::Emodel(std::string filename)
{
    this->filename = filename;

    loadFromObj();
    loadToGPU();
}

/**
 * Loads mesh data from a .obj file using the Assimp (Asset Importer) library.
 */
bool Emodel::loadFromObj()
{
    // Using an instance of the Importer class,
    // have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    // Note: The loaded data is managed by the importer and will be freed
    // once the importer is deconstructed.
    const aiScene* scene = this->importer.ReadFile( this->filename,
                                              aiProcess_CalcTangentSpace       |
                                              aiProcess_Triangulate            |
                                              aiProcess_JoinIdenticalVertices  |
                                              aiProcess_SortByPType);

    // If the import failed, report it
    if( !scene) {
        std::cout << "ERROR:ASSIMP:MODEL_IMPORT:\n" << this->importer.GetErrorString() << std::endl;
        return false;
    }
    // Now we can access the file's contents.
    return true;
}

/**
 * Loads the Assimp imported mesh data into the GPU.
 */
void Emodel::loadToGPU()
{
    // Set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    // Define the triforce triangle we want to render
    float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left // left triangle
            0.0f, -0.5f, 0.0f, // right
            -0.25f,  0.0f, 0.0f,  // top
            -0.25f, 0.0f, 0.0f, // left // top triangle
            0.25f, 0.0f, 0.0f, // right
            0.0f,  0.5f, 0.0f,  // top
            0.5f, -0.5f, 0.0f, // left // right triangle
            0.0f, -0.5f, 0.0f, // right
            0.25f,  0.0f, 0.0f  // top
    };

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).

    // Generate a Vertex Array Object, then bind it for editing.
    this->vao_size = 1;
    glGenVertexArrays(this->vao_size, &this->vao);

    glBindVertexArray(this->vao);
    {
        // Generate a Vertex Buffered Object, then bind it for editing
        this->vbo_size = 1;
        glGenBuffers(this->vbo_size, &this->vbo);

        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        {
            // Load the vertices into the Vertex Buffered Object
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // Set the attributes of the loaded buffer data
            const GLuint index = 0;
            const GLint size = 3;
            const GLenum type = GL_FLOAT;
            const GLboolean normalized = GL_FALSE;
            const GLsizei stride = 3 * sizeof(float);
            const GLvoid * pointer = (void*)0;
            glVertexAttribPointer(index, size, type, normalized, stride, pointer);

            // Enable the attributes we just set for this Vertex Buffered Object
            // ???? I think this is what this is doing, need to ask Amy ????
            glEnableVertexAttribArray(0);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);
}

/**
 * Render's this object onto the screen.
 *
 * Note: If nothing is rendering, double check that 'loadFromObj' and 'loadToGPU'
 * methods have been called before calling draw.
 */
void Emodel::draw(GLuint shaderProgram)
{
    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(this->vao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 9);
    // glBindVertexArray(0); // no need to unbind it every time
}

/**
 * Destroy this object and free the GPU memory.
 */
Emodel::~Emodel()
{
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(this->vao_size, &this->vao);
    glDeleteBuffers(this->vbo_size, &this->vbo);

    // Note: Mesh data loaded from the .obj file will be freed
    // when the importer is deconstructed, which is also the
    // same as when this class is deconstructed.
}


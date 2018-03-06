#include "Emodel.hpp"

/*
 * Useful documentation:
 * https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object
 * https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices
 */

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
    // Create an instance of the Importer class
    Assimp::Importer importer;

    // Using an instance of the Importer class,
    // have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    // Note: The loaded data is managed by the importer and will be freed
    // once the importer is deconstructed.
    const aiScene* model_data = importer.ReadFile( this->filename,
                                              aiProcess_CalcTangentSpace       |
                                              aiProcess_Triangulate            |
                                              aiProcess_JoinIdenticalVertices  |
                                              aiProcess_SortByPType);

    // If the import failed, report it
    if (!model_data) {
        std::cout << "ERROR:ASSIMP:MODEL_IMPORT:\n" << importer.GetErrorString() << std::endl;
        return false;
    }
    // Now we can access the file's contents.


    // Thanks to: https://sites.google.com/site/gsucomputergraphics/educational/load_3d_model
    // on how to load the Assimp data into a format OpenGL can understand.


    // **** Transfer vertex data to RAM ****
    numVertices = model_data->mMeshes[0]->mNumVertices;
    vertices = (float *) malloc(sizeof(float) * numVertices);
    memcpy(vertices, model_data->mMeshes[0]->mVertices, sizeof(float) * numVertices);
    for (unsigned int i = 0; i < numVertices; i++) {
        std::cout << vertices[i] << ",";
    }
    std::cout << std::endl;


    // **** Transfer face data to RAM ****
    // For each mesh in the aiScene object
    for (unsigned int n = 0; n < model_data->mNumMeshes; ++n) {
        std::cout << "Mesh id: " << n << std::endl;

        // Get the current aiMesh object.
        const aiMesh *mesh = model_data->mMeshes[n];

        // Create a one dimensional array to store face indices
        GLsizei numFaces;
        unsigned int *faceArray = nullptr;
        faceArray = (unsigned int *) malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
        unsigned int faceIndex = 0;

        // Copy face indices from aiMesh to faceArray.
        // In an aiMesh object, face indices are stored in two layers. There is an array of aiFace, and then each aiFace stores 3 indices.
        // Must copy face indices to a one dimensional array so that it can be used in OpenGL functions.
        for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
            const aiFace *face = &mesh->mFaces[t];  // Go through the list of aiFace

            // For each aiFace, copy its indices to faceArray.
            memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
            faceIndex += 3;
        }
        numFaces = mesh->mNumFaces; // Record the number of faces.

        for (unsigned int i = 0; i < numFaces; i += 3) {
            std::cout << faceArray[i] << "," << faceArray[i+1] << "," << faceArray[i+1] << std::endl;
        }
    }

    return true;
}

/**
 * Loads the Assimp imported mesh data into the GPU.
 */
void Emodel::loadToGPU()
{
    // Set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    // Thanks to: https://www.khronos.org/opengl/wiki_opengl/index.php
    // for being a great documentation reference for OpenGL.

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).

    // Generate a Vertex Array Object, then bind it for editing.
    this->vaoSize = 1;
    glGenVertexArrays(this->vaoSize, &this->vao);

    glBindVertexArray(this->vao);
    {
        // Generate a Vertex Buffered Object, then bind it for editing
        this->vboSize = 1;
        glGenBuffers(this->vboSize, &this->vbo);

        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        {
            // Load the vertices into the Vertex Buffered Object
            glBufferData(GL_ARRAY_BUFFER, numVertices, this->vertices, GL_STATIC_DRAW);

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
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    // glBindVertexArray(0); // no need to unbind it every time
}

/**
 * Destroy this object and free the GPU memory.
 */
Emodel::~Emodel()
{
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(this->vaoSize, &this->vao);
    glDeleteBuffers(this->vboSize, &this->vbo);
}


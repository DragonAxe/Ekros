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


    aiMesh* mesh = model_data->mMeshes[0];

    verts = new std::vector<float>();
//    mNumVerticies = mesh->mNumVertices;
//    for (unsigned int i = 0; i < mNumVerticies; i++) {
//        verts->push_back(mesh->mVertices[i].x);
//        verts->push_back(mesh->mVertices[i+1].y);
//        verts->push_back(mesh->mVertices[i+2].z);
//    }
    mNumVerticies = 3;
    verts->push_back(-0.5);
    verts->push_back(0.5);
    verts->push_back(0);
    verts->push_back(-0.5);
    verts->push_back(-0.5);
    verts->push_back(0);
    verts->push_back(0.5);
    verts->push_back(0.5);
    verts->push_back(0);
    std::cout << "Verts: ";
    std::cout << "size=" << verts->size() << " vNum=" << mNumVerticies << " " << std::endl;
    for (unsigned int i = 0; i < verts->size(); i++) {
        std::cout << verts->data()[i] << ",";
    }
    std::cout << std::endl;



    std::vector<float> colors = std::vector<float>();


    faces = new std::vector<unsigned int>();
//    mNumFaces = mesh->mNumFaces;
//    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
//        aiFace* face = &(mesh->mFaces[i]);
//        faces->push_back(face->mIndices[0]);
//        faces->push_back(face->mIndices[1]);
//        faces->push_back(face->mIndices[2]);
//    }
    mNumFaces = 1;
    faces->push_back(0);
    faces->push_back(1);
    faces->push_back(2);
    std::cout << "Faces: ";
    std::cout << "size=" << faces->size() << " vNum=" << mNumFaces << " " << std::endl;
    for (unsigned int i = 0; i < faces->size(); i++) {
        std::cout << faces->data()[i] << ",";
    }
    std::cout << std::endl;




    return true;
}

/*
 * We need
 * vert list
 * color list
 * normal list
 * index list
 *
 * 
 * glBufferSubData(GL_ARRAY_BUFFER, 0, size, vert);
 * glBufferSubData(GL_ARRAY_BUFFER, size, size, color);
 */

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
            glBufferData(GL_ARRAY_BUFFER, verts->size()*sizeof(float), verts->data(), GL_STATIC_DRAW);

            // Set the attributes of the loaded buffer data
            const GLuint index = 0;
            const GLint size = 3;
            const GLenum type = GL_FLOAT;
            const GLboolean normalized = GL_FALSE;
            const GLsizei stride = 0; //3 * sizeof(float);
            const GLvoid * pointer = (void*)0;
            glVertexAttribPointer(index, size, type, normalized, stride, pointer);

            // Enable the attributes we just set for this Vertex Buffered Object
            // ???? I think this is what this is doing, need to ask Amy ????
            glEnableVertexAttribArray(0);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &indexVBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        {
            glBufferData(GL_ARRAY_BUFFER, faces->size() * sizeof(unsigned int), faces->data(), GL_STATIC_DRAW);


        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
    glDrawArrays(GL_TRIANGLES, 0, mNumVerticies);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
//    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);
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

    delete verts;
    delete faces;
}


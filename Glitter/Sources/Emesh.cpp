//
// Created by guy on 3/27/18.
//

#include "Emesh.hpp"


/**
 * Creates and loads an object from the specified .obj
 * file and loads it into the GPU, ready for rendering.
 * @param filename The path to a .obj file.
 */
Emesh::Emesh(const aiScene* scene, unsigned int index)
{
    loadFromObj(scene, index);
    loadToGPU();
}

/**
 * Loads mesh data from a .obj file using the Assimp (Asset Importer) library.
 */
bool Emesh::loadFromObj(const aiScene* scene, unsigned int index)
{

    verts = new std::vector<float>();
    faces = new std::vector<unsigned int>();
    mNumVerticies = 0;
    mNumFaces = 0;

    //printf("Mesh count=%d\n", model_data->mNumMeshes);

    // Assimp separates each group of faces that use the same material
    // into different meshes. This loop combines them into a single
    // flat array for vertices and indices (faces).

        const aiMesh* mesh = scene->mMeshes[index];

        // Add the x,y,z of each vertex to a flat array
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            verts->push_back(mesh->mVertices[i].x);
            verts->push_back(mesh->mVertices[i].y);
            verts->push_back(mesh->mVertices[i].z);
        }

        // Add the indices of each face (made up of 3 vertices because it's a triangle)) to a flat array
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace* face = &(mesh->mFaces[i]);
            // Each face index restarts at zero for each Assimp mesh,
            // so we must add an offset of the number of vertices that
            // have been added so far to our flat array.
            faces->push_back(face->mIndices[0]);
            faces->push_back(face->mIndices[1]);
            faces->push_back(face->mIndices[2]);
        }

    // Print out vertex statistics
    /*std::cout << "Verts: ";
    std::cout << "size=" << verts->size() << " fNum=" << mNumVerticies << " " << std::endl;
    for (unsigned int i = 0; i < verts->size(); i++) {
        std::cout << verts->data()[i] << ",";
    }
    std::cout << std::endl;

    // Print out face statistics
    std::cout << "Faces: ";
    std::cout << "size=" << faces->size() << " vNum=" << mNumFaces << " " << std::endl;
    for (unsigned int i = 0; i < faces->size(); i++) {
        std::cout << faces->data()[i] << ",";
    }
    std::cout << std::endl;
    */

    return true; // Success
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
void Emesh::loadToGPU()
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
        glGenBuffers(1, &indexVBO);

        // We can bind the buffers all at once
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);

        // Load the vertices into the Vertex Buffered Object
        glBufferData(GL_ARRAY_BUFFER, (verts->size()) * sizeof(float),
                     verts->data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     faces->size() * sizeof(unsigned int), faces->data(),
                     GL_STATIC_DRAW);

        // Set the attributes of the loaded buffer data
        glVertexAttribPointer(0,          // index in VAO to store this buffer
                              3,          // size, number of values per vertex
                              GL_FLOAT,   // type
                              GL_FALSE,   // normalized
                              0,          // stride
                              (void*) 0); // pointer to a value in the buffer

        // Enable the attributes we just set for this Vertex Buffered Object
        glEnableVertexAttribArray(
                0); // 0, the index we set above in glVertexAttribPointer
    }
    glBindVertexArray(0);
}

/**
 * Render's this object onto the screen.
 *
 * Note: If nothing is rendering, double check that 'loadFromObj' and 'loadToGPU'
 * methods have been called before calling draw.
 */
void Emesh::draw()
{
    glBindVertexArray(this->vao);

    // Render our model using indices instead of raw vertices
    glDrawElements(GL_TRIANGLES, (GLsizei) faces->size(), GL_UNSIGNED_INT, (void*) 0);

    // glBindVertexArray(0); // no need to unbind it every time
}

/**
 * Destroy this object and free the GPU memory.
 */
Emesh::~Emesh()
{
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(this->vaoSize, &this->vao);
    glDeleteBuffers(this->vboSize, &this->vbo);

    delete verts;
    delete faces;
}
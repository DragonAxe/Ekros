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
    const aiScene* model_data = importer.ReadFile(this->filename,
                                                  aiProcess_CalcTangentSpace |
                                                  aiProcess_Triangulate |
                                                  aiProcess_JoinIdenticalVertices |
                                                  aiProcess_SortByPType);

    // If the import failed, report it
    if (!model_data) {
        std::cout << "ERROR:ASSIMP:MODEL_IMPORT:\n" << importer.GetErrorString()
                  << std::endl;
        return false;
    }

    //printf("Mesh count=%d\n", model_data->mNumMeshes);

    //Create a vector of Emesh objects
    for (unsigned int mesh_index = 0;
         mesh_index < model_data->mNumMeshes; mesh_index++) {

        Emesh* newMesh = new Emesh(model_data, mesh_index);

        multiMesh.push_back(newMesh);
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

/**
 * Render's this object onto the screen.
 *
 * Note: If nothing is rendering, double check that 'loadFromObj' and 'loadToGPU'
 * methods have been called before calling draw.
 */
void Emodel::draw()
{
    // draw our first triangle
    for (unsigned int mIndex = 0;
         mIndex < multiMesh.size(); mIndex++) {

        multiMesh[mIndex]->draw();
    }
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


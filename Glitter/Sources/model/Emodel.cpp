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
Emodel::Emodel(std::string filename, StaticShader* shader)
        : Emodel(filename, shader, glm::vec3(0, 0, 0))
{}

/**
 * Creates and loads an object from the specified .obj
 * file and loads it into the GPU, ready for rendering.
 * @param filename The path to a .obj file.
 */
Emodel::Emodel(std::string filename, StaticShader* shader, glm::vec3 pos)
{
    this->filename = filename;
    this->pos = pos;
    this->shader = shader;

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
    const aiScene* model_data = importer.ReadFile(
            this->filename,
            aiProcess_CalcTangentSpace
            | aiProcess_Triangulate
            | aiProcess_JoinIdenticalVertices
            | aiProcess_SortByPType
            | aiProcess_FlipUVs);

    // If the import failed, report it
    if (!model_data) {
        std::cout << "ERROR:ASSIMP:MODEL_IMPORT:\n" << importer.GetErrorString()
                  << std::endl;
        return false;
    }

    //Create a vector of Emesh objects
    for (unsigned int mesh_index = 0;
         mesh_index < model_data->mNumMeshes; mesh_index++) {

        int materialIndex = model_data->mMeshes[mesh_index]->mMaterialIndex;
        aiMaterial* mat = model_data->mMaterials[materialIndex];
        aiString name;
        mat->Get(AI_MATKEY_NAME, name);
        std::string hiddenName = "hidden_material";

        if (hiddenName != name.C_Str()) {
            Emesh* newMesh = new Emesh(model_data, mesh_index, this->filename);
            multiMesh.push_back(newMesh);
        }

    }

    return true; // Success
}

/**
 * Render's this object onto the screen.
 *
 * Note: If nothing is rendering, double check that 'loadFromObj' and 'loadToGPU'
 * methods have been called before calling draw.
 */
void Emodel::draw(Camera camera)
{
    shader->start();
    {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos);
        //glm::mat4 rm = glm::rotate(glm::mat4(1.0f), xyzRot[0], glm::vec3(0,1,0));
        xyzRot[0] += 0.01f;

        glm::mat4 em = camera.getCameraMatrix() * modelMatrix;
        shader->setEverythingMatrix(em);

        // draw our first triangle
        for (unsigned int mIndex = 0;
             mIndex < multiMesh.size(); mIndex++) {

            multiMesh[mIndex]->draw(shader);
        }
    }
    shader->stop();
}

/**
 * Destroy this object and free the GPU memory.
 */
Emodel::~Emodel()
{
}


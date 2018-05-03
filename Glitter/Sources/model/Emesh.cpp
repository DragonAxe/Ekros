#include "Emesh.hpp"


/**
 * Creates and loads an object from the specified .obj
 * file and loads it into the GPU, ready for rendering.
 *
 * @param filename The path to a .obj file.
 */
Emesh::Emesh(const aiScene* scene, unsigned int index, std::string filename)
{
    this->filename = filename;
    //assimpMeshInfo(scene, index);
    loadFromObj(scene, index);
    loadToGPU();
}

/**
 * Debug feature:
 * Print all the information from assimp pertaining to the
 * UV texture coordinates and texture path.
 *
 * @param scene The assimp scene of the loaded .obj file.
 * @param index The mesh index of the current mesh to be loaded by this object.
 */
void Emesh::assimpMeshInfo(const aiScene* scene, unsigned int index)
{
    if (this->filename == "MuseumModels/objs/CubeTextured.obj") {
        printf(">>>>>> Filename: '%s'\n", this->filename.c_str());
        printf(">>> Mesh name: '%s'\n", scene->mMeshes[index]->mName.C_Str());
        printf("scene->mNumMeshes: '%d'\n", scene->mNumMeshes);
        printf("scene->mNumTextures: '%d'\n", scene->mNumTextures);
        printf("scene->mNumMaterials: '%d'\n", scene->mNumMaterials);


        int materialIndex = scene->mMeshes[index]->mMaterialIndex;
        printf("scene->mMeshes[%d]->mMaterialIndex: '%d'\n", index,
               materialIndex);
        printf("scene->mMeshes[%d]->mNumVertices: '%d'\n", index,
               scene->mMeshes[index]->mNumVertices);

        aiMaterial* mat = scene->mMaterials[materialIndex];
        unsigned int texCount = mat->GetTextureCount(aiTextureType_DIFFUSE);
        printf("scene->mMaterials[%d]->GetTextureCount(Defuse): '%d'\n",
               materialIndex, texCount);

        for (unsigned int texIndex = 0; texIndex < texCount; texIndex++) {
            aiString texPath = aiString();
            aiTextureMapping texMapping;
            unsigned int uvIndex;
            mat->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath,
                            &texMapping, &uvIndex);

            printf("scene->mMaterials[%d]->GetTexture(Defuse, %d)['texPath']: '%s'\n",
                   materialIndex, texIndex, texPath.C_Str());
            printf("scene->mMaterials[%d]->GetTexture(Defuse, %d)['texMapping']: '%d'\n",
                   materialIndex, texIndex, texMapping);
            printf("scene->mMaterials[%d]->GetTexture(Defuse, %d)['uvIndex']: '%d'\n",
                   materialIndex, texIndex, uvIndex);

            printf("scene->mMeshes[%d]->HasTextureCoords(%d): '%d'\n", index,
                   uvIndex, scene->mMeshes[index]->HasTextureCoords(uvIndex));
            printf("scene->mMeshes[%d]->GetNumUVChannels(): '%d'\n", index,
                   scene->mMeshes[index]->GetNumUVChannels());
        }

        for (unsigned int texCoordIndex = 0;
             texCoordIndex < AI_MAX_NUMBER_OF_TEXTURECOORDS; texCoordIndex++) {
            if (scene->mMeshes[index]->mTextureCoords[texCoordIndex] !=
                nullptr) {
                for (unsigned int vertIndex = 0;
                     vertIndex <
                     scene->mMeshes[index]->mNumVertices; vertIndex++) {
                    printf("scene->mMeshes[%d]->mTextureCoords[%d][%d]: x=%f, y=%f\n",
                           index, texCoordIndex, vertIndex,
                           scene->mMeshes[index]->mTextureCoords[texCoordIndex][vertIndex].x,
                           scene->mMeshes[index]->mTextureCoords[texCoordIndex][vertIndex].y);
                }
            }
        }
        printf("\n");
    }
}

/**
 * Loads mesh data from a .obj file using the Assimp (Asset Importer) library
 * so it can be later loaded into the GPU and rendered.
 *
 * @param scene The assimp scene of the loaded .obj file.
 * @param index The mesh index of the current mesh to be loaded by this object.
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
    int materialIndex = mesh->mMaterialIndex;
    aiMaterial* mat = scene->mMaterials[materialIndex];
    unsigned int texCount = mat->GetTextureCount(aiTextureType_DIFFUSE);
    // Variables used if textures exists
    //bool hasTexture = false; (Instance variable)
    aiVector3D* uvTexArray;
    //aiString texPath; (Instance variable)

    // Get texture file path relative to obj.
    if (texCount == 1) {
        unsigned int texIndex = 0;
        aiString texPath = aiString();
        aiTextureMapping texMapping;
        unsigned int uvIndex = 0; // Must be initialized,
        // I'm not sure why keeping in uninitialized works in assimpMeshInfo(),
        // but not here. The GetTexture() call is supposed to set it to a
        // reasonable value, but it doesn't.
        mat->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath,
                        &texMapping, &uvIndex);

        // Generate an absolute path of the texture to be loaded
        texturePath = apathy::Path(
                this->filename.c_str()).up().directory().relative(
                texPath.C_Str()).absolute().string();

        //printf(">>>>>> Filename: '%s'\n", this->filename.c_str());
        //printf("scene->mMaterials[%d]->GetTexture(Defuse, %d)['texPath']: '%s'\n",
        //       materialIndex, texIndex, texPath.C_Str());
        //printf("scene->mMaterials[%d]->GetTexture(Defuse, %d)['texMapping']: '%d'\n",
        //       materialIndex, texIndex, texMapping);
        //printf("scene->mMaterials[%d]->GetTexture(Defuse, %d)['uvIndex']: '%d'\n",
        //       materialIndex, texIndex, uvIndex);
        //
        //printf("scene->mMeshes[%d]->HasTextureCoords(%d): '%d'\n", index,
        //       uvIndex, scene->mMeshes[index]->HasTextureCoords(uvIndex));
        //printf("scene->mMeshes[%d]->GetNumUVChannels(): '%d'\n", index,
        //       scene->mMeshes[index]->GetNumUVChannels());

        hasTexture = true;

        uvTexArray = mesh->mTextureCoords[uvIndex];

        // The following commented out for loop would retrieve
        // every UV texture coordinate for this mesh.
        //for (unsigned int vertIndex = 0;
        //     vertIndex < mesh->mNumVertices; vertIndex++) {
        //    float texX = uvTexArray[vertIndex].x;
        //    float texY = uvTexArray[vertIndex].y;
        //}

    } else if (texCount > 1) {
        fprintf(stderr,
                "Warn: Unhandled: More than one texture detected while loading obj file: %s\n",
                this->filename.c_str());
    }

    // Add the x,y,z of each vertex to a flat array
    // If this mesh has a texture, append the s,t UV texture coordinates
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        verts->push_back(mesh->mVertices[i].x);
        verts->push_back(mesh->mVertices[i].y);
        verts->push_back(mesh->mVertices[i].z);

        if (hasTexture) {
            verts->push_back(uvTexArray[i].x);
            verts->push_back(uvTexArray[i].y);
        } else {
            verts->push_back(0);
            verts->push_back(0);
        }
    }


    // Add the indices of each face (made up of 3 vertices because it's a
    // triangle)) to a flat array
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace* face = &(mesh->mFaces[i]);
        // Each face index restarts at zero for each Assimp mesh,
        // so we must add an offset of the number of vertices that
        // have been added so far to our flat array.
        faces->push_back(face->mIndices[0]);
        faces->push_back(face->mIndices[1]);
        faces->push_back(face->mIndices[2]);
    }

    // Load the texture from file into the GPU
    if (hasTexture) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height,
                                        &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            fprintf(stderr, "Failed to load texture: '%s'\n",
                    texturePath.c_str());
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    return true; // Success
}

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
        // Set vertex data attributes
        glVertexAttribPointer(0,          // index in VAO to store this buffer
                              3,          // size, number of values per vertex
                              GL_FLOAT,   // type
                              GL_FALSE,   // normalized
                              5 * sizeof(float),   // stride
                              (void*) 0); // pointer to a value in the buffer
        // Set UV tex coord attributes
        glVertexAttribPointer(2,          // index in VAO to store this buffer
                              2,          // size, number of values per vertex
                              GL_FLOAT,   // type
                              GL_FALSE,   // normalized
                              5 * sizeof(float),   // stride
                              (void*) (3 *
                                       sizeof(float))); // pointer to a value in the buffer


        // Enable the attributes we just set for this Vertex Buffered Object
        glEnableVertexAttribArray(
                0); // 0, the index we set above in glVertexAttribPointer
        glEnableVertexAttribArray(
                2); // 0, the index we set above in glVertexAttribPointer
    }
    glBindVertexArray(0);
}

/**
 * Render's this object onto the screen.
 *
 * Note: If nothing is rendering, double check that 'loadFromObj' and 'loadToGPU'
 * methods have been called before calling draw.
 */
void Emesh::draw(StaticShader* shader)
{
    // Set the correct texture, if any
    shader->setHasTexture(hasTexture);
    if (hasTexture) {
        glBindTexture(GL_TEXTURE_2D, texture);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Render our model using indices instead of raw vertices
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) faces->size(), GL_UNSIGNED_INT,
                   (void*) 0);

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

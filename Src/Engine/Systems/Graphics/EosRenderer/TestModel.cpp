#include "TestModel.h"

namespace Test
{
    // constructor, expects a filepath to a 3D model.

    Model::Model() :gammaCorrection(false)
    {
    }

    Model::Model(std::string const& path, bool gamma) : gammaCorrection(gamma)
    {
        UNREFERENCED_PARAMETER(path);
        std::string cpath = "cube/cube.obj";
        loadModel(cpath);
    }

    void Model::Init(std::string const& path)
    {
        std::string cpath = path;
        //unsigned int index = MeshLoader::LoadOwnFormat(cpath.c_str());
        loadModel(cpath);
        //UNREFERENCED_PARAMETER(index);
    }

    // draws the model, and thus all its meshes

    void Model::Draw(Shader& shader)
    {
        shader.Bind();
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
        glUseProgram(0);
    }

    void Model::DrawShadow(Shader& shader, unsigned int _depthMap)
    {
        shader.Bind();
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].DrawShadow(shader,_depthMap);
        glUseProgram(0);
    }

    void Model::Clear()
    {
        textures_loaded.clear();
        meshes.clear();
    }

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.

    void Model::loadModel(std::string const& path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::string msg{ "ERROR::ASSIMP:: " };
            msg += importer.GetErrorString();
            PE_CORE_ERROR(msg.c_str());
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).

    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }
    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                              // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].z;
            vector.z = -mesh->mVertices[i].y;
            vertex.Position = vector;
            // normals
            //if (mesh->HasNormals())
            //{
            //    vector.x = mesh->mNormals[i].x;
            //    vector.y = mesh->mNormals[i].y;
            //    vector.z = mesh->mNormals[i].z;
            //    vertex.Normal = vector;
            //}
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        for (unsigned int i = 0; i < indices.size(); i += 3)
        {
            glm::vec3 A = vertices[indices[i]].Position;
            glm::vec3 B = vertices[indices[i + 1]].Position;
            glm::vec3 C = vertices[indices[i + 2]].Position;

            glm::vec3 nA = C - B;
            glm::vec3 nB = A - B;
            glm::vec3 cNormal = glm::normalize(glm::cross(nA, nB));

            vertices[indices[i]].Normal += cNormal;
            vertices[indices[i + 1]].Normal += cNormal;
            vertices[indices[i + 2]].Normal += cNormal;
        }

        for (unsigned int i = 0; i < indices.size(); i++)
        {
            vertices[i].Normal = glm::normalize(vertices[i].Normal);
        }


        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        aiString texture_file;
        material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file);
        const aiTexture* texture = scene->GetEmbeddedTexture(texture_file.C_Str());
        // Checks if there is a embeeded attached to the mesh
        if (texture)
        {
            std::vector<Texture> embedTextures = loadEmbeddedTextures(texture);
            textures.insert(textures.end(), embedTextures.begin(), embedTextures.end());
        }
        else
        {
            //std::cout << "material: " << material->GetName().C_Str() << std::endl;

            // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
            // Same applies to other texture as the following list summarizes:
            // diffuse: texture_diffuseN
            // specular: texture_specularN
            // normal: texture_normalN

            // 1. diffuse maps
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        }



        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.

    std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }


    std::vector<Texture> Model::loadEmbeddedTextures(const aiTexture* tex)
    {
        std::vector<Texture> textures;

        Texture embeddedTex;
        embeddedTex.id = TextureFromMemory(tex);
        embeddedTex.type = "Embedded Texture";
        embeddedTex.path = tex->mFilename.C_Str();

        textures.push_back(embeddedTex);

        return textures;
    }

    void Model::AlignAxis(const aiScene* scene)
    {
        if (scene->mMetaData)
        {
            std::cout << "aligning axis" << std::endl;
            int32_t upAxis = 1, upAxisSign = 1, frontAxis = 2, frontAxisSign = 1, coordAxis = 0, coordAxisSign = 1;
            double unitScaleFactor = 1.0;
            for (unsigned metaDataIndex = 0; metaDataIndex < scene->mMetaData->mNumProperties; ++metaDataIndex)
            {
                if (strcmp(scene->mMetaData->mKeys[metaDataIndex].C_Str(), "UpAxis") == 0)
                    scene->mMetaData->Get<int32_t>(metaDataIndex, upAxis);

                if (strcmp(scene->mMetaData->mKeys[metaDataIndex].C_Str(), "UpAxisSign") == 0)
                    scene->mMetaData->Get<int32_t>(metaDataIndex, upAxisSign);

                if (strcmp(scene->mMetaData->mKeys[metaDataIndex].C_Str(), "FrontAxis") == 0)
                    scene->mMetaData->Get<int32_t>(metaDataIndex, frontAxis);

                if (strcmp(scene->mMetaData->mKeys[metaDataIndex].C_Str(), "FrontAxisSign") == 0)
                    scene->mMetaData->Get<int32_t>(metaDataIndex, frontAxisSign);

                if (strcmp(scene->mMetaData->mKeys[metaDataIndex].C_Str(), "CoordAxis") == 0)
                    scene->mMetaData->Get<int32_t>(metaDataIndex, coordAxis);

                if (strcmp(scene->mMetaData->mKeys[metaDataIndex].C_Str(), "CoordAxisSign") == 0)
                    scene->mMetaData->Get<int32_t>(metaDataIndex, coordAxisSign);

                if (strcmp(scene->mMetaData->mKeys[metaDataIndex].C_Str(), "UnitScaleFactor") == 0)
                    scene->mMetaData->Get<double>(metaDataIndex, unitScaleFactor);
            }

            aiVector3D upVec, forwardVec, rightVec;

            upVec[upAxis] = upAxis * (float)unitScaleFactor;
            forwardVec[frontAxis] = frontAxisSign * (float)unitScaleFactor;
            rightVec[coordAxis] = coordAxisSign * (float)unitScaleFactor;

            aiMatrix4x4 mat(rightVec.x, rightVec.y, rightVec.z, 0.0f,
                            upVec.x, upVec.y, upVec.z, 0.0f,
                            forwardVec.x, forwardVec.y, forwardVec.z, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f);
            scene->mRootNode->mTransformation = mat;
        }
    }

    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
    {
        UNREFERENCED_PARAMETER(gamma);
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;
            else
                format = GL_RED;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path from testmodel: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
    unsigned int TextureFromMemory(const aiTexture* texture)
    {
        unsigned int textureID;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        int width, height, components_per_pixel;
        unsigned char* image_data = nullptr;

        if (texture->mHeight == 0)
        {
            image_data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texture->pcData), texture->mWidth, &width, &height, &components_per_pixel, 0);
        }
        else
        {
            image_data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texture->pcData), texture->mWidth * texture->mHeight, &width, &height, &components_per_pixel, 0);
        }

        if (components_per_pixel == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
        }
        else if (components_per_pixel == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(image_data);

        return textureID;
    }
}
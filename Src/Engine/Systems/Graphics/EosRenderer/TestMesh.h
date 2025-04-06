#pragma once

#include "TestShader.h"
#include "Src\pch.h"

namespace Test
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    struct Texture {
        unsigned int id = 0;
        std::string type;
        std::string path;
    };

    class Mesh {
    public:
        // mesh Data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;
        unsigned int VAO;

        // constructor
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        // render the mesh
        void Draw(Shader& shader);
        void DrawShadow(Shader& shader,unsigned int _depthMap);

    private:
        // render data 
        unsigned int VBO, EBO;

        // initializes all the buffer objects/arrays
        void setupMesh();
    };


}
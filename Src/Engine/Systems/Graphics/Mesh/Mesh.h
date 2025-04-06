//#pragma once
//
//#include <vector>
//
//#include "Src/Engine/Systems/Graphics/Importer/AssimpLoader.h"
//#include "Src/Engine/Systems/Graphics/Shader.h"
//
//class Mesh 
//{
//public:
//    // mesh Data
//    std::vector<Vertex>       vertices;
//    std::vector<unsigned int> indices;
//    std::vector<Texture>      textures;
//    unsigned int VAO;
//
//    // constructor
//    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
//
//    // render the mesh
//    void Draw(Eos::Shader& shader);
//
//private:
//    // render data 
//    unsigned int VBO, EBO;
//
//    // initializes all the buffer objects/arrays
//    void setupMesh();
//};
//
//class ImportedModel
//{
//    AssimpLoader assimploader;
//
//public:
//	std::vector<Texture> textures_loaded;
//    std::vector<Mesh> meshes;
//    std::string directory;
//
//    ImportedModel();
//    ImportedModel(const std::string& path);
//
//    // Draws models and its consisting meshes
//    void Draw(Eos::Shader& shader);
//};
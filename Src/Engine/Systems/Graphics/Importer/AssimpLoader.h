//
//#include <vector>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//#include <assimp/Importer.hpp>
//
//#include "ParserFlags.h"
//
//struct Vertex
//{
//	glm::vec3 position;
//	glm::vec2 texCoords;
//	glm::vec3 normals;
//	glm::vec3 tangent;
//	glm::vec3 bitangent;
//};
//
//struct Texture
//{
//	unsigned int id = 0;
//	std::string type;
//	std::string path;
//};
//
//struct ReturnBlock
//{
//	std::vector<Texture> textures_loaded;
//	std::vector<Mesh> meshes;
//	std::string directory;
//};
//
//class AssimpLoader
//{
//	// Private helper function
//	//void ProcessMaterials(const aiScene* scene, std::vector<std::string>& materials);
//	//void ProcessNode(void* transform, aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes);
//	//void ProcessMesh(void* transform, aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& outVertices, std::vector<uint32_t>& outIndices);
//
//	// Helper functions
//	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes, std::vector<Texture>& materials);
//	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Mesh>& meshes, std::vector<Texture>& materials);
//
//	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
//	std::vector<Texture> LoadEmbeddedTextures(const aiTexture* tex);
//
//	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
//	unsigned int TextureFromMemory(const aiTexture* texture);
//
//	void AlignAxis(const aiScene* scene);
//
//public:
//	AssimpLoader();
//	~AssimpLoader();
//
//	//ReturnBlock returnblock;
//
//	void LoadModel(const std::string& filepath, 
//		std::vector<Mesh>& meshes, 
//		std::vector<Texture>& materials);
//
//	/*bool LoadModel
//	(
//		const std::string & fileName,
//		std::vector<Mesh*>& meshes,
//		std::vector<std::string>& materials,
//		Eos::ParserFlags parserFlags
//	);*/
//};
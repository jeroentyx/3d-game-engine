#pragma once

#include "TestShader.h"
#include "TestMesh.h"
#include <fstream>
#include <sstream>
#include "stb_image\stb_image.h"
#include "Src\Engine\Systems\Graphics\Compiler\MeshLoader.h"

namespace Test
{
	inline unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
	inline unsigned int TextureFromMemory(const aiTexture* texture);

	class Model
	{
	public:
		// model data 
		std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
		std::vector<Mesh>    meshes;
		std::string directory;
		bool gammaCorrection;

		// constructor, expects a filepath to a 3D model.
		Model();
		Model(std::string const& path, bool gamma = false);
		void Init(std::string const& path);

		// draws the model, and thus all its meshes
		void Draw(Shader& shader);
		void DrawShadow(Shader& shader,unsigned int _depthMap);
		void Clear();

	private:
		// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
		void loadModel(std::string const& path);

		// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
		void processNode(aiNode* node, const aiScene* scene);

		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		// checks all material textures of a given type and loads the textures if they're not loaded yet.
		// the required info is returned as a Texture struct.
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		
		std::vector<Texture> loadEmbeddedTextures(const aiTexture* tex);

		void AlignAxis(const aiScene* scene);
	};
}

#pragma once

#include "../Importer/ParserFlags.h"
#include "TestModel.h"
#include "TestMesh.h"
#include "TestShader.h"
#include "Tools\Singleton.h"
#include "Tools\GraphicsHelper.h"
#include "Src\Engine\Systems\Graphics\Compiler\TextureLoader.h"

namespace Eos
{
	/*
	* A class to hold all the resources in the editor
	* Textures, icons and models so that it doesn't get 
	* loaded again to the editor
	*/
	class EosEditorAssets : 
		public Singleton<EosEditorAssets>
	{
	public:
		EosEditorAssets();
		~EosEditorAssets();

		void Init();
		Test::Model GetModel(const std::string& _id);
		Test::Shader GetShader(const std::string& _id);
		int GetTexture(const std::string& _id);

	private:
		std::unordered_map<std::string, Test::Model> m_models;
		std::unordered_map<std::string, Test::Model> m_staticMehses;
		std::unordered_map<std::string, Test::Shader> m_shaders;
		std::unordered_map<std::string, unsigned int> m_textures;
	};
}
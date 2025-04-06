#include "EosEditorResources.h"


namespace Eos
{

	EosEditorAssets::EosEditorAssets()
	{
		/*
		* For the files directory
		*/

	}
	EosEditorAssets::~EosEditorAssets()
	{
		/*
		* Destroy all the models
		*/


	}
	void EosEditorAssets::Init()
	{
		ParserFlags modelFlags = ParserFlags::NONE;



		modelFlags |= ParserFlags::TRIANGULATE;
		modelFlags |= ParserFlags::GEN_SMOOTH_NORMALS;
		modelFlags |= ParserFlags::OPTIMIZE_MESHES;
		modelFlags |= ParserFlags::OPTIMIZE_GRAPH;
		modelFlags |= ParserFlags::FIND_INSTANCES;
		modelFlags |= ParserFlags::CALC_TANGENT_SPACE;
		modelFlags |= ParserFlags::JOIN_IDENTICAL_VERTICES;
		modelFlags |= ParserFlags::DEBONE;
		modelFlags |= ParserFlags::FIND_INVALID_DATA;
		modelFlags |= ParserFlags::IMPROVE_CACHE_LOCALITY;
		modelFlags |= ParserFlags::GEN_UV_COORDS;
		modelFlags |= ParserFlags::PRE_TRANSFORM_VERTICES;
		modelFlags |= ParserFlags::GLOBAL_SCALE;

		/*
		* Set up all the icons and the textures model loading here
		*/

		//TODO: Create Model Loader
		//m_models["cube"] = EosModelCompiler::Create("cube/cube.obj", modelFlags);

		Test::Model model;
		//model.Init("cube/cube.obj");
		Test::Shader modelShader;
		Test::Shader normalShader;
		Test::Shader phongShader;
		Test::Shader shadowShader;

		modelShader.Init("mesh.vert", "mesh.frag");
		normalShader.Init("normal.vert","normal.frag");
		normalShader.Init("normal.vert","normal.frag");
		
		phongShader.Init("phong.vert", "phong.frag");

		//unsigned int diffuseMap = DDSCompiler::LoadOwnFormat("brick_albedo.dds");
		//unsigned int glossMap = DDSCompiler::LoadOwnFormat("brick_gloss.dds");
		//unsigned int normalMap = DDSCompiler::LoadOwnFormat("brick_normal.dds");


		m_models["cube"] = model;
		m_models["sphere"] = model;

		m_shaders["cube"] = modelShader;
		m_shaders["phong"] = phongShader;
		m_shaders["normal"] = normalShader;
		m_shaders["shadow"] = shadowShader;

		//this->m_textures["diffuse"] = diffuseMap;
		//this->m_textures["gloss"] = glossMap;
		//this->m_textures["normal"] = normalMap;

		//Commented out to run faster on released mode
		Test::Model spheremodel;
		//spheremodel.Init("sphere/sphere.obj");
		m_models["sphere"] = spheremodel;

	}

	Test::Model EosEditorAssets::GetModel(const std::string& _id)
	{
		if (m_models.find(_id) != m_models.end())
		{
			return m_models.at(_id);
		}

		return Test::Model();
	}
	Test::Shader EosEditorAssets::GetShader(const std::string& _id)
	{

		if (m_shaders.find(_id) != m_shaders.end())
		{
			return m_shaders.at(_id);
		}

		return Test::Shader();
	}
	int EosEditorAssets::GetTexture(const std::string& _id)
	{
		if (m_textures.find(_id) != m_textures.end())
		{
			return m_textures.at(_id);
		}
		return -1;
	}
}
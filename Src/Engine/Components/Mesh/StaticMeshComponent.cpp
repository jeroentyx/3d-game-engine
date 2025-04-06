#include "StaticMeshComponent.h"



Static_Mesh_Component::Static_Mesh_Component() : 
	meshptr{nullptr}
{

}

Static_Mesh_Component::Static_Mesh_Component(std::string filepath)
{

}

Static_Mesh_Component::~Static_Mesh_Component()
{

}

// Init will only be called when you reload the scene
void Static_Mesh_Component::Init()
{
	//m_model = Eos::EosEditorAssets::GetInstance().GetModel(m_model.directory);

	//Comment from Aloysius
	//If you want to have directory and the filename, What i suggest that u save the the entire filepath in a string 
	// and that will be the only file you will seriailise
	// then in the init is where u split the information into directory and filename.

	//m_model.Init("Assets/Models/FBX_Meshes/" + directory);
	
	//m_shader.Init("shaders/mesh.vert", "shaders/mesh.frag");

	
}

// Run time loading
//void Static_Mesh_Component::LoadModelAndShader(const std::string& filepath)
//{
//	directory = Eos::FileManager::GetFileNameWithExtension(filepath);
//
//	//m_model.Init("Assets/Models/FBX_Meshes/" + directory);
//	//m_shader.Init("shaders/mesh.vert", "shaders/mesh.frag");
//}

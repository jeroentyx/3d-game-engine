#include "MeshComponent.h"

Mesh_Component::Mesh_Component() :
	testVariable(0),
	type(MeshType::NONE),
	m_model(),
	m_shader()
{
}

Mesh_Component::~Mesh_Component()
{
}

void Mesh_Component::Init()
{
	m_model = Eos::EosEditorAssets::GetInstance().GetModel(m_model.directory);
	m_shader.Init("shaders/mesh.vert", "shaders/mesh.frag");
}



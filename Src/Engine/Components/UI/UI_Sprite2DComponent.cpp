#include "Src/pch.h"

#include "UI_Sprite2DComponent.h"

#include "Src\Engine\Systems\Graphics\Renderer\Texture.h"

// Called when component is created
UI_Sprite2D_Component::UI_Sprite2D_Component() : 
	color{glm::vec3(0.0f, 0.0f, 0.0f)},
	billboard_pos(glm::vec3{0.0f, 0.0f, 0.0f}),
	billboard_size{glm::vec2(20.0f)},
	isFixedSize{false}
{
	
}

// Called when component is destroyed
UI_Sprite2D_Component::~UI_Sprite2D_Component()
{
	
}

// Called when component is loaded in after being created
void UI_Sprite2D_Component::Init()
{
	textureobj = Texture2D::Create("Assets/Textures/" + textureName);
}

glm::vec2 UI_Sprite2D_Component::GetBillBoardSize()
{
	return billboard_size;
}

glm::vec3 UI_Sprite2D_Component::GetBillBoardPos()
{
	return billboard_pos;
}

glm::vec3 UI_Sprite2D_Component::GetColor()
{
	return color;
}

std::shared_ptr<Texture2D> UI_Sprite2D_Component::GetSprite()
{
	return textureobj;
}

void UI_Sprite2D_Component::RuntimeLoad(std::string path)
{
	//std::cout << path << std::endl;

	textureName = Eos::FileManager::GetFileNameWithExtension(path);

	textureobj = Texture2D::Create(path);
}



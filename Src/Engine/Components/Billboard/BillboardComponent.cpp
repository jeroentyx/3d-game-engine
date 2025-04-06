#include "BillboardComponent.h"

#include "Src\Engine\Systems\Graphics\Renderer\Texture.h"

BillBoard_Component::BillBoard_Component() :
	posOffset{0.0f, 0.0f, 0.0f},
	size{1.0f, 1.0f},
	isSet{false}
{
	//billboard_texture = Texture2D::Create("Assets/Textures/UI/container.png");
}

void BillBoard_Component::Init()
{

}
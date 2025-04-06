#pragma once

#include "Src\Engine\Core\Interfaces\IComponent.h"

class Texture2D;

class BillBoard_Component : public IComponent
{

public:
	BillBoard_Component();

	virtual void Init() override;
	
	std::string textureDirectory;
	//std::shared_ptr<Texture2D> billboard_texture;
	glm::vec3 posOffset;
	glm::vec2 size;
	bool isSet;

	property_vtable()
};

property_begin(BillBoard_Component)
{
	property_var(textureDirectory),
	property_var(posOffset),
	property_var(size),
	property_var(isSet)
}property_vend_h(BillBoard_Component)
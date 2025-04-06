#pragma once

#include "Src\Engine\Core\Interfaces\IComponent.h"

class Texture2D;

class UI_Sprite2D_Component : public IComponent
{
	glm::vec3 color;
	std::string textureName;

	glm::vec2 billboard_size;
	glm::vec3 billboard_pos;
	bool isFixedSize;

	
	std::shared_ptr<Texture2D> textureobj;

public:
	UI_Sprite2D_Component();
	~UI_Sprite2D_Component();

	virtual void Init() override;
	glm::vec2 GetBillBoardSize();
	glm::vec3 GetBillBoardPos();
	glm::vec3 GetColor();
	std::shared_ptr<Texture2D> GetSprite();

	void RuntimeLoad(std::string path);

	property_vtable()
};

property_begin(UI_Sprite2D_Component)
{
	property_var(color)
  , property_var(textureName)
}property_vend_h(UI_Sprite2D_Component)
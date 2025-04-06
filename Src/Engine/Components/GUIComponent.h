#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> //matrix transformations
#include <glm/gtx/quaternion.hpp> //toMat4
#include "Src/Engine/Core/Interfaces/IComponent.h"
#include "Src/Engine/Systems/Graphics/UIElements.h"

class GUI_Component : public IComponent
{

public:

	GUI_Component();
	~GUI_Component();

	virtual void Init() override;
	glm::vec3 TestVariable;

	glm::vec3 inColor;
	float m_Opacity;
	std::string filePath;


	//Getter
	glm::vec3 GetColor() const;
	float GetOpacity() const;
	std::string GetFilePath() const;

	//Setter
	void SetColor(glm::vec3& color);
	void SetOpacity(float& opaque);
	void SetFilePath(std::string& path);
	


	OpenGuiTexture m_GUI;
	property_vtable()
};

property_begin(GUI_Component)
{
	property_var(m_Opacity)
	, property_var(inColor)
	, property_var(filePath)
}property_vend_h(GUI_Component)


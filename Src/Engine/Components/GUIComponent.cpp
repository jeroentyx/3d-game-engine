#include "GUIComponent.h"

GUI_Component::GUI_Component() : m_Opacity(0.0f), inColor(glm::vec3(0, 0, 0))
{
	//empty for now
}

GUI_Component::~GUI_Component()
{
	//empty for now
}

void GUI_Component::Init()
{
	//empty for now
}

glm::vec3 GUI_Component::GetColor() const
{
	return inColor;
}

float GUI_Component::GetOpacity() const
{
	return m_Opacity;
}

std::string GUI_Component::GetFilePath() const
{
	return filePath;
}

void GUI_Component::SetColor(glm::vec3& color)
{
	this->m_GUI.UpdateColor(color);
	inColor = color;
}

void GUI_Component::SetOpacity(float& opaque)
{
	this->m_GUI.UpdateOpacity(opaque);
	m_Opacity = opaque;
}

void GUI_Component::SetFilePath(std::string& path)
{
	this->m_GUI.UpdateFilePath(path);
	filePath = path;
}

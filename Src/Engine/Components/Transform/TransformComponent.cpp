#include "TransformComponent.h"
#include "../Src/pch.h"

void Transform_Component::Init()
{

}

Transform_Component::Transform_Component() :
	Rotation(glm::vec3(0, 0, 0)),
	Scale(glm::vec3(0, 0, 0)),
	Translation(glm::vec3(0, 0, 0))
	//testVariable(0)
{
}

Transform_Component::~Transform_Component()
{
}


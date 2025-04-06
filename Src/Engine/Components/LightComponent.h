#pragma once
#include "Src\pch.h"
#include "Src/Engine/Core/Interfaces/IComponent.h"
#include "Transform\TransformComponent.h"

class Light_Component : public IComponent
{
public:
	Light_Component() = default;
	~Light_Component() = default;

	glm::vec3 Color = glm::vec3{ 0.0f, 0.0f, 0.0f };

	void Init() override;

	property_vtable()
};
property_begin(Light_Component)
{
	property_var(Color)
}property_vend_h(Light_Component)

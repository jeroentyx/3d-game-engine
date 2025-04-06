#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> //matrix transformations
#include <glm/gtx/quaternion.hpp> //toMat4
#include "Src/Engine/Core/Interfaces/IComponent.h"

class Transform_Component : public IComponent
{

public:
	glm::vec3 Translation;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	virtual void Init() override;

	//int testVariable;
	Transform_Component();
	~Transform_Component();

	void TransOffset(glm::vec3);
	void RotOffset(glm::vec3);
	void ScaleOffset(glm::vec3);

	glm::mat4 GetModelMatrix() const
	{
		//model matrix
		const glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
		return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
	}

	glm::mat4 rotToMat4() {
		glm::vec3 rad = glm::radians(Rotation);
		glm::vec3 sinRad = glm::sin(rad);
		glm::vec3 cosRad = glm::cos(rad);

		glm::mat4 rx{
			1,		  0,		 0,	0,
			0, cosRad.x, -sinRad.x, 0,
			0, sinRad.x,  cosRad.x, 0,
			0,		  0,	     0, 1
		};

		glm::mat4 ry{
			cosRad.y, 0, -sinRad.y, 0,
				   0, 1,		 0, 0,
			sinRad.y, 0,  cosRad.y, 0,
				   0, 0,	     0, 1
		};

		glm::mat4 rz{
			cosRad.z, -sinRad.z, 0, 0,
			sinRad.z,  cosRad.z, 0, 0,
				   0,		  0, 1, 0,
				   0,		  0, 0, 1
		};

		return rz * ry * rx;
	}
	property_vtable()
};

property_begin(Transform_Component)
{
    property_var(Translation)
  , property_var(Rotation)
  , property_var(Scale)
}property_vend_h(Transform_Component)

 
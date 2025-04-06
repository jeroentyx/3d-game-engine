#pragma once
#include "Src/Engine/Core/Interfaces/IComponent.h"


class Camera_Component : public IComponent
{

	glm::mat4 m_ProjectionMatrix = glm::mat4{ 1.0f };
	glm::mat4 m_ViewMatrix;

	float m_fov = 30.0f;
	float m_size;

	float m_yaw;
	float m_pitch;

	float distancefromplayer = 50;
	float angleAroundPlayer = 0;

	bool m_Active;

	glm::vec3 Position = glm::vec3{ 1.0f };
	glm::vec3 TargetRotation = glm::vec3{ 1.0f };
	glm::vec3 TargetPosition = glm::vec3{ 1.0f };

public:

	Camera_Component() = default;
    
	//Check for the Component if active
	bool GetActive()  const { return m_Active; }
	float GetFOV()    const { return m_fov; }
	float GetAspect() const { return m_size; }
	float GetYaw() { return m_yaw; }
	float GetPitch() { return m_pitch; }



	glm::vec3 GetPosition() const { return Position; }
	glm::vec3 GetTargetRotation() const { return TargetRotation; }
	glm::vec3 GetTargetPosition() const { return TargetPosition; }


	void SetActive(bool active) { m_Active = active; }
	void SetFOV(float fov)		{ m_fov = fov; }
	void SetAspect(float Aspect){ m_size = Aspect; }
	void SetYaw(float yaw) { m_yaw = yaw; }
	void SetPitch(float pitch) { m_pitch = pitch; }




	void SetPosition(glm::vec3& Pos)  { Position = Pos; }
	void SetTargetRotation(glm::vec3& Rot)  { TargetRotation = Rot; }
	void SetTargetPosition(glm::vec3& TPos) { TargetPosition = TPos; }

	glm::vec3 UpdateCameraPosition();


	//Calculate the matrix based of the character
	void CalculateMatrix();

	glm::mat4 GetViewMaxtrix();
	glm::mat4 GetProjectionMatrix();
	
	

	void Init() override;

	property_vtable()
};

property_begin(Camera_Component)
{
	property_var(m_Active)
  , property_var(m_fov)
  , property_var(m_size)

}property_vend_h(Camera_Component)
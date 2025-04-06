#include "CameraComponent.h"


glm::vec3 Camera_Component::UpdateCameraPosition()
{
	return glm::vec3(1, 1, 1);
}

void Camera_Component::CalculateMatrix()
{
	//m_ProjectionMatrix = glm::perspective(glm::radians(fov), (float)Eos::Context::GetInstance().scr_width / (float)Eos::Context::GetInstance().scr_height, 0.1f, 100.0f);
}

glm::mat4 Camera_Component::GetViewMaxtrix()
{
	return m_ViewMatrix;
}

glm::mat4 Camera_Component::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
}


void Camera_Component::Init()
{
}



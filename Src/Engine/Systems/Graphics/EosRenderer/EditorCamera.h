#ifndef EDITOR_CAMERA
#define EDITOR_CAMERA

#include <algorithm> 

#include <Src\pch.h>
#include "Src\Engine\Systems\Graphics\Shader.h"

namespace editor
{
	//
	// A Simple Camera to rotate/pann in the editor view
	//
	constexpr float staticYaw = -90.0f;
	constexpr float staticPitch = 0.0f;
	constexpr float staticSpeed = 4.0f;
	constexpr float staticSensitivity = 0.10f;
	constexpr float staticFov = glm::radians(45.0f);
	
	enum class CameraMovement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera
	{
	public:
		glm::vec3 position;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		glm::vec3 cameraRight;
		glm::vec3 worldUp;
		float cameraYaw;
		float cameraPitch;
		float cameraSpeed;
		float cameraSensitivity;
		float cameraFov;

		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = staticYaw,
			float pitch = staticPitch);
		~Camera();
		
		glm::mat4 GetViewMatrix();
		void GlfwKeyboardCall(CameraMovement direction, float deltaTime);
		void GlfwMouseCall(float xoffset, float yoffset, bool constrainPitch = true);
		void GlfwScrollCall(float yoffset);
	private:
		void UpdateCameraMatrix();
	};


}

#endif
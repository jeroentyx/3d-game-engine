#pragma once

#include "Src\pch.h"

namespace editor
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& _projection) :
			m_Projection(_projection)
		{

		}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);

	};

	enum class Mode
	{
		SPHERE,
		FLYCAM,
		NONE
	};

	class FpsCamera : public Camera
	{
	public:
		FpsCamera() = default;
		FpsCamera(float fov, float aspectRatio, float near, float far);

		void Focus(const glm::vec3& focusPoint);
		void Update();
		
		bool isActive() const { return this->m_Active; }
		bool SetActive(bool active) { return m_Active == active; }

		float GetDistance() const { return m_Distance; }
		void SetDistance(float distance) { m_Distance = distance; }

		const glm::vec3& GetFocalPoint() const { return m_FocalPoint; }
		void SetViewportSize(float width, float height)
		{
			//
			//	Setting this up for aspect ratio
			//
			(void)width;
			(void)height;
		}

	private:
		
		//
		//	Variables declarations
		//
		glm::mat4 m_View;
		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;
		glm::vec3 m_FocalPoint;

		bool m_Active = false;
		glm::vec2 m_InitialMousePosition{};
		
		float m_Distance;
		float  m_Speed{ 0.002f };
		float m_LastSpeed = 0.0f;

		//
		//	For the sphere to work , use like a spherical camera
		//
		float m_Pitch, m_Yaw;
		float m_PitchOffset;
		float m_YawOffset;
		glm::vec3 m_PositionDelta{};
		glm::vec3 m_RightDirection{};

		Mode editorMode{ Mode::SPHERE };
		
		//
		//	For the far clip
		//
		float minFocusDistance = 100.0f;

		float m_ViewportWidth = 1280.0f;
		float m_ViewportHeight = 720.0f;
		float m_Fov = 45.0f;
		float m_aspectRatio;
		float m_NearClip = 0.1f;
		float m_FarClip = 1000.0f;

		//
		//	Functions
		//

	};
}

#pragma once

#include "Camera/PerspectiveCamera.h"
#include "Texture.h"
#include "../Shader.h"
#include "Src/Engine/Components/Transform/TransformComponent.h"


class Renderer3D
{
public:
	static void Init();
	static void ShutDown();

	static void BeginScene(const PerspectiveCamera& camera);
	static void EndScene();
	static void Flush();

	// Drawing commands

	// Draw a cube with a standard color
	static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);

	// rotated cube is slightly more complicated because of the different rotating angles and amounts
	static void DrawRotatedCube(const glm::vec3& position, const glm::vec3& size, float angle, const glm::vec3& axis);
	static void DrawRotatedCube(const glm::vec3& position, const glm::vec3& size, const glm::quat& quaternion);

	// Draw AABB bound box
	static void DrawDebugBox(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color = {0.0f, 1.0f, 0.0f, 1.0f});
	static void DrawDebugBox(const Transform_Component& tc, const glm::vec4& color = { 0.0f, 1.0f, 0.0f, 1.0f });
	static void DrawDebugBox(const glm::vec3& center, const glm::vec3& scale, const glm::vec3& offset, const glm::vec4& color = { 0.0f, 1.0f, 0.0f, 1.0f });

	// Statistics
	struct Stats
	{
		int DrawCalls = 0;
		int QuadCount = 0;

		int GetTotalVertexCount() { return QuadCount * 4; }
		int GetTotalIndexCount() { return QuadCount * 6; }
	};

	static void ResetStats();
	static Stats GetStats();

private:
	static void FlushAndReset();
};

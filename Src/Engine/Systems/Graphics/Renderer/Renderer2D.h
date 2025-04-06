#pragma once

#include "Camera/PerspectiveCamera.h"
#include "Texture.h"

#include <array>

class Font_Component;

class Renderer2D
{
public:
	static void Init();
	static void ShutDown();

	static void BeginScene(const glm::mat4& projViewMatrix);
	static void EndScene();
	static void Flush();

	// Drawing commands
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));

	static void DrawQuad(const glm::mat4& matrix, const glm::vec4& color);
	static void DrawQuad(const glm::mat4& matrix, const std::shared_ptr<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));

	//static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
	//static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));

	//static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
	//static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));

	static void DrawTextComponent(Font_Component& comp, const glm::vec2& position, const std::string& text);
	static void DrawTextMSDF(const glm::vec2& position, const glm::quat& rot, float emheight, const std::string& text);

	// Statistic commands
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
	static void MSDFFlushReset(GLuint textureHandle);
};
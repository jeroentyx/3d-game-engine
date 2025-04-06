#pragma once

#include "RendererAPI.h"

class RenderCommand
{
public:
	static void Init()
	{
		s_RendererAPI->Init();
	}

	static void SetViewport(GLuint x, GLuint y, GLuint width, GLuint height)
	{
		s_RendererAPI->SetViewport(x, y, width, height);
	}

	static void SetClearColor(const glm::vec4& color)
	{
		s_RendererAPI->SetClearColor(color);
	}

	static void Clear()
	{
		s_RendererAPI->Clear();
	}

	static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, GLuint count = 0)
	{
		s_RendererAPI->DrawIndexed(vertexArray, count);
	}

	static void DrawIndexedLines(const std::shared_ptr<VertexArray>& vertexArray, GLuint count = 0)
	{
		s_RendererAPI->DrawIndexLines(vertexArray, count);
	}
private:
	static std::unique_ptr<RendererAPI> s_RendererAPI;
};

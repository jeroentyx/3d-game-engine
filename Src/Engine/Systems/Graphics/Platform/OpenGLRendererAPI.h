#pragma once

#include "../Renderer/RendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void Init() override;
	virtual void SetViewport(GLuint x, GLuint y, GLuint width, GLuint height) override;

	virtual void SetClearColor(const glm::vec4& color) override;
	virtual void Clear() override;

	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, GLuint indexCount = 0) override;
	virtual void DrawIndexLines(const std::shared_ptr<VertexArray>& vertexArray, GLuint indexCount = 0) override;
};
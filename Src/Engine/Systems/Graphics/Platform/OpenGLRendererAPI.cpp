#pragma once

#include <gl/glew.h>

#include "OpenGLRendererAPI.h"

#include <iostream>

void OpenGLRendererAPI::Init()
{
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRendererAPI::SetViewport(GLuint x, GLuint y, GLuint width, GLuint height)
{
	(void)x; (void)y;
	glViewport(0, 0, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
	(void)color;
}

void OpenGLRendererAPI::Clear()
{
}

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, GLuint indexCount)
{
	GLuint count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLRendererAPI::DrawIndexLines(const std::shared_ptr<VertexArray>& vertexArray, GLuint indexCount)
{
	GLuint count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
	glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, nullptr);
}



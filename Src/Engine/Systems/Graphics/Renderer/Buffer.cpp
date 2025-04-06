#include "Buffer.h"

#include "../Platform/OpenGLBuffer.h"

#include <gl/glew.h>

std::shared_ptr<VertexBuffer> VertexBuffer::Create(GLuint size)
{
	return std::make_shared<OpenGLVertexBuffer>(size);
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, GLuint size)
{
	return std::make_shared<OpenGLVertexBuffer>(vertices, size);
}

std::shared_ptr<IndexBuffer> IndexBuffer::Create(GLuint* indices, GLuint size)
{
	return std::make_shared<OpenGLIndexBuffer>(indices, size);
}

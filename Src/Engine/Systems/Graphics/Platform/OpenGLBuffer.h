#pragma once

#include "../Renderer/Buffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
	GLuint m_RendererID;
	BufferLayout m_Layout;

public:
	OpenGLVertexBuffer(GLuint size);
	OpenGLVertexBuffer(float* vertices, GLuint size);
	virtual ~OpenGLVertexBuffer();

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual void SetData(const void* data, GLuint size) override;

	virtual const BufferLayout& GetLayout() const { return m_Layout; }
	virtual void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
};


class OpenGLIndexBuffer : public IndexBuffer
{
	GLuint m_RendererID;
	GLuint m_Count;

public:
	OpenGLIndexBuffer(GLuint* vertices, GLuint size);
	virtual ~OpenGLIndexBuffer();

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual GLuint GetCount() const { return m_Count; }
};

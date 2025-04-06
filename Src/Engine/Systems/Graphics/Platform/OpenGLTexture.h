#pragma once

#include "../Renderer/Texture.h"
#include "../Shader.h"
#include <gl/glew.h>
#include <string>



class OpenGLTexture2D : public Texture2D
{
	std::string m_Path;
	GLuint m_Width, m_Height;
	GLuint m_RendererID;
	GLenum m_internalFormat, m_dataFormat;

public:
	OpenGLTexture2D(const GLuint width, const GLuint height);
	OpenGLTexture2D(const std::string& path);
	OpenGLTexture2D(const TextureSpecification& spec);
	virtual ~OpenGLTexture2D();

	virtual GLuint GetWidth() const override { return m_Width; }
	virtual GLuint GetHeight() const override { return m_Height; }
	virtual GLuint GetID() const override { return m_RendererID; }

	virtual void SetData(void* data, GLuint size) override;

	virtual void Bind(GLuint slot = 0) const override;
	virtual void Bind(const std::string& uniformSampler, const std::shared_ptr<Eos::Shader>& shader, uint32_t textureUnit) override;

	virtual bool operator==(const Texture& rhs) const override
	{
		return m_RendererID == ((OpenGLTexture2D&)rhs).m_RendererID;
	}
};
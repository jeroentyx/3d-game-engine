#include "OpenGLTexture.h"

#include "SOIL2.h"
#include <stb_image/stb_image.h>


#include <iostream>

GLenum TextureFilterToGL(TextureFilter filter)
{
	switch (filter)
	{
	case TextureFilter::Nearest:	return GL_NEAREST;
	case TextureFilter::Linear:		return GL_LINEAR;
	}
	return GL_NONE;
}

GLenum TextureWrapToGL(TextureWrap wrap)
{
	switch (wrap)
	{
	case TextureWrap::ClampToEdge:	return GL_CLAMP_TO_EDGE;
	case TextureWrap::Repeat:		return GL_REPEAT;
	case TextureWrap::MirrorRepeat: return GL_MIRRORED_REPEAT;
	}
	return GL_NONE;
}

GLenum TextureFormatToGL(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::RED:	return GL_RED;
	case TextureFormat::RGB:	return GL_RGB;
	case TextureFormat::RGBA:	return GL_RGBA;
	}
	return GL_NONE;
}

GLenum TextureFormatToInternalGL(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::RED:	return GL_R32F;
	case TextureFormat::RGB:	return GL_RGB12;
	case TextureFormat::RGBA:	return GL_RGBA8;
	}
	return GL_NONE;
}

GLenum TextureDataFormatToGL(TextureDataFormat format)
{
	switch (format)
	{
	case TextureDataFormat::UBYTE:	return GL_UNSIGNED_BYTE;
	}
	return GL_NONE;
}

/*
	2D texture creation function definitions
*/

OpenGLTexture2D::OpenGLTexture2D(const GLuint width, const GLuint height) : 
	m_Width{width}, m_Height{height}, 
	m_RendererID{0},
	m_internalFormat{ GL_RGBA8 },
	m_dataFormat{ GL_RGBA }
{
	//TODO;

}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) :
	m_Width{0}, m_Height{0},
	m_internalFormat{ GL_RGBA8 },
	m_dataFormat{ GL_RGBA }
{
    GLuint ddsTexture = SOIL_load_OGL_texture
    (
        path.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
    );
    if (ddsTexture == 0)
    {
        std::cout << "Error: loading Soil Texture ! ";
    }
    m_RendererID = ddsTexture; // A trivial passing of dds data to this OpenGLTexture that is not in used at the moment
	

	/*int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!data)
		std::cout << "Failed to load image" << std::endl;
	m_Width = width;
	m_Height = height;

	GLenum internalFormat = 0, dataFormat = 0;
	if (channels == 4)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (channels == 3)
	{
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}

	m_internalFormat = internalFormat;
	m_dataFormat = dataFormat;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, m_internalFormat, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	GLenum err = glGetError();

	stbi_image_free(data);*/
}

OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& spec)
{
	int texWidth, texHeight, texChannels;
	void* pixels_stbi = nullptr;
	void* pixels = nullptr;

	if (spec.TextureFilepath.size() > 1)
	{
		pixels_stbi = stbi_load(spec.TextureFilepath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		pixels = pixels_stbi;
		// info assert - "Failed to load the texture"
	}
	else if (spec.data != nullptr)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		pixels = (void*)spec.data;
		texWidth = spec.width;
		texHeight = spec.height;
	}
	else
	{
		// info assert - "No texture filepath or data"
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, TextureFormatToInternalGL(spec.format), texWidth, texHeight);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterToGL(spec.minFilter));
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterToGL(spec.magFilter));

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapToGL(spec.wrapS));
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapToGL(spec.wrapT));
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, TextureWrapToGL(spec.wrapR));

	glTextureSubImage2D(m_RendererID, 0, 0, 0, texWidth, texHeight, TextureFormatToGL(spec.format), TextureDataFormatToGL(spec.dataFormat), pixels);

	if (pixels_stbi != nullptr)
		stbi_image_free(pixels);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::SetData(void* data, GLuint size)
{
	(void)size;
	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::Bind(GLuint slot) const
{
	glBindTextureUnit(slot, m_RendererID);
}

void OpenGLTexture2D::Bind(const std::string& uniformSampler, const std::shared_ptr<Eos::Shader>& shader, uint32_t textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	shader->Bind();

	int location = glGetUniformLocation(shader->id, uniformSampler.c_str());
	glUniform1i(location, textureUnit);
}

#undef STB_IMAGE_IMPLEMENTATION
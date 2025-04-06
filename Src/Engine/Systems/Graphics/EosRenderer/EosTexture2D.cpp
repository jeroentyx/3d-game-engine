#include "EosTexture2D.h"

GLenum eos_format_from_channel_count(int channel_count)
{
	switch (channel_count)
	{
	case 1:
		return GL_LUMINANCE;
	case 2:
		return GL_LUMINANCE_ALPHA;
	case 3:
		return GL_RGB;
	case 4:
		return GL_RGBA;
	default:
		throw std::logic_error("Invalid texture format");
	}
}

GLenum eos_interal_format_from_channel_count(int channel_count, bool uses_srgb)
{
	switch (channel_count)
	{
	case 1:
		if (uses_srgb)
			return GL_SLUMINANCE;
		else
			return GL_LUMINANCE;
	case 2:
		if (uses_srgb)
			return GL_SLUMINANCE_ALPHA;
		else
			return GL_LUMINANCE_ALPHA;
	case 3:
		if (uses_srgb)
			return GL_SRGB;
		else
			return GL_RGB;
	case 4:
		if (uses_srgb)
			return GL_SRGB_ALPHA;
		else
			return GL_RGBA;
	default:
		throw std::logic_error("Invalid texture format");
	}
}

EosTexture2D Eosload_texture(const char* const path, bool is_color_data)
{
	int width, height, channel_count;
	stbi_loader data{ path, &width, &height, &channel_count, 0 };
	if (data.get_data() != nullptr)
	{
		return EosTexture2D(data.get_data(), width, height, channel_count, is_color_data);
	}
	else
	{
		throw std::runtime_error("Failed to load texture");
	}
}

EosTexture2D::EosTexture2D(const unsigned char* const data, int width_, int height_, int channel_count, bool uses_srgb) :
	EosTexture2D(data, width_, height_, eos_interal_format_from_channel_count(channel_count, uses_srgb), eos_format_from_channel_count(channel_count), true)
{
}

EosTexture2D::EosTexture2D(const unsigned char* const data, int width_, int height_, GLenum internal_format, GLenum format, bool generate_mipmap) :
	id(NULL),
	width(width_),
	height(height_)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data);
	if (generate_mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);
}

EosTexture2D::EosTexture2D(EosTexture2D&& other) noexcept :
	id(other.id),
	width(other.width),
	height(other.height)
{
	other.id = NULL;
}

void EosTexture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}

GLuint EosTexture2D::get_id() const
{
	return id;
}

EosTexture2D::~EosTexture2D()
{
	if (id != NULL)
		glDeleteTextures(1, &id);
}

const unsigned char* const stbi_loader::get_data()
{
	return data;
}

stbi_loader::~stbi_loader()
{
	stbi_image_free(data);
}

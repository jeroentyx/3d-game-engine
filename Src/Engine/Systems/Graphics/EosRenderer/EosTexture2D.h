#pragma once
#include "Src\pch.h"
#include "stb_image\stb_image.h"


GLenum eos_format_from_channel_count(int channel_count);
GLenum eos_interal_format_from_channel_count(int channel_count, bool uses_srgb);

class EosTexture2D
{

private:
	GLuint id;
public:
	const int width, height;

public:
	EosTexture2D(const unsigned char* const data, int width_, int height_, int channel_count, bool uses_srgb);

	EosTexture2D(const unsigned char* const data, int width_, int height_, GLenum internal_format, GLenum format, bool generate_mipmap);

	EosTexture2D(const EosTexture2D&) = delete;
	EosTexture2D(EosTexture2D&& other) noexcept;

	void bind() const;

	GLuint get_id() const;


	~EosTexture2D();
};

struct stbi_loader
{
private:
	unsigned char* data;

public:
	stbi_loader(char const* filename, int* x, int* y, int* comp, int req_comp) :
		data(stbi_load(filename, x, y, comp, req_comp))
	{
	}

	const unsigned char* const get_data();

	~stbi_loader();
};


EosTexture2D Eosload_texture(const char* const path, bool is_color_data);
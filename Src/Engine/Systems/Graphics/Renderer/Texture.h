#pragma once

#include <gl/glew.h>
#include <memory>
#include <string>

namespace Eos
{
	class Shader;
}

class Texture
{
public:
	virtual ~Texture() = default;

	virtual GLuint GetWidth() const = 0;
	virtual GLuint GetHeight() const = 0;
	virtual GLuint GetID() const = 0;

	virtual void SetData(void* data, GLuint size) = 0;

	virtual void Bind(GLuint slot = 0) const = 0;
	virtual void Bind(const std::string& uniformSampler, const std::shared_ptr<Eos::Shader>& shader, uint32_t textureUnit) = 0;

	virtual bool operator==(const Texture& rhs) const = 0;
};

enum class TextureFormat : uint8_t
{
	None = 0,
	RED,
	RGB,
	RGBA
};

enum class TextureDataFormat : uint8_t
{
	None = 0,
	UBYTE
};

enum class TextureWrap : uint8_t
{
	None = 0,
	ClampToEdge,
	Repeat,
	MirrorRepeat
};

enum class TextureFilter : uint8_t
{
	None = 0,
	Nearest,
	Linear
};

struct TextureSpecification
{
	uint32_t width;
	uint32_t height;
	TextureFormat format = TextureFormat::RGBA;
	TextureDataFormat dataFormat = TextureDataFormat::UBYTE;
	TextureWrap wrapS = TextureWrap::Repeat, wrapT = TextureWrap::Repeat, wrapR = TextureWrap::Repeat;
	TextureFilter minFilter = TextureFilter::Linear, magFilter = TextureFilter::Linear;

	std::string TextureFilepath;
	const void* data = nullptr;
};

class Texture2D : public Texture
{
public:
	static std::shared_ptr<Texture2D> Create(GLuint width, GLuint height);
	static std::shared_ptr<Texture2D> Create(const std::string& path);
	static std::shared_ptr<Texture2D> Create(const TextureSpecification& spec);
};
#include "Texture.h"
#include "../Platform/OpenGLTexture.h"

std::shared_ptr<Texture2D> Texture2D::Create(GLuint width, GLuint height)
{
	return std::make_shared<OpenGLTexture2D>(width, height);
}

std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
{
	return std::make_shared<OpenGLTexture2D>(path);
}

std::shared_ptr<Texture2D> Texture2D::Create(const TextureSpecification& spec)
{
	return std::make_shared<OpenGLTexture2D>(spec);
}

#include "RendererAPI.h"
#include "../Platform/OpenGLRendererAPI.h"

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

std::unique_ptr<RendererAPI> RendererAPI::Create()
{
	switch (s_API)
	{
	case RendererAPI::API::None:	return nullptr;
	case RendererAPI::API::OpenGL:	return std::make_unique<OpenGLRendererAPI>();
	}
	return nullptr;
}

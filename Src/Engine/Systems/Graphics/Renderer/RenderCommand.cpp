#include "RenderCommand.h"

#include "../Platform/OpenGLRendererAPI.h"

std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
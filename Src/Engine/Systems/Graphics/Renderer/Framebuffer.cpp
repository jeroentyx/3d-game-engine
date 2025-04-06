#include "Framebuffer.h"
#include "../Platform/OpenGLFramebuffer.h"

std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
{
	return std::make_shared<OpenGLFramebuffer>(spec);
}
#include "EosFramebuffer.h"

EosRenderbuffer::EosRenderbuffer(GLenum format, int width, int height)
{
	glGenRenderbuffers(1, &id);

	glNamedRenderbufferStorage(id, format, width, height);
}

GLuint EosRenderbuffer::get_id() const
{
	return id;
}

EosRenderbuffer::~EosRenderbuffer()
{
	glDeleteRenderbuffers(1, &id);
}

void EosFramebuffer::set_no_color_buffer()
{
	glNamedFramebufferDrawBuffer(id, GL_NONE);
	glNamedFramebufferReadBuffer(id, GL_NONE);
}

#pragma once
#include "Src\pch.h"
#include "EosTexture2D.h"

class EosRenderbuffer
{
private:
	GLuint id;

public:
	EosRenderbuffer(GLenum format, int width, int height);

	GLuint get_id() const;

	~EosRenderbuffer();
};

class EosFramebuffer
{
private:
	void set_no_color_buffer();

private:

public:
	GLuint id;
	template <typename CBT>
	EosFramebuffer(const std::vector<CBT>& color_buffers);

	template <typename CBT, typename DSBT>
	EosFramebuffer(const std::vector<CBT>& color_buffers, const DSBT& depth_stencil_buffer) :
		EosFramebuffer(color_buffers)
	{
		if constexpr (std::is_same_v<DSBT, EosTexture2D>)
		{
			glNamedFramebufferTexture(id, GL_DEPTH_STENCIL_ATTACHMENT, depth_stencil_buffer.get_id(), 0);
		}
		else if constexpr (std::is_same_v<DSBT, EosRenderbuffer>)
		{
			glNamedFramebufferRenderbuffer(id, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_buffer.get_id());
		}
		else
			throw "Invalid type";
	}

	template <typename CBT, typename DBT, typename SBT>
	EosFramebuffer(const std::vector<CBT>& color_buffers, const DBT& depth_buffer, const SBT& stencil_buffer) :
		EosFramebuffer(color_buffers)
	{
		if constexpr (std::is_same_v<DBT, EosTexture2D>)
		{
			glNamedFramebufferTexture(id, GL_DEPTH_ATTACHMENT, depth_buffer.get_id(), 0);
		}
		else if constexpr (std::is_same_v<DBT, EosRenderbuffer>)
		{
			glNamedFramebufferRenderbuffer(id, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer.get_id());
		}
		else if constexpr (!std::is_same_v<DBT, std::monostate>)
			throw "Invalid type";

		if constexpr (std::is_same_v<SBT, EosTexture2D>)
		{
			glNamedFramebufferTexture(id, GL_STENCIL_ATTACHMENT, stencil_buffer.get_id(), 0);
		}
		else if constexpr (std::is_same_v<SBT, EosRenderbuffer>)
		{
			glNamedFramebufferRenderbuffer(id, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencil_buffer.get_id());
		}
		else if constexpr (!std::is_same_v<SBT, std::monostate>)
			throw "Invalid type";
	}

	void bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	~EosFramebuffer()
	{
		glDeleteFramebuffers(1, &id);
	}
};

template<typename CBT>
EosFramebuffer::EosFramebuffer(const std::vector<CBT>& color_buffers)
{
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	if constexpr (std::is_same_v<CBT, EosTexture2D>)
	{
		if (color_buffers.size() > 0)
		{
			int i = 0;
			for (const auto& texture : color_buffers)
			{
				glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0 + i, texture.get_id(), 0);
				++i;
			}
		}
		else
			set_no_color_buffer();
	}
	else if constexpr (std::is_same_v<CBT, EosRenderbuffer>)
	{
		if (color_buffers.size() > 0)
		{
			int i = 0;
			for (const auto& renderbuffer : color_buffers)
			{
				glNamedFramebufferRenderbuffer(id, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, renderbuffer.get_id());
				++i;
			}
		}
		else
			set_no_color_buffer();
	}
	else if constexpr (std::is_same_v<CBT, std::monostate>)
	{
		set_no_color_buffer();
	}
	else
		throw "Invalid type";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

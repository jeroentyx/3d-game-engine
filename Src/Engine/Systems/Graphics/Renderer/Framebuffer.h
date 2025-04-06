#pragma once

#include <GL/glew.h>
#include <utility>
#include <vector>
#include <memory>

enum class FramebufferTextureFormat
{
	None = 0,

	// Color buffer
	RGBA8,
	RED_INTEGER,

	// Depth/stencil buffer
	DEPTH24STENCIL8,

	// Defaults
	Depth = DEPTH24STENCIL8
};

// Actual physical specification of a texture
struct FramebufferTextureSpecification
{
	FramebufferTextureSpecification() = default;
	FramebufferTextureSpecification(FramebufferTextureFormat format) : 
		TextureFormat(format) {}

	FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
};

// Collective framebuffer attachements
struct FramebufferAttachmentSpecification
{
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
		: Attachments(attachments) {}

	std::vector<FramebufferTextureSpecification> Attachments;
};

struct FramebufferSpecification
{
	uint32_t width{ 0 }, height{ 0 };
	FramebufferAttachmentSpecification Attachments;
	uint32_t Samples = 1;

	bool SwapChainTarget = false;
};

class Framebuffer
{
public:
	virtual ~Framebuffer() = default;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	virtual void Resize(uint32_t width, uint32_t height) = 0;
	virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

	virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

	virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

	virtual const FramebufferSpecification& GetSpecification() const = 0;

	static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
};
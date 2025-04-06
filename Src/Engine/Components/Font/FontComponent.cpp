#include "FontComponent.h"
#include "Src\Engine\Systems\Graphics\Platform\OpenGLVertexArray.h"
#include "Src/pch.h"
#include <tuple>

void Font_Component::LoadFontFile(const std::string& filepath)
{

}

Font_Component::Font_Component() :
	testVariable{0}
{
	_vao = VertexArray::Create();
	float vertices[5 * 4] = {
		 -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f
	};
	std::shared_ptr<VertexBuffer> _vbo;
	_vbo = VertexBuffer::Create(vertices, sizeof(vertices));
	_vbo->SetLayout({
		{ShaderDataType::Float3, "a_Position"},
		{ShaderDataType::Float2, "a_TexCoord"}
		});
	_vao->AddVertexBuffer(_vbo);

	GLuint indices[6] = { 0,1,2,2,3,1 };
	std::shared_ptr<IndexBuffer> _ibo;
	_ibo = IndexBuffer::Create(indices, sizeof(indices) / sizeof(GLuint));
	_vao->SetIndexBuffer(_ibo);

	fontshader = Eos::Shader::Create("shaders/msdfFont.glsl");
	//compiler.readTTF_to_MSDF("fontTFF/Lato-Regular.ttf");

	

	compiler.readTTF_to_ownFormat("fontTFF/Lato-Regular.ttf",
		codepoints, advances, uvs, quads, verticals, kernings);
}

void Font_Component::Init()
{

}

void Font_Component::GetFontCharUVBounds(uint32_t unicodeChar, float& out_l, float& out_r, float& out_b, float& out_t)
{
	if (uvs.count(unicodeChar) > 0)
	{
		out_l = std::get<0>(uvs[unicodeChar]);
		out_r = std::get<1>(uvs[unicodeChar]);
		out_b = std::get<2>(uvs[unicodeChar]);
		out_t = std::get<3>(uvs[unicodeChar]);
	}
	else
	{
		// Can consider adding unicodeChar and atlas to the error message
		PE_CORE_WARN("GetFontCharUVBounds(): Font does not contain character");
	}
}

void Font_Component::GetFontCharQuadBounds(uint32_t unicodeChar, float& out_l, float& out_r, float& out_b, float& out_t, uint32_t prevChar)
{
	if (quads.count(unicodeChar) > 0)
	{
		out_l = std::get<0>(quads[unicodeChar]);
		out_r = std::get<1>(quads[unicodeChar]);
		out_b = std::get<2>(quads[unicodeChar]);
		out_t = std::get<3>(quads[unicodeChar]);

		if (kernings.count(std::pair(unicodeChar, prevChar)) > 0)
		{
			out_l += kernings[std::pair(unicodeChar, prevChar)];
		}
	}
	else
	{
		PE_CORE_WARN("GetFontCharQuadBounds(): Font does not contain character");
	}
}

double Font_Component::GetFontCharAdvance(uint32_t unicodeChar)
{
	if (advances.count(unicodeChar) > 0)
	{
		return advances[unicodeChar];
	}
	else
	{
		PE_CORE_WARN("Graphics::GetFontCharAdvance(): Font does not contain character");
		return 0.0;
	}
}

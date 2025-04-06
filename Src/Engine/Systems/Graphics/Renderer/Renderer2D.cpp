#include "Renderer2D.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "../Shader.h"

#include "Src/Engine/Components/Font/FontComponent.h"

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};

struct Renderer2DData
{
	static const GLuint MaxQuads = 80;
	static const GLuint MaxVertices = MaxQuads * 4;
	static const GLuint MaxIndices = MaxQuads * 6;
	static const GLuint MaxTextureSlots = 32;		// Todo: Render Capability

	std::shared_ptr<VertexArray> QuadVertexArray;
	std::shared_ptr<VertexBuffer> QuadVertexBuffer;
	std::shared_ptr<Eos::Shader> TextureShader;
	std::shared_ptr<Texture2D> WhiteTexture;

	GLuint QuadIndexCount = 0;
	QuadVertex* QuadVertexBufferBase = nullptr;
	QuadVertex* QuadVertexBufferPtr = nullptr;

	std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
	GLuint TextureSlotIndex = 1;	// Start from 1, since 0 -> white texture

	glm::vec4 QuadVertexPositions[4] = { glm::vec4{ 0.0 } };

	Renderer2D::Stats stats;
};
static Renderer2DData s_Data;



struct MSDFVertex
{
	glm::vec3 Position;
	glm::vec2 Coordinate;
};

struct RendererMSDFData
{
	static const GLuint MaxQuads = 100;
	static const GLuint MaxVertices = MaxQuads * 4;
	static const GLuint MaxIndices = MaxQuads * 6;
	static const GLuint MaxTextureSlots = 32;

	std::shared_ptr<VertexArray> MSDFVAO;
	std::shared_ptr<VertexBuffer> MSDFVBO;
	std::shared_ptr<Eos::Shader> MSDFShader;

	GLuint MSDFIndexCount = 0;
	MSDFVertex* MSDFVertexBufferBase = nullptr;
	MSDFVertex* MSDFVertexBufferPtr = nullptr;

	glm::vec4 MSDFQuadVertexPositions[4] = { glm::vec4{0.0} };
};
static RendererMSDFData s_Text;



void Renderer2D::Init()
{
	// Standard 2D Renderer init
	{
		s_Data.QuadVertexArray = VertexArray::Create();
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex"},
			{ ShaderDataType::Float, "a_TilingFactor"}
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
		GLuint* quadIndices = new GLuint[s_Data.MaxIndices];

		GLuint offset = 0;
		for (GLuint i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		GLuint whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(GLuint));

		GLint samplers[s_Data.MaxTextureSlots];
		for (GLuint i = 0; i < s_Data.MaxTextureSlots; ++i)
			samplers[i] = i;

		// Reading in shader from file
		s_Data.TextureShader = Eos::Shader::Create("shaders/UI.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);

		// Set all texture slots to 0
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };	// Bottom left 
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };	// Bottom right
		s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };		// Top right
		s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };	// Top left
	}
	
	// MSDF Renderer init
	{
		s_Text.MSDFVAO = VertexArray::Create();
		s_Text.MSDFVBO = VertexBuffer::Create(s_Text.MaxVertices * sizeof(MSDFVertex));
		s_Text.MSDFVBO->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
			});
		s_Text.MSDFVAO->AddVertexBuffer(s_Text.MSDFVBO);

		s_Text.MSDFVertexBufferBase = new MSDFVertex[s_Text.MaxVertices];
		GLuint* indices = new GLuint[s_Text.MaxIndices];

		GLuint offset = 0;
		for (GLuint i = 0; i < s_Text.MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<IndexBuffer> quadIB = IndexBuffer::Create(indices, s_Text.MaxIndices);
		s_Text.MSDFVAO->SetIndexBuffer(quadIB);
		delete[] indices;

		// Textures will go here if needed

		s_Text.MSDFShader = Eos::Shader::Create("shaders/msdfFont.glsl");
		s_Text.MSDFShader->Bind();
		//

		s_Text.MSDFQuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };	// Bottom left 
		s_Text.MSDFQuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };	// Bottom right
		s_Text.MSDFQuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };		// Top right
		s_Text.MSDFQuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };	// Top left
	}
}

void Renderer2D::ShutDown()
{
	delete[] s_Data.QuadVertexBufferBase;
	delete[] s_Text.MSDFVertexBufferBase;
}
	 
void Renderer2D::BeginScene(const glm::mat4& projViewMatrix)
{
	s_Data.TextureShader->Bind();
	s_Data.TextureShader->SetMat4("u_ViewProjection", projViewMatrix);

	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	s_Data.TextureSlotIndex = 1;
}

void Renderer2D::EndScene()
{
	GLuint dataSize = (GLuint)((GLubyte*)s_Data.QuadVertexBufferPtr - (GLubyte*)s_Data.QuadVertexBufferBase);
	s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

	Flush();
}

void Renderer2D::Flush()
{
	s_Data.QuadVertexArray.get()->Bind();
	s_Data.TextureShader->Bind();

	// Bind textures
	for (GLuint i = 0; i < s_Data.TextureSlotIndex; ++i)
		s_Data.TextureSlots[i]->Bind(i);
	
	RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	s_Data.stats.DrawCalls++;
}

void Renderer2D::FlushAndReset()
{
	EndScene();

	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	s_Data.TextureSlotIndex = 1;
}

void Renderer2D::MSDFFlushReset(GLuint textureHandle)
{
	//EndScene();
	{
		GLuint dataSize = (GLuint)((GLubyte*)s_Text.MSDFVertexBufferPtr - (GLubyte*)s_Text.MSDFVertexBufferBase);
		s_Text.MSDFVBO->SetData(s_Text.MSDFVertexBufferBase, dataSize);
	}
	
	//Flush()
	{
		s_Text.MSDFVAO.get()->Bind();
		s_Text.MSDFShader->Bind();

		// Bind that one texture atlas containing the MSDF data
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		//for (GLuint i = 0; i < s_Data.TextureSlotIndex; ++i)
		//	s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Text.MSDFVAO, s_Text.MSDFIndexCount);
		//s_Data.stats.DrawCalls++;
	}

	s_Text.MSDFIndexCount = 0;
	s_Text.MSDFVertexBufferPtr = s_Text.MSDFVertexBufferBase;
}
	 
void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	DrawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		FlushAndReset();

	const float WhitetexIndex = 0.0f;
	const float tilingFactor = 1.0f;

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
	s_Data.QuadVertexBufferPtr->TexIndex = WhitetexIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = WhitetexIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = WhitetexIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = WhitetexIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadIndexCount += 6;

	s_Data.stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tint)
{
	DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tint);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tint)
{
	UNREFERENCED_PARAMETER(tint);
	s_Data.QuadVertexArray.get()->Bind();


	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		FlushAndReset();

	constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	float textureIndex = 0.0f;
	for (GLuint i = 1; i < s_Data.TextureSlotIndex; ++i)
	{
		if (*s_Data.TextureSlots[i].get() == *texture.get())
		{
			textureIndex = static_cast<float>(i);
			break;
		}
	}

	if (textureIndex == 0.0f)
	{
		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
		s_Data.TextureSlotIndex++;
	}

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadIndexCount += 6;

	s_Data.stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::mat4& matrix, const glm::vec4& color)
{
	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		FlushAndReset();

	const float WhitetexIndex = 0.0f;
	const float tilingFactor = 1.0f;

	s_Data.QuadVertexBufferPtr->Position = matrix * s_Data.QuadVertexPositions[0];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = WhitetexIndex;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = matrix * s_Data.QuadVertexPositions[1];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = WhitetexIndex;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = matrix * s_Data.QuadVertexPositions[2];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = WhitetexIndex;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = matrix * s_Data.QuadVertexPositions[3];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = WhitetexIndex;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadIndexCount += 6;

	s_Data.stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::mat4& matrix, const std::shared_ptr<Texture2D>& texture, const glm::vec4& tint)
{
	s_Data.QuadVertexArray.get()->Bind();

	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		FlushAndReset();

	constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	float textureIndex = 0.0f;
	for (GLuint i = 1; i < s_Data.TextureSlotIndex; ++i)
	{
		if (*s_Data.TextureSlots[i].get() == *texture.get())
		{
			textureIndex = static_cast<float>(i);
			break;
		}
	}

	if (textureIndex == 0.0f)
	{
		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
		s_Data.TextureSlotIndex++;
	}

	s_Data.QuadVertexBufferPtr->Position = matrix * s_Data.QuadVertexPositions[0];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = matrix * s_Data.QuadVertexPositions[1];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = matrix * s_Data.QuadVertexPositions[2];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadVertexBufferPtr->Position = matrix * s_Data.QuadVertexPositions[3];
	s_Data.QuadVertexBufferPtr->Color = color;
	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
	s_Data.QuadVertexBufferPtr++;

	s_Data.QuadIndexCount += 6;

	s_Data.stats.QuadCount++;
}

void Renderer2D::DrawTextComponent(Font_Component& comp, const glm::vec2& position, const std::string& text)
{
	// Check for flush and reset
	if (s_Text.MSDFIndexCount >= RendererMSDFData::MaxIndices)
		FlushAndReset();

	float cursorPos = 0.0f;

	// original has a quat orientation calculated into the write direction
	glm::vec3 writeDir = glm::vec3(1.0f, 0.0f, 0.0f);

	char prevChar = 0;
	for (const char& c : text)
	{
		// Calculate transform matrix
		glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), cursorPos * writeDir);

		// Get relevant data
		float uv_l, uv_r, uv_b, uv_t;
		float quad_l, quad_r, quad_b, quad_t;

		comp.GetFontCharUVBounds(c, uv_l, uv_r, uv_b, uv_t);
		comp.GetFontCharQuadBounds(c, quad_l, quad_r, quad_b, quad_t, prevChar);
		//GetFontCharUVBounds(fontAtlas, c, uv_l, uv_r, uv_b, uv_t);
		//GetFontCharQuadBounds(fontAtlas, c, quad_l, quad_r, quad_b, quad_t, prevChar);

		// Set relevant data
		s_Text.MSDFVertexBufferPtr->Position = transformMatrix * s_Text.MSDFQuadVertexPositions[0];
		s_Text.MSDFVertexBufferPtr->Coordinate = { uv_l,uv_b };
		s_Text.MSDFVertexBufferPtr++;

		s_Text.MSDFVertexBufferPtr->Position = transformMatrix * s_Text.MSDFQuadVertexPositions[1];
		s_Text.MSDFVertexBufferPtr->Coordinate = { uv_r,uv_b };
		s_Text.MSDFVertexBufferPtr++;

		s_Text.MSDFVertexBufferPtr->Position = transformMatrix * s_Text.MSDFQuadVertexPositions[2];
		s_Text.MSDFVertexBufferPtr->Coordinate = { uv_r,uv_t };
		s_Text.MSDFVertexBufferPtr++;

		s_Text.MSDFVertexBufferPtr->Position = transformMatrix * s_Text.MSDFQuadVertexPositions[3];
		s_Text.MSDFVertexBufferPtr->Coordinate = { uv_l,uv_t };
		s_Text.MSDFVertexBufferPtr++;

		s_Text.MSDFIndexCount += 6;

		// Update previous character
		prevChar = c;

		// Update cursor position
		cursorPos += comp.GetFontCharAdvance(c);
		//cursorPos += GetFontCharAdvance(fontAtlas, c);
	}
}

void Renderer2D::DrawTextMSDF(const glm::vec2& position, const glm::quat& rot, float emheight, const std::string& text)
{
	UNREFERENCED_PARAMETER(position);
	UNREFERENCED_PARAMETER(rot);
	UNREFERENCED_PARAMETER(emheight);
	//{
	//	// Can use position to replace cursorPos?
	//	float cursorPos = 0.0f;
	//	glm::vec3 writeDir = rot * glm::vec3(1.0f, 0.0f, 0.0f);

	//	// Set model matrix
	//	// Bind shader
	//	// Bind texture

	//	size_t quadNumber = 0;
	//	char prevChar = 0;
	//	for (const char& c : text)
	//	{
	//		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), cursorPos * writeDir);

	//		float l, r, b, t;

	//		// Assemble UVs
	//		//GetFontCharUVBounds(fontAtlas, c, l, r, b, t);
	//		outVerts[quadNumber * 4].in_uv = { l,b };
	//		outVerts[quadNumber * 4 + 1].in_uv = { r,b };
	//		outVerts[quadNumber * 4 + 2].in_uv = { r,t };
	//		outVerts[quadNumber * 4 + 3].in_uv = { l,t };

	//		// Assemble vertex pos considering the prev character
	//		//GetFontCharQuadBounds(fontAtlas, c, l, r, b, t, prevChar);
	//		outVerts[quadNumber * 4].ep_position = glm::vec3(transMatrix * glm::vec4(l, b, 0, 1));
	//		outVerts[quadNumber * 4 + 1].ep_position = glm::vec3(transMatrix * glm::vec4(r, b, 0, 1));
	//		outVerts[quadNumber * 4 + 2].ep_position = glm::vec3(transMatrix * glm::vec4(r, t, 0, 1));
	//		outVerts[quadNumber * 4 + 3].ep_position = glm::vec3(transMatrix * glm::vec4(l, t, 0, 1));

	//		outVerts[quadNumber * 4].in_tex = 0;
	//		outVerts[quadNumber * 4 + 1].in_tex = 0;
	//		outVerts[quadNumber * 4 + 2].in_tex = 0;
	//		outVerts[quadNumber * 4 + 3].in_tex = 0;

	//		quadNumber++;
	//		prevChar = c;
	//		cursorPos += GetFontCharAdvance(fontAtlas, c);
	//	}
	//}
	

	//{
	//	// Check for flush and reset
	//	if (s_Text.MSDFIndexCount >= RendererMSDFData::MaxIndices)
	//		FlushAndReset();

	//	float cursorPos = 0.0f;
	//	glm::vec3 writeDir = rot * glm::vec3(1.0f, 0.0f, 0.0f);

	//	char prevChar = 0;
	//	for (const char& c : text)
	//	{
	//		// Calculate transform matrix
	//		glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), cursorPos * writeDir);

	//		// Get relevant data
	//		float uv_l, uv_r, uv_b, uv_t;
	//		float quad_l, quad_r, quad_b, quad_t;

	//		GetFontCharUVBounds(fontAtlas, c, uv_l, uv_r, uv_b, uv_t);
	//		GetFontCharQuadBounds(fontAtlas, c, quad_l, quad_r, quad_b, quad_t, prevChar);

	//		// Set relevant data
	//		s_Text.MSDFVertexBufferPtr->Position = transformMatrix * s_Text.MSDFQuadVertexPositions[0];
	//		s_Text.MSDFVertexBufferPtr->Coordinate = { uv_l,uv_b };
	//		s_Text.MSDFVertexBufferPtr++;

	//		s_Text.MSDFVertexBufferPtr->Position = transformMatrix * s_Text.MSDFQuadVertexPositions[1];
	//		s_Text.MSDFVertexBufferPtr->Coordinate = { uv_r,uv_b };
	//		s_Text.MSDFVertexBufferPtr++;

	//		s_Text.MSDFVertexBufferPtr->Position = transformMatrix * s_Text.MSDFQuadVertexPositions[2];
	//		s_Text.MSDFVertexBufferPtr->Coordinate = { uv_r,uv_t };
	//		s_Text.MSDFVertexBufferPtr++;

	//		s_Text.MSDFVertexBufferPtr->Position = transformMatrix * s_Text.MSDFQuadVertexPositions[3];
	//		s_Text.MSDFVertexBufferPtr->Coordinate = { uv_l,uv_t };
	//		s_Text.MSDFVertexBufferPtr++;

	//		s_Text.MSDFIndexCount += 6;

	//		// Update previous character
	//		prevChar = c;

	//		// Update cursor position
	//		cursorPos += GetFontCharAdvance(fontAtlas, c);
	//	}
	//}

}


//void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
//{
//	DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, color);
//}

//void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tint)
//{
//	DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tint);
//}

//void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
//{
//	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
//		FlushAndReset();
//
//	const float textureIndex = 0.0f;
//	const float tilingFactor = 1.0f;
//
//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
//		glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
//		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
//
//	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
//	s_Data.QuadVertexBufferPtr->Color = color;
//	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
//	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
//	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
//	s_Data.QuadVertexBufferPtr++;
//
//	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
//	s_Data.QuadVertexBufferPtr->Color = color;
//	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
//	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
//	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
//	s_Data.QuadVertexBufferPtr++;
//
//	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
//	s_Data.QuadVertexBufferPtr->Color = color;
//	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
//	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
//	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
//	s_Data.QuadVertexBufferPtr++;
//
//	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
//	s_Data.QuadVertexBufferPtr->Color = color;
//	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
//	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
//	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
//	s_Data.QuadVertexBufferPtr++;
//
//	s_Data.QuadIndexCount += 6;
//
//	s_Data.stats.QuadCount++;
//}

//void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tint)
//{
//	UNREFERENCED_PARAMETER(tint);
//	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
//		FlushAndReset();
//
//	constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
//
//	float textureIndex = 0.0f;
//	for (GLuint i = 1; i < s_Data.TextureSlotIndex; ++i)
//	{
//		if (*s_Data.TextureSlots[i].get() == *texture.get())
//		{
//			textureIndex = static_cast<float>(i);
//			break;
//		}
//	}
//
//	if (textureIndex == 0.0f)
//	{
//		textureIndex = (float)s_Data.TextureSlotIndex;
//		s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
//		s_Data.TextureSlotIndex++;
//	}
//
//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
//		glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
//		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
//
//	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
//	s_Data.QuadVertexBufferPtr->Color = color;
//	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
//	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
//	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
//	s_Data.QuadVertexBufferPtr++;
//
//	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
//	s_Data.QuadVertexBufferPtr->Color = color;
//	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
//	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
//	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
//	s_Data.QuadVertexBufferPtr++;
//
//	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
//	s_Data.QuadVertexBufferPtr->Color = color;
//	s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
//	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
//	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
//	s_Data.QuadVertexBufferPtr++;
//
//	s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
//	s_Data.QuadVertexBufferPtr->Color = color;
//	s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
//	s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
//	s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
//	s_Data.QuadVertexBufferPtr++;
//
//	s_Data.QuadIndexCount += 6;
//
//	s_Data.stats.QuadCount++;
//}

void Renderer2D::ResetStats()
{
	memset(&s_Data.stats, 0, sizeof(Stats));
}

Renderer2D::Stats Renderer2D::GetStats()
{
	return s_Data.stats;
}

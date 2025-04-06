#include "Renderer3D.h"
#include "RenderCommand.h"
#include "VertexArray.h"

#include <array>
#include <glm/gtx/transform.hpp>

//Debug
#include <iostream>

// Each vertex of a cube contains the following
struct CubeVertex
{
	glm::vec3 Position;
	glm::vec4 color;
};

// Data for a solid cube w/wo textures
struct Renderer3DData
{
	// Batch settings
	static const GLuint MaxQuads = 10;
	static const GLuint MaxVertices = MaxQuads * 8;
	static const GLuint MaxIndices = MaxQuads * 36;
	//static const GLuint MaxTextureSlots = 16;

	// Essentials 
	std::shared_ptr<VertexArray> CubeVertexArray;
	std::shared_ptr<VertexBuffer> CubeVertexBuffer;
	std::shared_ptr<Eos::Shader> TextureShader;
	//std::shared_ptr<Texture2D> WhiteTexture;

	GLuint CubeIndexCount = 0;
	CubeVertex* CubeVertexBufferBase = nullptr;
	CubeVertex* CubeVertexBufferPtr = nullptr;

	//std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
	GLuint TextureSlotIndex = 1;

	glm::vec4 CubeVertexPositions[8] = { glm::vec4{0} };

	Renderer3D::Stats stats;
};
static Renderer3DData s_3DData;

// Data for debug box drawing
struct RendererDebug3D
{
	static const GLuint MaxQuads = 1;
	static const GLuint MaxVertices = MaxQuads * 8;
	static const GLuint MaxIndices = MaxQuads * 24;

	std::shared_ptr<VertexArray> DebugVertexArray;
	std::shared_ptr<VertexBuffer> DebugVertexBuffer;
	std::shared_ptr<Eos::Shader> DebugShader;

	GLuint DebugIndexCount = 0;
	CubeVertex* DebugVertexBufferBase = nullptr;
	CubeVertex* DebugVertexBufferPtr = nullptr;

	glm::vec4 DebugCubeVertexPositions[8] = { glm::vec4{0} };
};
static RendererDebug3D s_DebugData;

// Initialize 3D Renderer with required information
void Renderer3D::Init()
{
	//// VertexArray::Create() calls glCreateVertexArrays(1, rendererID)
	//s_3DData.CubeVertexArray = VertexArray::Create();
	//
	//s_3DData.CubeVertexBuffer = VertexBuffer::Create(s_3DData.MaxVertices * sizeof(CubeVertex));
	//s_3DData.CubeVertexBuffer->SetLayout({
	//	{ ShaderDataType::Float3, "aPos"},
	//	{ ShaderDataType::Float4, "aColor"}
	//	});
	//s_3DData.CubeVertexArray->AddVertexBuffer(s_3DData.CubeVertexBuffer);

	//s_3DData.CubeVertexBufferBase = new CubeVertex[s_3DData.MaxVertices];
	//GLuint* cubeIndices = new GLuint[s_3DData.MaxIndices];

	//// Not sharing vertices to avoid texturing coordintate complications
	//// 6 indices x 6 sides = 36
	//GLuint offset = 0;
	//for (GLuint i = 0; (i + 36) <= s_3DData.MaxIndices; i += 36)
	//{
	//	// front
	//	cubeIndices[i + 0] = offset + 0;
	//	cubeIndices[i + 1] = offset + 1;
	//	cubeIndices[i + 2] = offset + 2;

	//	cubeIndices[i + 3] = offset + 2;
	//	cubeIndices[i + 4] = offset + 3;
	//	cubeIndices[i + 5] = offset + 0;

	//	// right
	//	cubeIndices[i + 6] = offset + 1;
	//	cubeIndices[i + 7] = offset + 5;
	//	cubeIndices[i + 8] = offset + 6;

	//	cubeIndices[i + 9] = offset + 6;
	//	cubeIndices[i + 10] = offset + 2;
	//	cubeIndices[i + 11] = offset + 1;

	//	// back
	//	cubeIndices[i + 12] = offset + 7;
	//	cubeIndices[i + 13] = offset + 6;
	//	cubeIndices[i + 14] = offset + 5;

	//	cubeIndices[i + 15] = offset + 5;
	//	cubeIndices[i + 16] = offset + 4;
	//	cubeIndices[i + 17] = offset + 7;

	//	// left
	//	cubeIndices[i + 18] = offset + 4;
	//	cubeIndices[i + 19] = offset + 0;
	//	cubeIndices[i + 20] = offset + 3;

	//	cubeIndices[i + 21] = offset + 3;
	//	cubeIndices[i + 22] = offset + 7;
	//	cubeIndices[i + 23] = offset + 4;

	//	// bottom
	//	cubeIndices[i + 24] = offset + 4;
	//	cubeIndices[i + 25] = offset + 5;
	//	cubeIndices[i + 26] = offset + 1;

	//	cubeIndices[i + 27] = offset + 1;
	//	cubeIndices[i + 28] = offset + 0;
	//	cubeIndices[i + 29] = offset + 4;

	//	// top
	//	cubeIndices[i + 30] = offset + 3;
	//	cubeIndices[i + 31] = offset + 2;
	//	cubeIndices[i + 32] = offset + 6;

	//	cubeIndices[i + 33] = offset + 6;
	//	cubeIndices[i + 34] = offset + 7;
	//	cubeIndices[i + 35] = offset + 3;

	//	// increment offset
	//	offset += 8;
	//}

	//std::shared_ptr<IndexBuffer> cubeIB = IndexBuffer::Create(cubeIndices, s_3DData.MaxIndices);
	//s_3DData.CubeVertexArray->SetIndexBuffer(cubeIB);
	//delete[] cubeIndices;

	//// Set white texture
	////s_3DData.WhiteTexture = Texture2D::Create(1, 1);
	////GLuint whiteTextureData = 0xffffffff;
	////s_3DData.WhiteTexture->SetData(&whiteTextureData, sizeof(GLuint));

	///*GLint samplers[s_3DData.MaxTextureSlots];
	//for (GLuint i = 0; i < s_3DData.MaxTextureSlots; ++i)
	//	samplers[i] = i;*/

	//s_3DData.TextureShader = Shader::Create("shaders/testing3dSingle.glsl");
	//s_3DData.TextureShader->Bind();
	////s_3DData.TextureShader->SetIntArray("u_Texture", samplers, s_3DData.MaxTextureSlots);

	////s_3DData.TextureSlots[0] = s_3DData.WhiteTexture;

	//// 8 positions to represent sides of the cube
	//// Front face
	//s_3DData.CubeVertexPositions[0] = { -0.5f, -0.5f, 0.5f, 1.0f };
	//s_3DData.CubeVertexPositions[1] = {  0.5f, -0.5f, 0.5f, 1.0f };
	//s_3DData.CubeVertexPositions[2] = {  0.5f,  0.5f, 0.5f, 1.0f };
	//s_3DData.CubeVertexPositions[3] = { -0.5f,  0.5f, 0.5f, 1.0f };
	//// Back face
	//s_3DData.CubeVertexPositions[4] = { -0.5, -0.5, -0.5, 1.0f };
	//s_3DData.CubeVertexPositions[5] = {  0.5, -0.5, -0.5, 1.0f };
	//s_3DData.CubeVertexPositions[6] = {  0.5,  0.5, -0.5, 1.0f };
	//s_3DData.CubeVertexPositions[7] = { -0.5,  0.5, -0.5, 1.0f };





	s_DebugData.DebugVertexArray = VertexArray::Create();
	s_DebugData.DebugVertexBuffer = VertexBuffer::Create(s_DebugData.MaxVertices * sizeof(CubeVertex));
	s_DebugData.DebugVertexBuffer->SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float4, "aColor"}
		});
	s_DebugData.DebugVertexArray->AddVertexBuffer(s_DebugData.DebugVertexBuffer);

	s_DebugData.DebugVertexBufferBase = new CubeVertex[s_DebugData.MaxVertices];
	GLuint* debugindices = new GLuint[s_DebugData.MaxIndices];

	GLuint offset = 0;
	for (GLuint i = 0; (i + 24) <= s_DebugData.MaxIndices; i += 24)
	{
		// 1st
		debugindices[i + 0] = offset + 0;
		debugindices[i + 1] = offset + 1;
		debugindices[i + 2] = offset + 1;
		debugindices[i + 3] = offset + 2;
		debugindices[i + 4] = offset + 2;
		debugindices[i + 5] = offset + 3;

		// 2nd
		debugindices[i + 6] = offset + 4;
		debugindices[i + 7] = offset + 0;
		debugindices[i + 8] = offset + 0;
		debugindices[i + 9] = offset + 3;
		debugindices[i + 10] = offset + 3;
		debugindices[i + 11] = offset + 7;

		// 3rd
		debugindices[i + 12] = offset + 5;
		debugindices[i + 13] = offset + 4;
		debugindices[i + 14] = offset + 4;
		debugindices[i + 15] = offset + 7;
		debugindices[i + 16] = offset + 7;
		debugindices[i + 17] = offset + 6;

		// 4th
		debugindices[i + 18] = offset + 1;
		debugindices[i + 19] = offset + 5;
		debugindices[i + 20] = offset + 5;
		debugindices[i + 21] = offset + 6;
		debugindices[i + 22] = offset + 6;
		debugindices[i + 23] = offset + 2;

		offset += 8;
	}

	std::shared_ptr<IndexBuffer> debugIB = IndexBuffer::Create(debugindices, s_DebugData.MaxIndices);
	s_DebugData.DebugVertexArray->SetIndexBuffer(debugIB);
	delete[] debugindices;

	s_DebugData.DebugShader = Eos::Shader::Create("shaders/testing3dSingle.glsl");
	s_DebugData.DebugShader->Bind();

	// Will affect debug box size
	// Front face
	s_DebugData.DebugCubeVertexPositions[0] = { -0.5f, -0.5f, 0.5f, 1.0f };
	s_DebugData.DebugCubeVertexPositions[1] = { 0.5f, -0.5f, 0.5f, 1.0f };
	s_DebugData.DebugCubeVertexPositions[2] = { 0.5f,  0.5f, 0.5f, 1.0f };
	s_DebugData.DebugCubeVertexPositions[3] = { -0.5f,  0.5f, 0.5f, 1.0f };

	// Back face
	s_DebugData.DebugCubeVertexPositions[4] = { -0.5f, -0.5f, -0.5f, 1.0f };
	s_DebugData.DebugCubeVertexPositions[5] = { 0.5f, -0.5f, -0.5f, 1.0f };
	s_DebugData.DebugCubeVertexPositions[6] = { 0.5f,  0.5f, -0.5f, 1.0f };
	s_DebugData.DebugCubeVertexPositions[7] = { -0.5f,  0.5f, -0.5f, 1.0f };
}

// Shutsdown renderer and releases allocated memory
void Renderer3D::ShutDown()
{
	delete[] s_3DData.CubeVertexBufferBase;
	delete[] s_DebugData.DebugVertexBufferBase;
}

// Starts a scene and takes in a perspective camera
void Renderer3D::BeginScene(const PerspectiveCamera& camera)
{
	s_DebugData.DebugShader->Bind();
	
	s_DebugData.DebugShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	
	// Normal cube drawing
	s_3DData.CubeIndexCount = 0;
	s_3DData.CubeVertexBufferPtr = s_3DData.CubeVertexBufferBase;

	// Debug cube drawing
	s_DebugData.DebugIndexCount = 0;
	s_DebugData.DebugVertexBufferPtr = s_DebugData.DebugVertexBufferBase;

	//s_3DData.TextureSlotIndex = 1;
}

void Renderer3D::EndScene()
{
	GLuint drawDataSize = (GLuint)((GLubyte*)s_3DData.CubeVertexBufferPtr - (GLubyte*)s_3DData.CubeVertexBufferBase);
	if (drawDataSize)
		s_3DData.CubeVertexBuffer->SetData(s_3DData.CubeVertexBufferBase, drawDataSize);

	GLuint debugDataSize = (GLuint)((GLubyte*)s_DebugData.DebugVertexBufferPtr - (GLubyte*)s_DebugData.DebugVertexBufferBase);
	if (debugDataSize)
		s_DebugData.DebugVertexBuffer->SetData(s_DebugData.DebugVertexBufferBase, debugDataSize);

	Flush();
}

void Renderer3D::Flush()
{
	// Bind 3D textures

	// Check if there is any indexes to draw for normal cube
	if (s_3DData.CubeIndexCount)
	{
		RenderCommand::DrawIndexed(s_3DData.CubeVertexArray, s_3DData.CubeIndexCount);
		s_3DData.stats.DrawCalls++;
	}
	
	// Check if there is any indexes to draw for debug cube
	if (s_DebugData.DebugIndexCount)
	{
		RenderCommand::DrawIndexedLines(s_DebugData.DebugVertexArray, s_DebugData.DebugIndexCount);
		s_3DData.stats.DrawCalls++;
	}
}

void Renderer3D::FlushAndReset()
{
	EndScene();

	s_3DData.CubeIndexCount = 0;
	s_3DData.CubeVertexBufferPtr = s_3DData.CubeVertexBufferBase;

	s_DebugData.DebugIndexCount = 0;
	s_DebugData.DebugVertexBufferPtr = s_DebugData.DebugVertexBufferBase;
}

void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
{
	if (s_3DData.CubeIndexCount >= Renderer3DData::MaxIndices)
		FlushAndReset();

	const float whiteTexIndex = 0.0f;
	const float tilingFactor = 1.0f;

	// translate * rotate * scale
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		glm::scale(glm::mat4(1.0f), size);

	s_3DData.CubeVertexBufferPtr->Position = transform * s_3DData.CubeVertexPositions[0];
	s_3DData.CubeVertexBufferPtr->color = color;
	//s_3DData.CubeVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
	//s_3DData.CubeVertexBufferPtr->TexIndex = whiteTexIndex;
	s_3DData.CubeVertexBufferPtr++;
	
	s_3DData.CubeVertexBufferPtr->Position = transform * s_3DData.CubeVertexPositions[1];
	s_3DData.CubeVertexBufferPtr->color = color;
	//s_3DData.CubeVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
	//s_3DData.CubeVertexBufferPtr->TexIndex = whiteTexIndex;
	s_3DData.CubeVertexBufferPtr++;

	s_3DData.CubeVertexBufferPtr->Position = transform * s_3DData.CubeVertexPositions[2];
	s_3DData.CubeVertexBufferPtr->color = color;
	//s_3DData.CubeVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
	//s_3DData.CubeVertexBufferPtr->TexIndex = whiteTexIndex;
	s_3DData.CubeVertexBufferPtr++;

	s_3DData.CubeVertexBufferPtr->Position = transform * s_3DData.CubeVertexPositions[3];
	s_3DData.CubeVertexBufferPtr->color = color;
	//s_3DData.CubeVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
	//s_3DData.CubeVertexBufferPtr->TexIndex = whiteTexIndex;
	s_3DData.CubeVertexBufferPtr++;

	s_3DData.CubeVertexBufferPtr->Position = transform * s_3DData.CubeVertexPositions[4];
	s_3DData.CubeVertexBufferPtr->color = color;
	//s_3DData.CubeVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
	//s_3DData.CubeVertexBufferPtr->TexIndex = whiteTexIndex;
	s_3DData.CubeVertexBufferPtr++;

	s_3DData.CubeVertexBufferPtr->Position = transform * s_3DData.CubeVertexPositions[5];
	s_3DData.CubeVertexBufferPtr->color = color;
	//s_3DData.CubeVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
	//s_3DData.CubeVertexBufferPtr->TexIndex = whiteTexIndex;
	s_3DData.CubeVertexBufferPtr++;

	s_3DData.CubeVertexBufferPtr->Position = transform * s_3DData.CubeVertexPositions[6];
	s_3DData.CubeVertexBufferPtr->color = color;
	//s_3DData.CubeVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
	//s_3DData.CubeVertexBufferPtr->TexIndex = whiteTexIndex;
	s_3DData.CubeVertexBufferPtr++;

	s_3DData.CubeVertexBufferPtr->Position = transform * s_3DData.CubeVertexPositions[7];
	s_3DData.CubeVertexBufferPtr->color = color;
	//s_3DData.CubeVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
	//s_3DData.CubeVertexBufferPtr->TexIndex = whiteTexIndex;
	s_3DData.CubeVertexBufferPtr++;

	s_3DData.CubeIndexCount += 36;

	//for (GLuint i = 0; i < s_3DData.TextureSlotIndex; ++i)
		//s_3DData.TextureSlots[i]->Bind(i);


	//GLuint dataSize = (GLubyte*)s_3DData.CubeVertexBufferPtr - (GLubyte*)s_3DData.CubeVertexBufferBase;
	//s_3DData.CubeVertexBuffer->SetData(s_3DData.CubeVertexBufferBase, dataSize);

	//RenderCommand::DrawIndexed(s_3DData.CubeVertexArray, s_3DData.CubeIndexCount);
	//glDrawElements(GL_TRIANGLES, s_3DData.CubeVertexArray->GetIndexBuffer()->GetCount(),
	//	GL_UNSIGNED_INT, nullptr);
}

// Normal version
void Renderer3D::DrawRotatedCube(const glm::vec3& position, const glm::vec3& size, float angle, const glm::vec3& axis)
{
	if (s_DebugData.DebugIndexCount >= RendererDebug3D::MaxIndices)
		FlushAndReset();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis) *
		glm::scale(glm::mat4(1.0f), size);

	const glm::vec4 color = glm::vec4(0.0f, 1.0, 0.0f, 1.0f);

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[0];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[1];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[2];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[3];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[4];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[5];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[6];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[7];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugIndexCount += 24;
}

// Quaternion overload
void Renderer3D::DrawRotatedCube(const glm::vec3& position, const glm::vec3& size, const glm::quat& quaternion)
{
	if (s_DebugData.DebugIndexCount >= RendererDebug3D::MaxIndices)
		FlushAndReset();

	const glm::vec4 color = { 0.0f, 1.0f, 0.0f, 1.0f };

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		glm::mat4_cast(quaternion) *
		glm::scale(glm::mat4(1.0f), size);

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[0];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[1];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[2];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[3];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[4];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[5];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[6];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[7];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugIndexCount += 24;
}

void Renderer3D::DrawDebugBox(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
{
	//use vao
	s_DebugData.DebugVertexArray.get()->Bind();

	if (s_DebugData.DebugIndexCount >= RendererDebug3D::MaxIndices)
		FlushAndReset();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		glm::scale(glm::mat4(1.0f), size);

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[0];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[1];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[2];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[3];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[4];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[5];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[6];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[7];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	// Index count might change
	s_DebugData.DebugIndexCount += 24;
}

void Renderer3D::DrawDebugBox(const Transform_Component& tc, const glm::vec4& color)
{
	s_DebugData.DebugVertexArray.get()->Bind();

	if (s_DebugData.DebugIndexCount >= RendererDebug3D::MaxIndices)
		FlushAndReset();

	glm::mat4 transform = tc.GetModelMatrix();

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[0];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[1];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[2];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[3];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[4];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[5];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[6];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[7];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugIndexCount += 24;
}

void Renderer3D::DrawDebugBox(const glm::vec3& center, const glm::vec3& scale, const glm::vec3& offset, const glm::vec4& color)
{
	s_DebugData.DebugVertexArray.get()->Bind();

	if (s_DebugData.DebugIndexCount >= RendererDebug3D::MaxIndices)
		FlushAndReset();
	
	glm::vec3 newTranslation = center + offset;
	glm::mat4 transform{ 1.0f };
	transform = glm::scale(glm::translate(transform, newTranslation), scale);
	
	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[0];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[1];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[2];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[3];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[4];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[5];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[6];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugVertexBufferPtr->Position = transform * s_DebugData.DebugCubeVertexPositions[7];
	s_DebugData.DebugVertexBufferPtr->color = color;
	s_DebugData.DebugVertexBufferPtr++;

	s_DebugData.DebugIndexCount += 24;
}




void Renderer3D::ResetStats()
{
	memset(&s_3DData.stats, 0, sizeof(Stats));
}

Renderer3D::Stats Renderer3D::GetStats()
{
	return s_3DData.stats;
}

#include "Renderer.h"

std::unique_ptr<Renderer::SceneData> Renderer::m_SceneData = std::make_unique<Renderer::SceneData>();

void Renderer::BeginScene(PerspectiveCamera& camera)
{
	m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const std::shared_ptr<Eos::Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
{
	shader->Bind();
	shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
	shader->SetMat4("u_Transform", transform);


	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
}

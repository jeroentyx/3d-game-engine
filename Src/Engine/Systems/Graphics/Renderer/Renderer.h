#pragma once

#include <memory>
#include "VertexArray.h"
#include "RenderCommand.h"
//#include "Camera/OrthographicCamera.h"
#include "Camera/PerspectiveCamera.h"
#include "../Shader.h"

class Renderer
{
public:
	static void BeginScene(PerspectiveCamera& camera);
	static void EndScene();

	static void Submit(const std::shared_ptr<Eos::Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

private:
	struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
	};

	static std::unique_ptr<SceneData> m_SceneData;
};
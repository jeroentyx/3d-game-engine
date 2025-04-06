#pragma once

#include "Shader.h"
#include "stb_image.h"
#include <string>

class OpenGuiTexture
{
public:
	OpenGuiTexture();

	~OpenGuiTexture();
	
	void CreateVAOLayout();

	void UpdateTransform(const glm::mat4& LocalToWorld);
	void UpdateOpacity(const float& opacity);
	void UpdateDepth(const float d_value);
	void UpdateFilePath(const std::string& filePath);
	void CreateTexture2D();
	bool CheckForDirtyPath(const std::string& newPath);
	void UpdateColor(const glm::vec3& color);
	void UpdateGamma(const bool _gamma);

	void Render();

	void Cleanup();

	//Variable declarations
	Eos::Shader ShaderProgram;
	unsigned int vao, vbo, ebo;
	unsigned int textureID;
	std::string m_CurrFilePath;
};
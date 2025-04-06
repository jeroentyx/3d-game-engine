
#include "SkyboxComponent.h"
#include "Tools/GraphicsHelper.h"
#include "Src/Engine/Systems/Graphics/Shader.h"

#include <Src/pch.h>

// Constructor
Skybox_Component::Skybox_Component()
{
	// Build vao of skybox
 //   glGenVertexArrays(1, &vao);
 //   glGenBuffers(1, &vbo);
 //   glBindVertexArray(vao);
 //   glBindBuffer(GL_ARRAY_BUFFER, vbo);
 //   glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
 //   glEnableVertexAttribArray(0);
 //   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//std::vector<std::string> faces
	//{
	//	"Assets/Textures/GameBox/Right.png",
	//	"Assets/Textures/GameBox/Left.png",
	//	"Assets/Textures/GameBox/Up.png",
	//	"Assets/Textures/GameBox/Bottom.png",
	//	"Assets/Textures/GameBox/Front.png",
	//	"Assets/Textures/GameBox/Back.png"
	//};

 //   // Texture loading
	//cubemapTexture = loadCubemap(faces);

	//// Init Shaders
	//Eos::Shader shader;
	//shader.Init("shaders/skybox.vert", "shaders/skybox.frag");
	//skyboxShader = std::make_shared<Eos::Shader>(shader);
	//skyboxShader = Eos::Shader::Create("shaders/skybox.glsl");

	//skyboxShader->Bind();
	//skyboxShader->SetInt("skybox", 0);
	//skyboxShader->unBind();
}

void Skybox_Component::Init()
{

}

float Skybox_Component::GetRotSpeed()
{
	return rotationSpeed;
}

glm::vec3 Skybox_Component::GetRotDir()
{
	return rotationDir;
}


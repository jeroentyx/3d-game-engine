/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "..\Src\pch.h"
#include "Graphics_System.h"
#include <iostream>


#include "stb_image\stb_image.h"

#include "Renderer/Renderer2D.h"
#include "Renderer\Renderer3D.h"
#include "Platform\OpenGLTexture.h"

#include "Src/Engine/Core/Manager/AssetManager.h"
#include "Src/Engine/Systems/Graphics/Animation/AnimPose.h"
#include "Src/Engine/Systems/Graphics/Compiler/AnimationCompiler.h"

#define TransformComponent COMPONENTNAME::Comp_Transform_Component // this is slow, just for quick prototyping

std::shared_ptr<Texture2D> testTex;
unsigned int texture1;

Rendering_System::Rendering_System() :VBO{ 0 },
VAO{ 0 },
textureColorbuffer{ 0 },
skyboxVBO{ 0 },
skyboxVAO{ 0 },
windowWidth{ 0 },
windowHeight{ 0 },
rbo{ 0 },
quadVBO{ 0 },
quadVAO{ 0 },
p_width{ 0 },
p_posY{ 0 },
p_posX{ 0 },
p_height{ 0 },
planeVBO{ 0 },
planeVAO{ 0 },
gamescreen_posX{ 0 },
gamescreen_posY{ 0 },
framebuffer{ 0 },
floorTexture{ 0 },
fbo_width{ 0 },
fbo_height{ 0 },
cubemapTexture{ 0 }
{
}


void Rendering_System::Init()
{
	RenderCommand::Init();
	Renderer2D::Init();
	Renderer3D::Init();
	stbi_set_flip_vertically_on_load(true);
	CreateFbo();

	m_persCamera.Init(glm::vec3(0, 0, 6));

	glClearColor(0.1f, 0.1f, 0.1f, 0.6f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	cubemapTexture = loadCubemap(faces);

	skyboxShader.Init("shaders/skybox.vert", "shaders/skybox.frag");
	skyboxShader.Bind();
	skyboxShader.SetInt("skybox", 0);

	skyboxShader.unBind();

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);


	lighting.Init("shaders/lighting.vert", "shaders/lighting.frag");

	//floorTexture = DDSCompiler::LoadOwnFormat("brickwall.dds");

	lightPosition.push_back(glm::vec3(-3.0f, 0, 0));
	lightPosition.push_back(glm::vec3(-1.0f, 0, 0));
	lightPosition.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	lightPosition.push_back(glm::vec3(-3.0f, 0.0f, 0.0f));

	lightColors.push_back(glm::vec3(0.25f));
	lightColors.push_back(glm::vec3(0.50f));
	lightColors.push_back(glm::vec3(0.70f));
	lightColors.push_back(glm::vec3(1.0f));


	lighting.Bind();
	lighting.SetInt("texture1", 0);


	//Unbind all vao and program
	glBindVertexArray(0);
	glUseProgram(0);



	//Unbind all vao and program
	glBindVertexArray(0);
	glUseProgram(0);





	trans = &ComponentManager::GetInstance().GetTransformMap();

	//ani.m_shader = Eos::Shader::Create("shaders/testanimation.glsl");

	//billboard = Texture2D::Create("Assets/Textures/Bill_Test/main_menu_title.png");
	bill_shader.Init("shaders/UI.vert", "shaders/UI.frag");

	// Create VAO, VBO and EBO
	m_testVA = VertexArray::Create();
	float vertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f
	};
	std::shared_ptr<VertexBuffer> testVB;
	testVB = VertexBuffer::Create(vertices, sizeof(vertices));
	testVB->SetLayout({
		{ShaderDataType::Float3, "a_Position"},
		{ShaderDataType::Float2, "a_TexCoord"}
		});
	m_testVA->AddVertexBuffer(testVB);

	GLuint squareIndices[6] = { 0, 1, 2, 2, 3, 1 };
	std::shared_ptr<IndexBuffer> squareIB;
	squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(GLuint));
	m_testVA->SetIndexBuffer(squareIB);


	testTex = Texture2D::Create("Assets/Textures/UI/container.png");
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("Assets/Textures/UI/container.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Static meshes
	m_staticMeshShader.Init("shaders/mesh.vert", "shaders/mesh.frag");

	// Animations
	diffuseTexture = createTexturefromfile("Assets/Animations/Surge_split.png");
	m_animationshader = Eos::Shader::Create("shaders/testanimation.glsl");

	// Billboard
	m_billboard_vao = VertexArray::Create();
	float verts[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f
	};
	std::shared_ptr<VertexBuffer> VBO;
	VBO = VertexBuffer::Create(verts, sizeof(verts));
	VBO->SetLayout({
		{ShaderDataType::Float3, "a_Position"}
		});
	m_billboard_vao->AddVertexBuffer(VBO);
	m_billboard_vao->SetIndexBuffer(squareIB);
	m_billboard_shader = Eos::Shader::Create("shaders/billboard.glsl");


	//This will be a map of particles
	//need to init for all particles 

	////
	//particleSystem = std::make_shared<ParticleComponent>();

	//particleSystem->setDimension(Context::GetInstance().scr_width, Context::GetInstance().scr_height);
	//particleSystem->initScene();
	//particleSystem->resize(Context::GetInstance().scr_width, Context::GetInstance().scr_heig*ht)
	//



	Eos::PE_SUBSCRIBE_EVENT
	(
		Eos::ResizeFrameBufferEvent,
		[this](Eos::ResizeFrameBufferEvent& _evn)
		{
			windowWidth = (int32_t)_evn.size.x;
			windowHeight = (int32_t)_evn.size.y;
			gamescreen_posX = (int32_t)_evn.position.x;
			gamescreen_posY = (int32_t)_evn.position.y;
		}
	);

}



void Rendering_System::Update()
{

#ifndef _DEBUG
	//ZoneScopedN("Rendering System")

#endif

		//Inputs::GetInstance().SetCursorPos(50.0, 100.0);

		BindFbo();//TODO:Put this at the first of the rendering System
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	PreUpdate();
	//if (CoreManager::GetInstance().getEngineState() == ENGINESTATE::PLAY)
	//{
	//    m_persCamera.UpdateZoom();
	//    m_persCamera.Update();
	//}

	// Updates camera position and matrices
	// TODO: will have to remove and ideally put inside an event based system

	glm::mat4 projection = m_persCamera.GetProjectionMatrix();
	glm::mat4 view = m_persCamera.GetViewMaxtrix(); // remove translation from the view matrix


	this->BindFbo();
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	//Normal pipeline
	{
		for (auto mesh = meshes.begin(); mesh != meshes.end(); mesh++)
		{
			if (GET_ENTITY_MANAGER.getActive(mesh->first))
			{

				std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(mesh->first);
				mesh->second.get()->m_shader.Bind();
				mesh->second.get()->m_shader.SetMat4("projection", projection);
				mesh->second.get()->m_shader.SetMat4("view", view);

				glm::mat4 cubeModel = tc->GetModelMatrix();


				mesh->second.get()->m_shader.SetMat4("model", cubeModel);
				mesh->second.get()->m_model.Draw(mesh->second.get()->m_shader);
				mesh->second.get()->m_shader.unBind();

				glBindVertexArray(0);
			}
		}


		this->BindFbo();
	}
	if (normal)
	{

		for (auto mesh = meshes.begin(); mesh != meshes.end(); mesh++)
		{
			if (GET_ENTITY_MANAGER.getActive(mesh->first))
			{
				//std::cout << "Draw mesh" << std::endl;
				std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(mesh->first);
				mesh->second.get()->m_shader.Bind();
				mesh->second.get()->m_shader.SetMat4("projection", projection);
				mesh->second.get()->m_shader.SetMat4("view", view);
				glm::mat4 cubeModel = tc->GetModelMatrix();

				mesh->second.get()->m_shader.SetMat4("model", cubeModel);
				mesh->second.get()->m_model.Draw(mesh->second.get()->m_shader);
				mesh->second.get()->m_shader.unBind();

				glBindVertexArray(0);
			}
		}
	}
	else  if (blinnPhong)//TODO Blinn
	{

		for (auto mesh = meshes.begin(); mesh != meshes.end(); mesh++)
		{
			if (GET_ENTITY_MANAGER.getActive(mesh->first))
			{
				//std::cout << "Draw mesh" << std::endl;
				std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(mesh->first);
				mesh->second.get()->m_shader.Bind();


				mesh->second.get()->m_shader.SetInt("diffuseMap", 0);
				mesh->second.get()->m_shader.SetInt("normalMap", 1);



				glm::mat4 proj = m_persCamera.GetProjectionMatrix();
				glm::mat4 _view = m_persCamera.GetViewMaxtrix();
				mesh->second.get()->m_shader.Bind();
				mesh->second.get()->m_shader.SetMat4("projection", projection);
				mesh->second.get()->m_shader.SetMat4("view", _view);

				glm::mat4 cubeModel = tc->GetModelMatrix();
				mesh->second.get()->m_shader.SetMat4("model", cubeModel);
				mesh->second.get()->m_shader.SetVec3("viewPos", m_persCamera.GetPosition());
				mesh->second.get()->m_shader.SetVec3("lightPos", lightPos);
				glActiveTexture(GL_TEXTURE0);

				glBindTexture(GL_TEXTURE_2D, diffuseMap);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, normalMap);

				mesh->second.get()->m_model.Draw(mesh->second.get()->m_shader);


				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, lightPos);
				model = glm::scale(model, glm::vec3(0.1f));
				mesh->second.get()->m_shader.SetMat4("model", model);
				mesh->second.get()->m_model.Draw(mesh->second.get()->m_shader);
				mesh->second.get()->m_shader.unBind();


				glBindVertexArray(0);
			}
		}
	}
	else if (shadow)//Shadow
	{
		//depth_only.Bind();
		//depth_only.SetInt("diffuseTexture", 0);
		//depth_only.SetInt("shadowMap", 1);
		//
		//glm::mat4 lightProjection, lightView;
		//glm::mat4 lightSpaceMatrix;
		//float near_plane = 1.0f, far_plane = 7.5f;
		//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		//lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		//lightSpaceMatrix = lightProjection * lightView;
		//simpleDepth.Bind();
		//simpleDepth.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		//glViewport(0, 0, shadow_width, shadow_height);
		//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		//glClear(GL_DEPTH_BUFFER_BIT);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, floorTexture);
		//
		//
		////Do Render SCene
		//for (auto mesh = meshes.begin(); mesh != meshes.end(); mesh++)
		//{
		//	std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(mesh->first);
		//	mesh->second.get()->m_shader.Bind();
		//	glm::mat4 cubeModel = tc->GetModelMatrix();
		//	mesh->second.get()->m_shader.SetInt("depthMap", 0);
		//	mesh->second.get()->m_shader.SetFloat("near_plane", near_plane);
		//	mesh->second.get()->m_shader.SetFloat("far_plane", far_plane);
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, depthMap);
		//
		//	mesh->second.get()->m_shader.SetMat4("model", cubeModel);
		//	mesh->second.get()->m_model.Draw(mesh->second.get()->m_shader);
		//	mesh->second.get()->m_shader.unBind();
		//
		//}
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glViewport(0, 0, scr_width, scr_height);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//depth_only.Bind();
		//glm::mat4 sproj = m_persCamera.GetProjectionMatrix();
		//glm::mat4 sview = m_persCamera.GetViewMaxtrix();
		//depth_only.SetMat4("projection", sproj);
		//depth_only.SetMat4("view", sview);
		//depth_only.SetVec3("viewPos", m_persCamera.GetPosition());
		//depth_only.SetVec3("lightPos", lightPos);
		//depth_only.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, floorTexture);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, depthMap);
		//
		////Render scene shader

	}
	else
	{

	}


	//do skybox

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)

	//Generate Lights Data, pass as one uniform 
	lightPosition.clear(); //Extremely bad coding , no optimz 
	lightColors.clear();
	for (auto lits = lights.begin(); lits != lights.end(); lits++)
	{
		if (GET_ENTITY_MANAGER.getActive(lits->first))
		{
			std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(lits->first);
			lightColors.push_back(lits->second.get()->Color);
			lightPosition.push_back(tc->Translation);
		}
	}

	{
		//Remove the Plane
		//if (lights.size() != 0)
		//{
		//	lighting.Bind();
		//	lighting.SetMat4("projection", projection);
		//	lighting.SetMat4("view", view);
		//	glUniform3fv(glGetUniformLocation(lighting.id, "lightPositions"), (GLsizei)lights.size(), &lightPosition[0][0]);
		//	glUniform3fv(glGetUniformLocation(lighting.id, "lightColors"), (GLsizei)lights.size(), &lightColors[0][0]);
		//	lighting.SetVec3("viewPos", m_persCamera.GetPosition());
		//	lighting.SetInt("gamma", gamma);
		//
		//
		//	glBindVertexArray(planeVAO);
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, floorTexture);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}

	}


	// Draw meshes
	for (auto mesh = meshes.begin(); mesh != meshes.end(); mesh++)
	{
		if (GET_ENTITY_MANAGER.getActive(mesh->first))
		{
			std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(mesh->first);
			mesh->second.get()->m_shader.Bind();
			mesh->second.get()->m_shader.SetMat4("projection", projection);
			mesh->second.get()->m_shader.SetMat4("view", view);

			glm::mat4 cubeModel = tc->GetModelMatrix();

			mesh->second.get()->m_shader.SetMat4("model", cubeModel);
			mesh->second.get()->m_model.Draw(mesh->second.get()->m_shader);
			mesh->second.get()->m_shader.unBind();

			glBindVertexArray(0);

		}
	}


	//Render depth of scene to texture 
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	simpleDepthShader.Bind();
	simpleDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	//Should i bind texture here? bind woodtexture? what texture should i be binding?

	// Draw static meshes
	for (auto stat_mesh = static_meshes.begin(); stat_mesh != static_meshes.end(); stat_mesh++)
	{
		if (GET_ENTITY_MANAGER.getActive(stat_mesh->first))
		{
			// Check for nullptr
			if (!stat_mesh->second->meshptr)
				continue;

			std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(stat_mesh->first);
			glm::mat4 cubeModel = tc->GetModelMatrix();

			//stat_mesh->second.get()->m_shader.Bind();
			//stat_mesh->second.get()->m_shader.SetMat4("projection", projection);
			//stat_mesh->second.get()->m_shader.SetMat4("view", view);
			//stat_mesh->second.get()->m_shader.SetMat4("model", cubeModel);
			simpleDepthShader.Bind();
			simpleDepthShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
			simpleDepthShader.SetMat4("model", cubeModel);
			// Lights
			//glUniform3fv(glGetUniformLocation(stat_mesh->second.get()->m_shader.id, "lightPositions"), (GLsizei)lights.size(), &lightPosition[0][0]);
			//glUniform3fv(glGetUniformLocation(stat_mesh->second.get()->m_shader.id, "lightColors"), (GLsizei)lights.size(), &lightColors[0][0]);
			//stat_mesh->second.get()->m_shader.SetVec3("viewPos", m_persCamera.GetPosition());

			//stat_mesh->second.get()->m_model.Draw(simpleDepthShader);
			stat_mesh->second->meshptr->Draw(simpleDepthShader);

			m_staticMeshShader.unBind();

			glBindVertexArray(0);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	screenShader.Bind();

	//reset viewport
	glViewport(0, 0, Eos::Context::GetInstance().scr_width, Eos::Context::GetInstance().scr_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2. render scene as normal using the generated depth/shadow map  
	//         glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glViewport(0, 0, Eos::Context::GetInstance().scr_width, Eos::Context::GetInstance().scr_height);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//bind the one with shadows and phong lighting one

	// Draw static meshes
	for (auto stat_mesh = static_meshes.begin(); stat_mesh != static_meshes.end(); stat_mesh++)
	{
		if (GET_ENTITY_MANAGER.getActive(stat_mesh->first))
		{
			if (stat_mesh->second->directory == "")
				continue;

			// Set mesh pointer
			std::string test = FileManager::GetFileNameWithoutExtension(stat_mesh->second->directory);
			stat_mesh->second->meshptr = &Eos::AssetManager::GetInstance().GetStaticMesh(test.c_str());

			if (!stat_mesh->second->meshptr)
				continue;
			

			//shader configuration
			std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(stat_mesh->first);
			glm::mat4 cubeModel = tc->GetModelMatrix();
			
			m_staticMeshShader.Bind();
			m_staticMeshShader.SetMat4("projection", projection);
			m_staticMeshShader.SetMat4("view", view);
			m_staticMeshShader.SetMat4("model", cubeModel);

			// Lights
			glUniform3fv(glGetUniformLocation(m_staticMeshShader.id, "lightPositions"), (GLsizei)lights.size(), &lightPosition[0][0]);
			glUniform3fv(glGetUniformLocation(m_staticMeshShader.id, "lightColors"), (GLsizei)lights.size(), &lightColors[0][0]);
			m_staticMeshShader.SetVec3("viewPos", m_persCamera.GetPosition());
			m_staticMeshShader.SetVec3("lightPos", shadow_lightPos);
			m_staticMeshShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

			//Do Linear
			m_staticMeshShader.SetBool("gamma", Context::GetInstance().isGammaOn);
			//stat_mesh->second.get()->m_model.DrawShadow(m_staticMeshShader, depthMap);
			stat_mesh->second->meshptr->DrawShadow(m_staticMeshShader, depthMap);
			
			m_staticMeshShader.unBind();

			glBindVertexArray(0);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Draw Dynamic Meshes
	for (auto dy_mesh = dyna_meshes.begin(); dy_mesh != dyna_meshes.end(); dy_mesh++)
	{
		if (GET_ENTITY_MANAGER.getActive(dy_mesh->first))
		{
			{
				// Set pointers
				dy_mesh->second->mesh = &Eos::AssetManager::GetInstance().GetMesh(dy_mesh->second->meshDirectory.c_str());
				dy_mesh->second->skeleton = &Eos::AssetManager::GetInstance().GetSkeleton(dy_mesh->second->meshDirectory.c_str());
				dy_mesh->second->animTrack = &Eos::AssetManager::GetInstance().GetAnimation(dy_mesh->second->meshDirectory.c_str());
				
				// Check for nullptr
				if (!dy_mesh->second->mesh || !dy_mesh->second->skeleton || !dy_mesh->second->animTrack)
					continue;

				// Check for a state change
				if (dy_mesh->second->current_State != dy_mesh->second->previous_state)
				{
					//std::cout << "State changed" << std::endl;

					// Cycle through saved animation list and change state accordingly
					for (auto animEntry : dy_mesh->second->list_animations)
					{
						//std::cout << "Looking for new state: " << dy_mesh->second->current_State  << " " << animEntry.stateName << std::endl;
						// State name matched; change state
						if (dy_mesh->second->current_State == animEntry.stateName)
						{
							//std::cout << "New found " << animEntry.animTrackName << std::endl;
							dy_mesh->second->animTrack = &Eos::AssetManager::GetInstance().GetAnimation(animEntry.animTrackName.c_str());

							// Because data dont hold the same mesh; target mesh and skeleton as well
							dy_mesh->second->mesh = &Eos::AssetManager::GetInstance().GetMesh(animEntry.animTrackName.c_str());
							dy_mesh->second->skeleton = &Eos::AssetManager::GetInstance().GetSkeleton(animEntry.animTrackName.c_str());

							// Reset internal clock and set current state to previous state

							dy_mesh->second->internalTime = 0.1f;
							//std::cout << 
							dy_mesh->second->previous_state = dy_mesh->second->current_State;

							dy_mesh->second->currentplayingMode = animEntry.mode;

							dy_mesh->second->meshDirectory = animEntry.animTrackName;
						}
					}
				}

				

				// Check if attaching animation track failed
				if (!dy_mesh->second->animTrack)
					continue;

				// Check if reattaching skeleton and mesh failed
				if (!dy_mesh->second->mesh || !dy_mesh->second->skeleton)
					continue;

				// Check currentPose size
				if (dy_mesh->second->currentPose.size() != dy_mesh->second->skeleton->boneCount)
					dy_mesh->second->currentPose.resize(dy_mesh->second->skeleton->boneCount, dy_mesh->second->identity);

				// Check if the mesh has completed playing
				// If completed then dont increment time
				if (!dy_mesh->second->isCompleted)
				{
					dy_mesh->second->internalTime += (Eos::FrameRateManager::GetInstance().delta() / 2.5f);
				}

				//std::cout << dy_mesh->second->internalTime << std::endl;

				// Set the mesh in the proper pose
				getPose(*dy_mesh->second->animTrack,
					dy_mesh->second->skeleton->skeleton,
					dy_mesh->second->internalTime,
					dy_mesh->second->currentPose,
					dy_mesh->second->identity,
					dy_mesh->second->mesh->globalInvTransform);

				// Set orientated upright
				std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(dy_mesh->first);
				glm::mat4 model = tc->GetModelMatrix();
				glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)glm::radians(-90.f), { 1.0f, 0.0f, 0.0f });
				model = model * rot;

				m_animationshader->Bind();
				m_animationshader->SetMat4("view_projection_matrix", projection* view);
				m_animationshader->SetMat4("model_matrix", model);
				m_animationshader->SetMat4("bone_transforms", dy_mesh->second->currentPose[0], dy_mesh->second->skeleton->boneCount);
				m_animationshader->SetBool("gamma", Context::GetInstance().isGammaOn);

				glBindVertexArray(dy_mesh->second->mesh->vao);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, diffuseTexture);
				m_animationshader->SetInt("diff_texture", 0);

				glDrawElements(GL_TRIANGLES, (GLsizei)dy_mesh->second->mesh->indices.size(), GL_UNSIGNED_INT, 0);

				glUseProgram(0);
				glBindVertexArray(0);

				// Check completed status
				if (dy_mesh->second->currentplayingMode == AnimationWrapMode::Single)
				{
					if (dy_mesh->second->internalTime >= (dy_mesh->second->animTrack->duration - 1.f))
					{
						dy_mesh->second->isCompleted = true;
					}
				}
			}


			// Save and compare prev and current state, enum comparison should be ok
			//if (dy_mesh->second->currentState != dy_mesh->second->prevState)
			//{
			//	//std::cout << "Change of state detected" << std::endl;
			//	dy_mesh->second->internalTime = 0.0f;
			//	dy_mesh->second->prevState = dy_mesh->second->currentState;
			//}

			//// Second version - Adding detection of when animation clip has completed playing
			//if (!dy_mesh->second->isCompleted)
			//{
			//	//std::cout << "incrementing" << std::endl;
			//	dy_mesh->second->internalTime += 0.5f;
			//}

			//AnimationStates state = dy_mesh->second->GetState();
			//AnimationWrapMode mode = dy_mesh->second->aniClips[state].first;
			//AnimationCompiler::Ex_Info* anim = &dy_mesh->second->aniClips[state].second;
			//

			//// Check 
			//if (mode == AnimationWrapMode::Single)
			//{
			//	//std::cout << dy_mesh->second->internalTime << ", " << dy_mesh->second->aniClips[state].second.animtionClip.duration << std::endl;
			//	if (dy_mesh->second->internalTime >= (dy_mesh->second->aniClips[state].second.animtionClip.duration - 1.f)) // Clip completed
			//	{
			//		dy_mesh->second->isCompleted = true;
			//	}
			//}


			// First version
			//if (dy_mesh->second->isLoaded)
			//{
			//	if (dy_mesh->second->isAniPaused == false) // not paused; add dt
			//	{
			//		elapsedTime += 0.5f;
			//	}

			//	AnimationStates state = dy_mesh->second->GetState();
			//	AnimationCompiler::Ex_Info* anim = &dy_mesh->second->listOfAnimations[state];

			//	// Animationclip is of type AnimationTrack 
			//	getPose(anim->animtionClip,
			//		anim->skeleton,
			//		elapsedTime,
			//		dy_mesh->second->currentPose,
			//		dy_mesh->second->identity,
			//		anim->globalInvTransform);

			//	// Rendering
			//	std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(dy_mesh->first);
			//	glm::mat4 model = tc->GetModelMatrix();

			//	// Set the orientation upright
			//	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)glm::radians(-90.f), { 1.0f, 0.0f, 0.0f });
			//	model = model * rot;

			//	dy_mesh->second->m_shader->Bind();
			//	dy_mesh->second->m_shader->SetMat4("view_projection_matrix", projection * view);
			//	dy_mesh->second->m_shader->SetMat4("model_matrix", model);
			//	dy_mesh->second->m_shader->SetMat4("bone_transforms", dy_mesh->second->currentPose[0], anim->boneCount);

			//	glBindVertexArray(dy_mesh->second->vao);
			//	glActiveTexture(GL_TEXTURE0);
			//	glBindTexture(GL_TEXTURE_2D, dy_mesh->second->diffuseTexture);
			//	dy_mesh->second->m_shader->SetInt("diff_texture", 0);

			//	glDrawElements(GL_TRIANGLES, (GLsizei)anim->outputIdxs.size(), GL_UNSIGNED_INT, 0);

			//	glUseProgram(0);
			//	glBindVertexArray(0);
			//}
		}
	}


	for (auto camera = cam.begin(); camera != cam.end(); camera++)
	{
		if (GET_ENTITY_MANAGER.getActive(camera->first))
		{
			if (camera->second->GetActive() && !m_persCamera.m_active)
			{
				glm::vec3 pos = camera->second->GetPosition();
				glm::vec3 tar = camera->second->GetTargetPosition();
				glm::vec3 rot = camera->second->GetTargetRotation();

				m_persCamera.cameraPos = pos;

				m_persCamera.RecalculateViewMatrix(pos, tar);
			}
		}
	}

	// Fonts
	for (auto tx = textAndFonts.begin(); tx != textAndFonts.end(); ++tx)
	{
		tx->second->fontshader->Bind();
		tx->second->_vao->Bind();

		std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(tx->first);
		glm::mat4 modelMat = tc->GetModelMatrix();

		tx->second->fontshader->SetInt("u_Texture", 0);
		tx->second->fontshader->SetMat4("u_ViewProjection", projection * view);
		tx->second->fontshader->SetMat4("u_Transform", modelMat);

		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);

		glBindTexture(GL_TEXTURE_2D, tx->second->compiler.texHandleOut);
		//testTex->Bind();
		//tx->second->fontshader->SetInt("u_Texture", 0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		tx->second->_vao->Unbind();
		tx->second->fontshader->unBind();

		//Renderer2D
	}


	// Need to get reference to the internal map
	std::shared_ptr<Physics_System>  ps = std::dynamic_pointer_cast<Physics_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_PHYSICS));
	auto physics_map = ps->getComponentMap();

	// Render
	if (this->DrawDebug)
	{
		Renderer3D::BeginScene(m_persCamera);
		for (auto box = physics_map.begin(); box != physics_map.end(); ++box)
		{
			if (GET_ENTITY_MANAGER.getActive(box->first))
			{
				for (auto tc : GET_TRANSFORM_MAP)
				{
					// Check entityID
					if (box->first == tc.first)
					{
						Renderer3D::DrawDebugBox(tc.second->Translation, box->second->pc_aabbScale, box->second->pc_aabbOffset);
					}
				}
			}
		}
		Renderer3D::EndScene();
	}








	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.Bind();

	glm::mat4 sview = glm::mat4(1.0);
	sview = glm::mat4(glm::mat3(view));

	glm::mat4 rot{ 1.0f };
	for (auto sb = movingSkyboxes.begin(); sb != movingSkyboxes.end(); ++sb)
	{
		float RotationValue = sb->second->GetRotSpeed() * glfwGetTime() * 0.1f;
		rot = glm::rotate(glm::mat4(1.0f), RotationValue, sb->second->GetRotDir());
	}

	sview = rot * sview;

	//sview = glm::mat3(m_persCamera.GetViewMaxtrix()); //remove translation
	skyboxShader.SetMat4("view", sview);
	skyboxShader.SetMat4("projection", projection);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default


	//font.SetProjection(m_persCamera.GetViewProjectionMatrix());
	//font.RenderWithAtlas("aLoB", { 2.0,1.0 }, 1.0f);



	// Rendering billboard objects
	for (auto bb = billboards.begin(); bb != billboards.end(); bb++)
	{
		if (GET_ENTITY_MANAGER.getActive(bb->first))
		{
			m_billboard_shader->Bind();
			m_billboard_vao->Bind();

			// Extract out texture or color
			// bind texture

			/*if (bb->second->billboard_texture)
				glBindTexture(GL_TEXTURE_2D, bb->second->billboard_texture->GetID());*/

			if (!bb->second->textureDirectory.empty() && (bb->second->isSet == true))
			{
				std::shared_ptr<Texture2D> tex = Eos::AssetManager::GetInstance().GetTexture(bb->second->textureDirectory.c_str());
				if (!tex)
				{
					bb->second->isSet = false;
				}
				else
				{
					//std::cout << tex->GetID() << std::endl;
					glBindTexture(GL_TEXTURE_2D, tex->GetID());
				}			
			}

			glm::mat4 viewproj = m_persCamera.GetOrthoMatrix();     // no view to lock location
			// Set uniforms
			m_billboard_shader->SetMat4("u_ViewProjection", projection * view);
			m_billboard_shader->SetVec4("u_Color", { 1.0f, 0.0f, 0.0f, 1.0f });

			// transform consists of pos, scale and rotate
			std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(bb->first);
			m_billboard_shader->SetMat4("u_Transform", tc->GetModelMatrix());

			// Create vec3 and vec2 to pass into uniform locations
			//glm::mat4 tview = glm::inverse(glm::mat3(view));        // remove rotation
			glm::mat4 tview = glm::mat3(view);
			glm::vec3 u_camRight = glm::vec3(tview[0][0], tview[1][0], tview[2][0]);
			glm::vec3 u_camUp = glm::vec3(tview[0][1], tview[1][1], tview[2][1]);

			// Pass data into uniforms
			isFixedSize = false;	// control is fixed size or not on shader side

			m_billboard_shader->SetBool("FixedSize", isFixedSize);
			m_billboard_shader->SetVec3("CameraRight_worldspace", u_camRight);
			m_billboard_shader->SetVec3("CameraUp_worldspace", u_camUp);

			m_billboard_shader->SetVec3("BillboardPos", bb->second->posOffset);
			m_billboard_shader->SetVec2("BillboardSize", bb->second->size);

			// Begin drawing
			m_billboard_vao->Bind();
			GLuint count = m_billboard_vao->GetIndexBuffer()->GetCount();
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

			// After drawing
			m_billboard_shader->unBind();
			glBindVertexArray(0);

			//texture - either asset manager or something else
		}
	}

	////Jeroen you just need to call this to update particles
	glDepthFunc(GL_LESS); // set depth function back to defaults
	float delT = static_cast<float>(glfwGetTime());//before for loop

	for (auto particle = particles.begin(); particle != particles.end(); particle++)
	{
		if (GET_ENTITY_MANAGER.getActive(particle->first))
		{

			//Need to get Transform from the TransformMap
			////for range loop
			std::shared_ptr<Transform_Component> transform =
				ComponentManager::GetInstance().GetTransformComponent(particle->first);

			//Once i have the transform
			glm::mat4 particlesLTCMatrix = transform->GetModelMatrix();



			particle->second->PassInUniforms(view, projection, particlesLTCMatrix);
			particle->second->update(delT);
			particle->second->render();
			glBindVertexArray(0);
		}
	}
	glDepthFunc(GL_LESS); // set depth function back to default //after for loop


	//float delT = static_cast<float>(glfwGetTime());//before for loop
	delT = static_cast<float>(glfwGetTime());
	for (auto smokeParticle = SmokeParticles.begin(); smokeParticle != SmokeParticles.end(); smokeParticle++)
	{
		if (GET_ENTITY_MANAGER.getActive(smokeParticle->first))
		{

			//Need to get Transform from the TransformMap
			////for range loop
			std::shared_ptr<Transform_Component> transform =
				ComponentManager::GetInstance().GetTransformComponent(smokeParticle->first);

			//Once i have the transform
			glm::mat4 particlesLTCMatrix = transform->GetModelMatrix();

			smokeParticle->second->PassInUniforms(view, projection, particlesLTCMatrix);
			smokeParticle->second->update(delT);
			smokeParticle->second->render();
			glBindVertexArray(0);
		}
	}
	glDepthFunc(GL_LESS); // set depth function back to defaults



	//Render last - according to edwin
	for (auto gui = OpenGLGuis.begin(); gui != OpenGLGuis.end(); gui++)
	{
		if (GET_ENTITY_MANAGER.getActive(gui->first))
		{
			//std::cout << gui->second->filePath << std::endl;

			std::shared_ptr<Transform_Component> transform =
				ComponentManager::GetInstance().GetTransformComponent(gui->first);

			glm::mat4 UI_LocalToWorldMatrix = transform->GetModelMatrix();
			gui->second->m_GUI.UpdateTransform(UI_LocalToWorldMatrix);
			gui->second->m_GUI.UpdateOpacity(gui->second->m_Opacity);
			gui->second->m_GUI.UpdateColor(gui->second->inColor);
			gui->second->m_GUI.UpdateDepth(transform.get()->Translation.z);
			gui->second->m_GUI.UpdateFilePath(gui->second->filePath);
			gui->second->m_GUI.UpdateGamma(Context::GetInstance().isGammaOn);

			gui->second->m_GUI.Render();
		}

	}
	//SetupToSendFBO is sending it to GameScreenWindow to render 
	//the Scene into the GameScene ImGuiWindow


	SetupToSendFbo(); //TODO: Always put SetupToSendFbo Last
	Eos::SendFrameBufferObjectEvent framebufferEvents{ textureColorbuffer };
	Eos::PE_PUBLISH_EVENT(framebufferEvents);
	ResizeFbo();





#ifndef _DEBUG
	//FrameMark
#endif
}

void Rendering_System::Exit()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	Renderer2D::ShutDown();
	Renderer3D::ShutDown();
	font.Exit();
}

const std::map<EntityID, std::shared_ptr<Mesh_Component>>& Rendering_System::GetMeshesMap()
{
	return meshes;
}

const std::map<EntityID, std::shared_ptr<Static_Mesh_Component>>& Rendering_System::GetStaticMeshMap()
{
	return static_meshes;
}

void Rendering_System::AddingNewComponent(EntityID entityID, COMPONENTNAME compName, std::shared_ptr<IComponent> compPtr)
{
	if (compName == COMPONENTNAME::Comp_Mesh_Component)
	{
		if (!meshes.contains(entityID))
		{
			meshes.insert({ entityID, std::dynamic_pointer_cast<Mesh_Component>(compPtr) });
		}
	}
	else if (compName == COMPONENTNAME::Comp_Light_Component)
	{
		if (!lights.contains(entityID))
		{
			lights.insert({ entityID, std::dynamic_pointer_cast<Light_Component>(compPtr) });
		}
	}
	else if (compName == COMPONENTNAME::Comp_Animated_Mesh_Component)
	{
		if (!dyna_meshes.contains(entityID))
		{
			dyna_meshes.insert({ entityID, std::dynamic_pointer_cast<Animated_Mesh_Component>(compPtr) });
		}
	}
	else if (compName == COMPONENTNAME::Comp_Static_Mesh_Component)
	{
		if (!static_meshes.contains(entityID))
		{
			static_meshes.insert({ entityID, std::dynamic_pointer_cast<Static_Mesh_Component>(compPtr) });
		}
	}
	else if (compName == COMPONENTNAME::Comp_Camera_Component)
	{
		if (!cam.contains(entityID))
		{
			cam.insert({ entityID, std::dynamic_pointer_cast<Camera_Component>(compPtr) });
		}
	}
	else if (compName == COMPONENTNAME::Comp_GUI_Component)
	{
		if (!OpenGLGuis.contains(entityID))
		{
			OpenGLGuis.insert({ entityID, std::dynamic_pointer_cast<GUI_Component>(compPtr) });
		}
	}
	else if (compName == COMPONENTNAME::Comp_Font_Component)
	{
		if (!textAndFonts.contains(entityID))
		{
			textAndFonts.insert({ entityID, std::dynamic_pointer_cast<Font_Component>(compPtr) });
		}
	}
	else if (compName == COMPONENTNAME::Comp_Skybox_Component)
	{
		if (!movingSkyboxes.contains(entityID))
		{
			movingSkyboxes.insert({ entityID, std::dynamic_pointer_cast<Skybox_Component>(compPtr) });
		}
	}
	else if (compName == COMPONENTNAME::Comp_Particle_Component)
	{
		if (!particles.contains(entityID))
		{
			particles.insert({ entityID, std::dynamic_pointer_cast<Particle_Component>(compPtr) });
		}
	}
	else if (compName == COMPONENTNAME::Comp_Smoke_Component)
	{
		if (!particles.contains(entityID))
		{
			SmokeParticles.insert({ entityID, std::dynamic_pointer_cast<Smoke_Component>(compPtr) });
		}
	}
	else if (compName == COMPONENTNAME::Comp_BillBoard_Component)
	{
		if (!billboards.contains(entityID))
		{
			billboards.insert({ entityID, std::dynamic_pointer_cast<BillBoard_Component>(compPtr) });
		}
	}
}

void Rendering_System::RemovingComponent(EntityID entityID, COMPONENTNAME compName)
{
	if (compName == COMPONENTNAME::Comp_Light_Component)
	{
		lights.erase(entityID);
	}
	else if (compName == COMPONENTNAME::Comp_Mesh_Component)
	{
		meshes.erase(entityID);
	}
	else if (compName == COMPONENTNAME::Comp_Static_Mesh_Component)
	{
		static_meshes.erase(entityID);
	}
	else if (compName == COMPONENTNAME::Comp_Static_Mesh_Component)
	{
		dyna_meshes.erase(entityID);
	}
	else if (compName == COMPONENTNAME::Comp_Camera_Component)
	{
		cam.erase(entityID);
	}
	else if (compName == COMPONENTNAME::Comp_GUI_Component)
	{
		OpenGLGuis.erase(entityID);
	}
	else if (compName == COMPONENTNAME::Comp_Font_Component)
	{
		textAndFonts.erase(entityID);
	}
	else if (compName == COMPONENTNAME::Comp_Skybox_Component)
	{
		movingSkyboxes.erase(entityID);
	}
	else if (compName == COMPONENTNAME::Comp_Particle_Component)
	{
		particles.erase(entityID);
	}
	else if (compName == COMPONENTNAME::Comp_Smoke_Component)
	{
		SmokeParticles.erase(entityID);
	}
	else if (compName == COMPONENTNAME::Comp_BillBoard_Component)
	{
		billboards.erase(entityID);
	}
}

void Rendering_System::ClearAllComponents()
{
	lights.clear();
	meshes.clear();
	static_meshes.clear();
	dyna_meshes.clear();
	cam.clear();
	OpenGLGuis.clear();
	textAndFonts.clear();
	movingSkyboxes.clear();
	particles.clear();
	SmokeParticles.clear();
	billboards.clear();
}

void Rendering_System::CreateFbo()
{

	glUseProgram(0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	screenShader.Init("shaders/fbo.vert", "shaders/fbo.frag");

	//phong lighting with shadow depth map
	shadow_mapping_vs.Init("shaders/3.1.3.shadow_mapping.vert", "shaders/3.1.3.shadow_mapping.frag");

	//some shader with fragment to return nothing( frag is commented out)
	//need send lightSpaceMatrix and the model for simpledepth shader
	simpleDepthShader.Init("shaders/3.1.3.shadow_mapping_depth.vert", "shaders/3.1.3.shadow_mapping_depth.frag");

	//if i want to render the depthmap to a framebuffer i can use this
	debugDepthQuad.Init("shaders/3.1.3.debug_quad.vs", "shaders/3.1.3.debug_quad_depth.fs");

	glGenVertexArrays(1, &quadVAO); //we can do better
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	screenShader.Bind();
	screenShader.SetInt("screenTexture", 0);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	fbo_width = mode->width;
	fbo_height = mode->height;

	glGenFramebuffers(1, &depthMap);
	glGenTextures(1, &depthMap); //create depth texture
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthMap, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//Shader configurations
	shadow_mapping_vs.Bind();
	shadow_mapping_vs.SetInt("diffuseTexture", 0);
	shadow_mapping_vs.SetInt("shadowMap", 1);
	debugDepthQuad.Bind();
	debugDepthQuad.SetInt("depthMap", 0);


	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbo_width, fbo_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbo_width, fbo_height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it



	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		spdlog::warn("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		//std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Edwin TODO
	shadow_lightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
}

void Rendering_System::BindFbo()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


	glViewport(0, 0,
		Eos::Context::GetInstance().scr_width, Eos::Context::GetInstance().scr_height);
	glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Rendering_System::SetupToSendFbo()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, static_cast<GLint>(Eos::Context::GetInstance().scr_width),
		static_cast<GLint>(Eos::Context::GetInstance().scr_height),
		0, 0, static_cast<GLint>(Eos::Context::GetInstance().scr_width), static_cast<GLint>(Eos::Context::GetInstance().scr_height),
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
		GL_NEAREST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screenShader.Bind();
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Rendering_System::ResizeFbo()
{
	if (windowWidth != p_width || windowHeight != p_height) //TODO: this should be set to change
	{
		glDeleteFramebuffers(1, &framebuffer);
		glDeleteTextures(1, &textureColorbuffer);

		p_width = windowWidth;
		p_height = windowHeight;
		glViewport(0, 0, windowWidth, windowHeight);

		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbo_width, fbo_height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
}

void Rendering_System::CreateUISceneFBO()
{
	glUseProgram(0); //Unbind program
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	//Load the Scene Shaders
	UISceneShader.Init("shaders/fbo.vert", "shaders/fbo.frag");

	//Create Layout for FBO VAO
	glGenVertexArrays(1, &sceneVao); //we can do better
	glGenBuffers(1, &sceneVBO);
	glBindVertexArray(sceneVao);
	glBindBuffer(GL_ARRAY_BUFFER, sceneVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	UISceneShader.Bind();
	UISceneShader.SetInt("screenTexture", 0);

	//Take the largest resolutions for the current monitor
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	fbo_width = mode->width;
	fbo_height = mode->height;


	glGenFramebuffers(1, &UISceneFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, UISceneFrameBuffer);

	glGenTextures(1, &UISceneColorBuffer);
	glBindTexture(GL_TEXTURE_2D, UISceneColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbo_width, fbo_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, UISceneColorBuffer, 0);

	//Create Depth map for the FrameBuffer object
	glGenRenderbuffers(1, &UISceneRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, UISceneRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbo_width, fbo_height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, UISceneRBO); // now actually attach it

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer is not completed ! " << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Rendering_System::BindUISceneFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, UISceneFrameBuffer);
	glEnable(GL_DEPTH_TEST);
}

void Rendering_System::ResizeSceneFBO()
{
}

void Rendering_System::SetUpSceneFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, static_cast<GLint>(Eos::Context::GetInstance().scr_width),
		static_cast<GLint>(Eos::Context::GetInstance().scr_height),
		0, 0, static_cast<GLint>(Eos::Context::GetInstance().scr_width), static_cast<GLint>(Eos::Context::GetInstance().scr_height),
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
		GL_NEAREST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	UISceneShader.Bind();
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, UISceneColorBuffer);
}

void Rendering_System::PreUpdate()
{
	//Poll events by Context Manager
	//
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Rendering_System::PostUpdate()
{
}

#include "AnimationWindow.h"
#include "Src\pch.h"

#include "Src/Engine/Systems/Graphics/Animation/AnimPose.h"
#include "Src\Engine\Systems\Graphics\Shader.h"
#include "Src/Engine/Systems/Graphics/Platform/OpenGLVertexArray.h"
#include "Src/Engine/Systems/Graphics/Platform/OpenGLTexture.h"
#include "Src/Inputs/Inputs.h"

#include "Src/Engine/Core/Manager/AssetManager.h"

#include <ImGuizmo/ImSequencer.h>
#include "Src/ImGui/Sequencer.h"
#include "Src\Events\Events.h"
#include "Src\Events\EventDispatcher.h"
#include "Src\Events\EventObserver.h"


namespace Eos
{
	std::shared_ptr<VertexArray> my_vao;
	std::shared_ptr<Eos::Shader> my_shader;
	std::shared_ptr<Texture2D> my_testTex;

	AnimationWindow::AnimationWindow() :
		localMesh{nullptr},
		localSkeleton{nullptr},
		localTrack{nullptr},
		localTimer{0.1f}
	{
		//id = ImGui::GetID()

		//mysequence = new Sequence();
		mysequence = std::make_unique<Sequence>();
		mysequence->mFrameMin = 0;
		mysequence->mFrameMax = 100;

		// 
		mysequence->Items.push_back(Sequence::SequenceItem{ 0, 10, 30, false });
		mysequence->Items.push_back(Sequence::SequenceItem{ 1, 20, 30, false });
		mysequence->Items.push_back(Sequence::SequenceItem{ 1, 12, 60, false });
		mysequence->Items.push_back(Sequence::SequenceItem{ 1, 61, 90, false });
		mysequence->Items.push_back(Sequence::SequenceItem{ 1, 90, 99, false });



		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		fbo_width = (float)mode->width;
		fbo_height = (float)mode->height;

		//Creating textures buffers for the framebuffer object
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		//using this as framebufferobject texture
		glGenTextures(1, &textureColorBuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

		//Need the max width and height of the screen
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fbo_width, fbo_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		//Create the mipmap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer,0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbo_width, fbo_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind the framebuffer object

		//
		//FramebufferSpecification fbSpec;
		//fbSpec.width = 800;
		//fbSpec.height = 600;
		//m_Framebuffer = Framebuffer::Create(fbSpec);

		my_vao = VertexArray::Create();
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
		my_vao->AddVertexBuffer(testVB);

		GLuint squareIndices[6] = { 0, 1, 2, 2, 3, 1 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(GLuint));
		my_vao->SetIndexBuffer(squareIB);

		my_testTex = Texture2D::Create("Assets/Textures/UI/container.png");

		//my_shader = Shader::Create("shaders/test.glsl");
		my_shader = Eos::Shader::Create("shaders/testanimation.glsl");
		diffuseTexture = createTexturefromfile("Assets/Animations/Surge_split.png");

		m_camera.Init(glm::vec3(0, 0, 6));

		//Take grid
		prog.compileShader("flat_frag.glsl");
		prog.compileShader("flat_vert.glsl");

		prog.link();
		prog.use();


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);

		prog.use();
		prog.setUniform("Color", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		glUseProgram(0);


		Eos::PE_SUBSCRIBE_EVENT
		(
			Eos::SetAnimationPlayingEvent,
			[this](Eos::SetAnimationPlayingEvent& _evn)
			{
				playingString = _evn.meshName;
			}
		);
	}

	void AnimationWindow::SetAnimationClip(AnimationCompiler::Ex_Info* targetClip)
	{
		clip = targetClip;
	}

	void AnimationWindow::Update()
	{
		// Main window
		ImGui::Begin("Animation Window", &active, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
		{
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 contentRegionSize = ImGui::GetContentRegionAvail();
			windowPos.y += ImGui::GetWindowContentRegionMin().y;

			float width = ImGui::GetContentRegionAvail().x;
			float height = ImGui::GetContentRegionAvail().y;

			if (!playingString.empty())
			{
				localMesh = &Eos::AssetManager::GetInstance().GetMesh(playingString.c_str());
				localSkeleton = &Eos::AssetManager::GetInstance().GetSkeleton(playingString.c_str());
				localTrack = &Eos::AssetManager::GetInstance().GetAnimation(playingString.c_str());
			}
			
			//std::cout << playingString << std::endl;

			// Mini window with rounded border
			{
				if (ImGui::IsWindowHovered() && Eos::Inputs::GetInstance().IsMouseButtonPressed(Eos::Button::Right))
				{
					//rs->m_persCamera.UpdateZoom();
					m_camera.Update();
				}

				//ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				//ImGui::BeginChild("Skeleton", ImVec2(width * 0.25f, height * 0.7f), true);

				//bind the framebuffer 
				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
				glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

				//draw 
				//glClearColor(0.5f, 0.1f, 0.8f, 1.0f);
				my_vao->Bind();
				my_shader->Bind();

				
				// Rendering
				{
					

					GLuint GLuint = 0;
					//lClearBufferuiv(GL_COLOR | GL_DEPTH, framebuffer, &GLuint);

					glCullFace(GL_FRONT);
					glEnable(GL_DEPTH_TEST);
					
					glm::mat4 projection = m_camera.GetProjectionMatrix();
					glm::mat4 view = m_camera.GetViewMaxtrix();

					// Render grid
					prog.use(); 
					prog.setUniform("MV", view);
					prog.setUniform("Proj", projection);
					gridProgram.render();

					

					

					// If null pointer is set then skip rendering
					if (localMesh && localSkeleton && localTrack)
					{
						// Check current pose size
						if (currentPose.size() != localSkeleton->boneCount)
							currentPose.resize(localSkeleton->boneCount, identity);

						// Set the mesh in the proper pose
						getPose(*localTrack,
							localSkeleton->skeleton,
							localTimer,
							currentPose,
							identity,
							localMesh->globalInvTransform);

						// Increment local timer for next rendering
						localTimer += Eos::FrameRateManager::GetInstance().delta();

						// Set orientation upright
						glm::mat4 model{ 1.0f };
						glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)glm::radians(-90.f), { 1.0f, 0.0f, 0.0f });
						model = model * rot;

						// Bind shaders
						my_shader->Bind();
						my_shader->SetMat4("view_projection_matrix", projection * view);
						my_shader->SetMat4("model_matrix", model);
						my_shader->SetMat4("bone_transforms", currentPose[0], localSkeleton->boneCount);
						my_shader->SetBool("gamma", Context::GetInstance().isGammaOn);

						// Draw
						glBindVertexArray(localMesh->vao);
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, diffuseTexture);
						my_shader->SetInt("diff_texture", 0);

						glDrawElements(GL_TRIANGLES, (GLsizei)localMesh->indices.size(), GL_UNSIGNED_INT, 0);

						// Unbind shaders
						glUseProgram(0);
						glBindVertexArray(0);
					}
				}
				
				
				//Do the Calculation for the width and height of the context avaliable

				//Check if the size is equal, if not append
				//Check for resizing events
				if (m_size.x != contentRegionSize.x || m_size.y != contentRegionSize.y ||
					m_position.y != windowPos.x || m_position.y != windowPos.y)
				{
					m_size.x = contentRegionSize.x;
					m_size.y = contentRegionSize.y;
					m_position.x = windowPos.x;
					m_position.y = windowPos.y;
				}

				
				 

				//check if there is any size differences
				if (width != fbo_width || height != fbo_height)
				{
					glDeleteFramebuffers(1, &framebuffer);
					glDeleteTextures(1, &textureColorBuffer);

					fbo_width = width;
					fbo_height = height;
					glViewport(0, 0, width, height);

					glGenFramebuffers(1, &framebuffer);
					glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

					glGenTextures(1, &textureColorBuffer);
					glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

					glGenRenderbuffers(1, &rbo);
					glBindRenderbuffer(GL_RENDERBUFFER, rbo);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbo_width, fbo_height); // use a single renderbuffer object for both a depth AND stencil buffer.
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
				}

				//use the texture as the imgui Texture
				ImGui::Image((void*)(intptr_t)textureColorBuffer,
					ImVec2(m_size.x, m_size.y),
					ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

				//unbind the framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				//ImGui::Text("Skeleton");

				//if (ImGui::TreeNode(clip->skeleton.name.c_str()))
				//{
				//	//DisplayBoneSkeleton(clip.skeleton);
				//	ImGui::TreePop();
				//}			

				//ImGui::EndChild();
				//ImGui::PopStyleVar();
			}
			/*ImGui::SameLine();
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild("Editor", ImVec2(width * 0.5f, height * 0.7f), true);

				ImGui::Text("Window 2");

				ImGui::EndChild();
				ImGui::PopStyleVar();
			}
			ImGui::NewLine();
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
				ImGui::BeginChild("Timeline", ImVec2(width, height * 0.3f), true);

				//ImGui::Text("Timeline");
				// Controls
				if (ImGui::Button("|<"))
				{
					currentFrame = mysequence->mFrameMin;
				}
				ImGui::SameLine();
				//if (ImGui::Button("<"));
				if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { currentFrame--; }
				ImGui::SameLine();
				if (ImGui::Button("Play"))
				{

				}
				ImGui::SameLine();
				if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { currentFrame++; }
				{
					++currentFrame;
				}
				ImGui::SameLine();
				if (ImGui::Button(">|"))
				{
					currentFrame = mysequence->mFrameMax;
				}
				
				
				// Timeline

				ImGui::EndChild();
				ImGui::PopStyleVar();
			}*/
		}
		ImGui::End();
		//glfwSwapBuffers(Eos::Context::GetInstance().GetWindow());

		// Old
		/*{
			ImGui::PushItemWidth(130);
			ImGui::InputInt("Frame Min", &mysequence->mFrameMin);
			ImGui::SameLine();
			ImGui::InputInt("Frame ", &currentFrame);
			ImGui::SameLine();
			ImGui::InputInt("Frame Max", &mysequence->mFrameMax);
			ImGui::PopItemWidth();
		}*/


		//Sequencer(mysequence.get(), &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
		//if (selectedEntry != -1)
		//{
		//	const Sequence::SequenceItem& item = mysequence->Items[selectedEntry];
		//	ImGui::Text("I am a %s, please edit me", SequencerItemTypeNames[item.mType]);
		//	// switch (type) ....
		//}

		//ImGui::End();
	}

	void AnimationWindow::DisplayBoneSkeleton(AnimationCompiler::Bone bone)
	{
		/*for (auto node : bone.children)
		{
			if (ImGui::TreeNode(bone.name.c_str()))
			{
				DisplayBoneSkeleton(node);
				ImGui::TreePop();
			}
		}*/
	}

	void AnimationWindow::DisplayAnimSequencer()
	{
		//ImSequencer::Sequencer();
	}

	void AnimationWindow::DisplayAnimPreview()
	{

	}

	void AnimationWindow::DisplayAnimDetails()
	{
		//ImGuiID NodeId = ImGui::DockBuilderAddNode();
		//ImGui::DockBuilderSetNodeSize(NodeId, )

		if (ImGui::TreeNode("Transform"))
		{
			float vec4a[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

			ImGui::InputFloat3("Translation", vec4a);
			ImGui::InputFloat3("Rotation", vec4a);
			ImGui::InputFloat3("Scale", vec4a);

			ImGui::TreePop();
		}
	}
}

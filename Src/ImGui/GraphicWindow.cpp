#include "GraphicWindow.h"

namespace Eos
{
	GraphicWindow::GraphicWindow()
	{
		renderingSystem = SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS);
	}
	void GraphicWindow::Update()
	{

		enum Selection
		{
			Grass = 0,
			Ocean = 1
		};

		ImGui::Begin("Graphic Scene", &active, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
		
		//static int SkyBoxType = -1;
		//ImGui::Combo("SkyBox Type :", &SkyBoxType,
		//	"Grass\0"
		//	"Ocean\0"
		//	"None\0"
		//); // 0- grass  // 1 - ocean

		//if (oldSelection != SkyBoxType)
		//{
		//	std::cout << "Changed SkyBox" << std::endl;
		//	isDirty = true;
		//}
		//if (isDirty)
		//{
		//	std::shared_ptr<Rendering_System>  rs =
		//		std::dynamic_pointer_cast<Rendering_System>(renderingSystem);
		//	rs->faces.clear();
		//
		//	if (SkyBoxType == Selection::Grass)
		//	{
		//		rs->faces.push_back("right1.bmp");
		//		rs->faces.push_back("left1.bmp");
		//		rs->faces.push_back("bottom1.bmp");
		//		rs->faces.push_back("top1.bmp");
		//		rs->faces.push_back("front1.bmp");
		//		rs->faces.push_back("back1.bmp");
		//		glDeleteTextures(1, &rs->cubemapTexture);
		//
		//		//prototyping only
		//		unsigned int textureID;
		//		glGenTextures(1, &textureID);
		//		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		//
		//		int width, height, nrComponents;
		//		for (unsigned int i = 0; i < rs->faces.size(); i++)
		//		{
		//			unsigned char* data = stbi_load(rs->faces[i].c_str(), &width, &height, &nrComponents, 0);
		//			if (data)
		//			{
		//				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//				stbi_image_free(data);
		//			}
		//			else
		//			{
		//				std::cout << "Cubemap texture failed to load at path: " << rs->faces[i] << std::endl;
		//				stbi_image_free(data);
		//			}
		//		}
		//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		//
		//		rs->cubemapTexture = textureID;
		//
		//
		//
		//	}
		//	else if (SkyBoxType == Selection::Ocean)
		//	{
		//		rs->faces.push_back("right.jpg");
		//		rs->faces.push_back("left.jpg");
		//		rs->faces.push_back("bottom.jpg");
		//		rs->faces.push_back("top.jpg");
		//		rs->faces.push_back("front.jpg");
		//		rs->faces.push_back("back.jpg");
		//		glDeleteTextures(1, &rs->cubemapTexture);
		//
		//		//prototyping only
		//		unsigned int textureID;
		//		glGenTextures(1, &textureID);
		//		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		//
		//		int width, height, nrComponents;
		//		for (unsigned int i = 0; i < rs->faces.size(); i++)
		//		{
		//			unsigned char* data = stbi_load(rs->faces[i].c_str(), &width, &height, &nrComponents, 0);
		//			if (data)
		//			{
		//				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//				stbi_image_free(data);
		//			}
		//			else
		//			{
		//				std::cout << "Cubemap texture failed to load at path: " << rs->faces[i] << std::endl;
		//				stbi_image_free(data);
		//			}
		//		}
		//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		//		rs->cubemapTexture = textureID;
		//	}
		//	else
		//	{
		//		glDeleteTextures(1, &rs->cubemapTexture);
		//	}
		//
		//	isDirty = false;
		//}


		//oldSelection = SkyBoxType;
		
		std::shared_ptr<Rendering_System>  rs =
			std::dynamic_pointer_cast<Rendering_System>(renderingSystem);

		//ImGui::DragFloat3("Translation", &(rs->lightPosition.at(0).x), 0.01);

		ImGui::Text("Renderer Setting");
		ImGui::Text((const char*)glGetString(GL_VERSION));
		ImGui::Text((const char*)glGetString(GL_VENDOR));
		ImGui::Text((const char*)glGetString(GL_RENDERER));
		ImGui::Text("\nFramerate %.2f FPS / Frametime %.4f ms",
			ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

		ImGui::End();
	}
}
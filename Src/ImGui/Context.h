/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef CONTEXT_H_
#define CONTEXT_H_


#include <glew.h>
#include <GLFW\glfw3.h>

#include "..\pch.h"

#include <glm\glm.hpp>
#include <utility>
#include <stdlib.h>
#include <iostream>
#include "..\Tools\Singleton.h"

#include "Src\Events\Events.h"
#include "Src\Events\EventDispatcher.h"
#include "Src\Events\EventObserver.h"
#include "Src\Engine\Components\glutils.h"


namespace Eos
{
	class Context : public Singleton<Context>
	{
	public:
		Context();
		Context* Self();
		void Init(uint32_t wsizex, uint32_t wsizey);
		~Context();
		void Destroy();
		GLFWwindow* GetWindow() { return m_window; };
		glm::ivec2* GetSize() { return &m_size; };
		void Update();
		void PostUpdate();
		bool isCreated;
		
		//Viewport Size
		int32_t scr_width, scr_height;
		int32_t wposx, wposy;
		int32_t posX, posY;
		float fps; //TODO: bad code need to remove dependencies with glfwHelpers
		double scrollDelta;
		double cursor_x, cursor_y;
		bool isGammaOn = false;

		//Glfw callbacks
		static void framebufferSize_fn(GLFWwindow* window, int x, int y);
		static void scroll_fn(GLFWwindow* window, double xoffset, double yoffset);
		void framebufferCallback(int width, int height);
		void mouseScrollCallback(double xoffset, double yoffset);
		double scrollData();

		//Some window handling
		std::pair<double, double> cursorPosition() const noexcept;
		std::pair<double, double> cursorDelta() const;
		std::pair<double, double> cursorNDCDelta() const;
		


	private:

		GLFWwindow* m_window;
		glm::ivec2 m_size;
		bool inRelease;
	};

}
#endif
/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Tools\Singleton.h"
#include "..\pch.h"
#include "KeyCodes.h"
#include "Src\ImGui\Context.h"
#include "imgui.h"
#include "glm/glm.hpp"

namespace Eos
{
	enum class Cursor
	{
		Normal = 0,
		Hidden = 1,
		Locked = 2,
	};

	typedef enum class MouseButton : uint16_t
	{
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Left = Button0,
		Right = Button1,
		Middle = Button2
	}Button;

	enum class ScrollDirection
	{
		Up = 0,
		Still = 1,
		Down = 2
	};

	enum class ControllerBtn
	{
		A,
		B,
		X,
		Y,
		LBUMP,
		RBUMP,
		BACK,
		START,
		GUIDE,
		L_THUMB,
		R_THUMB,
		UP,
		RIGHT,
		DOWN,
		LEFT,
		L_TRIGGER,
		R_TRIGGER,
		UP_LA,
		RIGHT_LA,
		DOWN_LA,
		LEFT_LA,
		UP_RA,
		RIGHT_RA,
		DOWN_RA,
		LEFT_RA,
	};

	class Inputs
		: public Singleton<Inputs>
	{
		int activeJoystick = -1;
	public:

		bool IsKeyPressed(KeyCode keycode);
		bool IsKeyTriggered(KeyCode keycode);
		bool IsMouseButtonPressed(MouseButton button);
		bool IsMouseButtonReleased(MouseButton button);
		float GetMouseX();
		float GetMouseY();


		glm::vec2 GetMouseNDCCoordinates();
		void SetMouseNDCCoordinates(glm::vec2 ndcX);

		std::pair<float, float> GetMousePosition();

		void SetCursorMode(Cursor mode);
		void SetCursorPos(double xpos, double ypos);
		Cursor GetCursorMode();
		ScrollDirection direction = ScrollDirection::Still;


		//Controller
		bool IsJoystickConnected();
		bool JoystickInit();
		std::map<int, std::string> GetListOfControllers();
		bool setController(int);
		bool IsButtonPressed(ControllerBtn btn);
		void TestController();
		
		void Init(); //TODO: we can do better, might not need Init and Exit
		void Update();
		void Exit();

	private:

		glm::vec2 NormalizedCoordinates;
	};
}
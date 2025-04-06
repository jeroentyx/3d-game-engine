#pragma once

#include "Tools\Singleton.h"
#include "..\pch.h"

namespace Eos
{
	enum class Cursor : unsigned char
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

	class Inputs
		: public Singleton<Inputs>
	{
		bool IsKeyPressed(KeyCode keycode);

		bool IsMouseButtonPressed(MouseButton button);
		float GetMouseX();
		float GetMouseY();
		std::pair<float, float> GetMousePosition();

		void SetCursorMode(Cursor mode);
		Cursor GetCursorMode();



		void Init();
		void Update();
		void Exit();
	};
}
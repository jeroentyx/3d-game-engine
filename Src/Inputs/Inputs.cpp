/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Inputs.h"


namespace Eos
{

	void Inputs::Init()
	{

	}

	void Inputs::Update()
	{

	}

	void Inputs::Exit()
	{

	}
	bool Inputs::IsKeyPressed(KeyCode keycode)
	{
		//TODO: need do for imgui window as well

		GLFWwindow* win = Context::GetInstance().GetWindow();
		int state = glfwGetKey(win, static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Inputs::IsKeyTriggered(KeyCode keycode)
	{
        GLFWwindow* win = Context::GetInstance().GetWindow();
        int state = glfwGetKey(win, static_cast<int32_t>(keycode));
		return state == GLFW_PRESS;
	}
	bool Inputs::IsMouseButtonPressed(MouseButton button)
	{
		GLFWwindow* win = Context::GetInstance().GetWindow();
		int state = glfwGetMouseButton(win, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}
	bool Inputs::IsMouseButtonReleased(MouseButton button)
	{
		GLFWwindow* win = Context::GetInstance().GetWindow();
		int state = glfwGetMouseButton(win, static_cast<int32_t>(button));
		return state == GLFW_RELEASE;
	}
	float Inputs::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return static_cast<float>(x);
	}
	float Inputs::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return static_cast<float>(y);
	}

	glm::vec2 Inputs::GetMouseNDCCoordinates()
	{
		return NormalizedCoordinates;
	}

	void Inputs::SetMouseNDCCoordinates(glm::vec2 ndcX)
	{
		NormalizedCoordinates = ndcX;
	}


	std::pair<float, float> Inputs::GetMousePosition()
	{
		GLFWwindow* win = Context::GetInstance().GetWindow();
		double x, y;
		glfwGetCursorPos(win, &x, &y);
		return std::pair<float, float>(static_cast<float>(x), static_cast<float>(y));
	}
	void Inputs::SetCursorMode(Cursor mode)
	{
		GLFWwindow* win = Context::GetInstance().GetWindow();
		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL + static_cast<int>(mode));
	}
	void Inputs::SetCursorPos(double xpos, double ypos)
	{
		GLFWwindow* win = Context::GetInstance().GetWindow();
		glfwSetCursorPos(win, xpos, ypos);
	}
	Cursor Inputs::GetCursorMode()
	{
		GLFWwindow* win = Context::GetInstance().GetWindow();
		(Cursor)(glfwGetInputMode(win, GLFW_CURSOR) - GLFW_CURSOR_NORMAL);
		return Cursor();
	}

    bool Eos::Inputs::JoystickInit()
    {
        for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++)
        {
            if (glfwJoystickPresent(i))
            {
                activeJoystick = i;
                std::cout << "Joystick Found : " << glfwGetGamepadName(activeJoystick) << std::endl;
                return true;
            }
        }
        std::cout << "Joystick Not Found"<< std::endl;
        return false;
    }

    std::map<int, std::string> Inputs::GetListOfControllers()
    {
        std::map<int, std::string> temp;
        for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++)
        {
            if (glfwJoystickPresent(i))
            {
                std::string NameString = glfwGetGamepadName(activeJoystick);
                temp.insert(std::pair<int, std::string>(i, NameString));
            }
        }
        return temp;
    }

    bool Inputs::setController(int ctrl)
    {
        if (glfwJoystickPresent(ctrl)) 
        {
            activeJoystick = ctrl;
            return true;
        }
        else 
        {
            return false;
        }
    }



    bool Inputs::IsJoystickConnected()
    {
        if (activeJoystick >= 0)
            return true;
    }

    bool Eos::Inputs::IsButtonPressed(ControllerBtn btn)
    {
        if (activeJoystick >= 0) 
        {
            if (glfwJoystickIsGamepad(activeJoystick))
            {
                GLFWgamepadstate state;

                if (glfwGetGamepadState(activeJoystick, &state))
                {
                    if ((int)btn <= GLFW_GAMEPAD_BUTTON_LAST)
                    {
                        if (state.buttons[(int)btn])
                        {
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        switch (btn)
                        {
                        case ControllerBtn::L_TRIGGER:
                            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > 0)
                                return true;
                            else
                                return false;
                            break;
                        case ControllerBtn::R_TRIGGER:
                            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > 0)
                                return true;
                            else
                                return false;
                            break;
                        case ControllerBtn::LEFT_LA:
                            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -0.5)
                                return true;
                            else
                                return false;
                            break;
                        case ControllerBtn::RIGHT_LA:
                            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > 0.5)
                                return true;
                            else
                                return false;
                            break;
                        case ControllerBtn::UP_LA:
                            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -0.5)
                                return true;
                            else
                                return false;
                            break;
                        case ControllerBtn::DOWN_LA:
                            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > 0.5)
                                return true;
                            else
                                return false;
                            break;
                        case ControllerBtn::LEFT_RA:
                            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] < -0.5)
                                return true;
                            else
                                return false;
                            break;
                        case ControllerBtn::RIGHT_RA:
                            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] > 0.5)
                                return true;
                            else
                                return false;
                            break;
                        case ControllerBtn::UP_RA:
                            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] < -0.5)
                                return true;
                            else
                                return false;
                            break;
                        case ControllerBtn::DOWN_RA:
                            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] > 0.5)
                                return true;
                            else
                                return false;
                            break;
                        default:
                            return false;
                        }
                    }
                }
            }
        }
        
        return false;
    }

    void Eos::Inputs::TestController()
    {
        if (IsButtonPressed(ControllerBtn::UP_LA))
            std::cout << "Up (Left Analog)" << std::endl;
        if (IsButtonPressed(ControllerBtn::DOWN_LA))
            std::cout << "Down (Left Analog)" << std::endl;
        if (IsButtonPressed(ControllerBtn::LEFT_LA))
            std::cout << "Left (Left Analog)" << std::endl;
        if (IsButtonPressed(ControllerBtn::RIGHT_LA))
            std::cout << "Right (Left Analog)" << std::endl;

        if (IsButtonPressed(ControllerBtn::UP_RA))
            std::cout << "Up (Right Analog)" << std::endl;
        if (IsButtonPressed(ControllerBtn::DOWN_RA))
            std::cout << "Down (Right Analog)" << std::endl;
        if (IsButtonPressed(ControllerBtn::LEFT_RA))
            std::cout << "Left (Right Analog)" << std::endl;
        if (IsButtonPressed(ControllerBtn::RIGHT_RA))
            std::cout << "Right (Right Analog)" << std::endl;

        if (IsButtonPressed(ControllerBtn::UP))
            std::cout << "Up (D-Pad)" << std::endl;
        if (IsButtonPressed(ControllerBtn::DOWN))
            std::cout << "Down (D-Pad)" << std::endl;
        if (IsButtonPressed(ControllerBtn::LEFT))
            std::cout << "Left (D-Pad)" << std::endl;
        if (IsButtonPressed(ControllerBtn::RIGHT))
            std::cout << "Right (D-Pad)" << std::endl;

        if (IsButtonPressed(ControllerBtn::A))
            std::cout << "A" << std::endl;
        if (IsButtonPressed(ControllerBtn::B))
            std::cout << "B" << std::endl;
        if (IsButtonPressed(ControllerBtn::X))
            std::cout << "X" << std::endl;
        if (IsButtonPressed(ControllerBtn::Y))
            std::cout << "Y" << std::endl;

        if (IsButtonPressed(ControllerBtn::RBUMP))
            std::cout << "Right Bumper" << std::endl;
        if (IsButtonPressed(ControllerBtn::LBUMP))
            std::cout << "Left Bumper" << std::endl;
        if (IsButtonPressed(ControllerBtn::R_TRIGGER))
            std::cout << "Right Trigger" << std::endl;
        if (IsButtonPressed(ControllerBtn::L_TRIGGER))
            std::cout << "Left Trigger" << std::endl;

        if (IsButtonPressed(ControllerBtn::START))
            std::cout << "Start" << std::endl;
        if (IsButtonPressed(ControllerBtn::GUIDE))
            std::cout << "Guide" << std::endl;
        if (IsButtonPressed(ControllerBtn::BACK))
            std::cout << "Back" << std::endl;
        if (IsButtonPressed(ControllerBtn::R_THUMB))
            std::cout << "Right Thumb" << std::endl;
        if (IsButtonPressed(ControllerBtn::L_THUMB))
            std::cout << "Left Thumb" << std::endl;
    }
}
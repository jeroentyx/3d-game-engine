/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "..\Src\pch.h"

namespace Eos
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;
}

// From glfw3.h
#define EOS_KEY_SPACE           ::Eos::Key::Space
#define EOS_KEY_APOSTROPHE      ::Eos::Key::Apostrophe   
#define EOS_KEY_COMMA           ::Eos::Key::Comma        
#define EOS_KEY_MINUS           ::Eos::Key::Minus        
#define EOS_KEY_PERIOD          ::Eos::Key::Period       
#define EOS_KEY_SLASH           ::Eos::Key::Slash        
#define EOS_KEY_0               ::Eos::Key::D0
#define EOS_KEY_1               ::Eos::Key::D1
#define EOS_KEY_2               ::Eos::Key::D2
#define EOS_KEY_3               ::Eos::Key::D3
#define EOS_KEY_4               ::Eos::Key::D4
#define EOS_KEY_5               ::Eos::Key::D5
#define EOS_KEY_6               ::Eos::Key::D6
#define EOS_KEY_7               ::Eos::Key::D7
#define EOS_KEY_8               ::Eos::Key::D8
#define EOS_KEY_9               ::Eos::Key::D9
#define EOS_KEY_SEMICOLON       ::Eos::Key::Semicolon    
#define EOS_KEY_EQUAL           ::Eos::Key::Equal        
#define EOS_KEY_A               ::Eos::Key::A
#define EOS_KEY_B               ::Eos::Key::B
#define EOS_KEY_C               ::Eos::Key::C
#define EOS_KEY_D               ::Eos::Key::D
#define EOS_KEY_E               ::Eos::Key::E
#define EOS_KEY_F               ::Eos::Key::F
#define EOS_KEY_G               ::Eos::Key::G
#define EOS_KEY_H               ::Eos::Key::H
#define EOS_KEY_I               ::Eos::Key::I
#define EOS_KEY_J               ::Eos::Key::J
#define EOS_KEY_K               ::Eos::Key::K
#define EOS_KEY_L               ::Eos::Key::L
#define EOS_KEY_M               ::Eos::Key::M
#define EOS_KEY_N               ::Eos::Key::N
#define EOS_KEY_O               ::Eos::Key::O
#define EOS_KEY_P               ::Eos::Key::P
#define EOS_KEY_Q               ::Eos::Key::Q
#define EOS_KEY_R               ::Eos::Key::R
#define EOS_KEY_S               ::Eos::Key::S
#define EOS_KEY_T               ::Eos::Key::T
#define EOS_KEY_U               ::Eos::Key::U
#define EOS_KEY_V               ::Eos::Key::V
#define EOS_KEY_W               ::Eos::Key::W
#define EOS_KEY_X               ::Eos::Key::X
#define EOS_KEY_Y               ::Eos::Key::Y
#define EOS_KEY_Z               ::Eos::Key::Z
#define EOS_KEY_LEFT_BRACKET    ::Eos::Key::LeftBracket   
#define EOS_KEY_BACKSLASH       ::Eos::Key::Backslash     
#define EOS_KEY_RIGHT_BRACKET   ::Eos::Key::RightBracket  
#define EOS_KEY_GRAVE_ACCENT    ::Eos::Key::GraveAccent   
#define EOS_KEY_WORLD_1         ::Eos::Key::World1        
#define EOS_KEY_WORLD_2         ::Eos::Key::World2        

//functions
#define EOS_KEY_ESCAPE          ::Eos::Key::Escape
#define EOS_KEY_ENTER           ::Eos::Key::Enter
#define EOS_KEY_TAB             ::Eos::Key::Tab
#define EOS_KEY_BACKSPACE       ::Eos::Key::Backspace
#define EOS_KEY_INSERT          ::Eos::Key::Insert
#define EOS_KEY_DELETE          ::Eos::Key::Delete
#define EOS_KEY_RIGHT           ::Eos::Key::Right
#define EOS_KEY_LEFT            ::Eos::Key::Left
#define EOS_KEY_DOWN            ::Eos::Key::Down
#define EOS_KEY_UP              ::Eos::Key::Up
#define EOS_KEY_PAGE_UP         ::Eos::Key::PageUp
#define EOS_KEY_PAGE_DOWN       ::Eos::Key::PageDown
#define EOS_KEY_HOME            ::Eos::Key::Home
#define EOS_KEY_END             ::Eos::Key::End
#define EOS_KEY_CAPS_LOCK       ::Eos::Key::CapsLock
#define EOS_KEY_SCROLL_LOCK     ::Eos::Key::ScrollLock
#define EOS_KEY_NUM_LOCK        ::Eos::Key::NumLock
#define EOS_KEY_PRINT_SCREEN    ::Eos::Key::PrintScreen
#define EOS_KEY_PAUSE           ::Eos::Key::Pause
#define EOS_KEY_F1              ::Eos::Key::F1
#define EOS_KEY_F2              ::Eos::Key::F2
#define EOS_KEY_F3              ::Eos::Key::F3
#define EOS_KEY_F4              ::Eos::Key::F4
#define EOS_KEY_F5              ::Eos::Key::F5
#define EOS_KEY_F6              ::Eos::Key::F6
#define EOS_KEY_F7              ::Eos::Key::F7
#define EOS_KEY_F8              ::Eos::Key::F8
#define EOS_KEY_F9              ::Eos::Key::F9
#define EOS_KEY_F10             ::Eos::Key::F10
#define EOS_KEY_F11             ::Eos::Key::F11
#define EOS_KEY_F12             ::Eos::Key::F12
#define EOS_KEY_F13             ::Eos::Key::F13
#define EOS_KEY_F14             ::Eos::Key::F14
#define EOS_KEY_F15             ::Eos::Key::F15
#define EOS_KEY_F16             ::Eos::Key::F16
#define EOS_KEY_F17             ::Eos::Key::F17
#define EOS_KEY_F18             ::Eos::Key::F18
#define EOS_KEY_F19             ::Eos::Key::F19
#define EOS_KEY_F20             ::Eos::Key::F20
#define EOS_KEY_F21             ::Eos::Key::F21
#define EOS_KEY_F22             ::Eos::Key::F22
#define EOS_KEY_F23             ::Eos::Key::F23
#define EOS_KEY_F24             ::Eos::Key::F24
#define EOS_KEY_F25             ::Eos::Key::F25

//Keypad buttons
#define EOS_KEY_KP_0            ::Eos::Key::KP0
#define EOS_KEY_KP_1            ::Eos::Key::KP1
#define EOS_KEY_KP_2            ::Eos::Key::KP2
#define EOS_KEY_KP_3            ::Eos::Key::KP3
#define EOS_KEY_KP_4            ::Eos::Key::KP4
#define EOS_KEY_KP_5            ::Eos::Key::KP5
#define EOS_KEY_KP_6            ::Eos::Key::KP6
#define EOS_KEY_KP_7            ::Eos::Key::KP7
#define EOS_KEY_KP_8            ::Eos::Key::KP8
#define EOS_KEY_KP_9            ::Eos::Key::KP9
#define EOS_KEY_KP_DECIMAL      ::Eos::Key::KPDecimal
#define EOS_KEY_KP_DIVIDE       ::Eos::Key::KPDivide
#define EOS_KEY_KP_MULTIPLY     ::Eos::Key::KPMultiply
#define EOS_KEY_KP_SUBTRACT     ::Eos::Key::KPSubtract
#define EOS_KEY_KP_ADD          ::Eos::Key::KPAdd
#define EOS_KEY_KP_ENTER        ::Eos::Key::KPEnter
#define EOS_KEY_KP_EQUAL        ::Eos::Key::KPEqual

#define EOS_KEY_LEFT_SHIFT      ::Eos::Key::LeftShift
#define EOS_KEY_LEFT_CONTROL    ::Eos::Key::LeftControl
#define EOS_KEY_LEFT_ALT        ::Eos::Key::LeftAlt
#define EOS_KEY_LEFT_SUPER      ::Eos::Key::LeftSuper
#define EOS_KEY_RIGHT_SHIFT     ::Eos::Key::RightShift
#define EOS_KEY_RIGHT_CONTROL   ::Eos::Key::RightControl
#define EOS_KEY_RIGHT_ALT       ::Eos::Key::RightAlt
#define EOS_KEY_RIGHT_SUPER     ::Eos::Key::RightSuper
#define EOS_KEY_MENU            ::Eos::Key::Menu


#define EOS_MOUSE_BUTTON_LEFT    0
#define EOS_MOUSE_BUTTON_RIGHT   1
#define EOS_MOUSE_BUTTON_MIDDLE  2
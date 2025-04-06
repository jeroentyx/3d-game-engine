#pragma once
enum class BodyType : unsigned char{
	STATIC = 0, //never moves
	DYNAMIC, //moves and can be affected by other bodies
	KINEMATIC
};
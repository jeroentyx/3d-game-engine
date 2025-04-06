
#include "Src\pch.h"
#include "EosTexture2D.h"

struct EosMaterial
{
	EosTexture2D* diffuse;
	EosTexture2D* specular;
	EosTexture2D* normal;
	float shininess;
};
#pragma once
#include "../../dependencies/glm/glm.hpp"
#include <map>
#include <string>
#include <fstream>
#include "FontReader.h"



struct Data
{
	double advance;
	glm::dvec4 planebound;
	glm::vec4 atlasbound;
};




class FontSerializer
{

	std::map<int, Data> FontData;

public:

	//Public Data Member in case need to use;

	//atlas
	std::string type = std::string{};
	int distanceRange = 0;
	float size = 0.0f;
	int width = 0;
	int height = 0;
	std::string yOrigin = std::string{};


	//metrics
	int emSize;
	float lineHeight;
	double ascender;
	float descender;
	float underlineY;
	double underlineThickness;

	std::string OpenFile(const std::string& filepath);
	bool Load(const std::string& filepath, std::ofstream& out);

};
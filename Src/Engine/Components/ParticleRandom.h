#pragma once

#include "Src\pch.h"
#include "glew.h"
#include <random>
// contains new std::shuffle definition
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class Random {
private:
    std::mt19937 generator;
    std::uniform_real_distribution<float> distr01;

public:
	Random();

	float nextFloat();

	static void shuffle(std::vector<GLfloat>& v);

	glm::vec3 uniformHemisphere();

	glm::vec3 uniformCircle();
};

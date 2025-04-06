#pragma once

#include <glm/glm.hpp>
#include <glew.h>

struct Particle
{
	glm::vec3 pos, speed;
	unsigned r, g, b, a;
	float size, angle, weight;
	float life;
	float cameraDistance;

	bool operator<(const Particle& that) const;
};
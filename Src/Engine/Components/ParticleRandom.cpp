#include "ParticleRandom.h"
#pragma once

Random::Random() : distr01(0.0f, 1.0f) {
    std::random_device rd;
    generator.seed(rd());
}

float Random::nextFloat() {
    return distr01(generator);
}

void Random::shuffle(std::vector<GLfloat>& v) {
    auto rng = std::default_random_engine{};
    std::shuffle(v.begin(), v.end(), rng);
}

glm::vec3 Random::uniformHemisphere() {
	glm::vec3 result;
	float x1 = nextFloat(), x2 = nextFloat();
	float s = glm::sqrt(1.0f - x1 * x1);
	result.x = glm::cos(glm::two_pi<float>() * x2) * s;
	result.y = glm::sin(glm::two_pi<float>() * x2) * s;
	result.z = x1;
	return result;
}

glm::vec3 Random::uniformCircle() {
	glm::vec3 result(0.0f);
	float x = nextFloat();
	result.x = glm::cos(glm::two_pi<float>() * x);
	result.y = glm::sin(glm::two_pi<float>() * x);
	return result;
}

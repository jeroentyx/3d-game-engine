#include "Cube.h"
Cube::Cube() :
	min{ glm::vec3{0} },
	max{ glm::vec3{0} }
{
}


glm::vec3 Cube::getMin() {
	return min;
}

glm::vec3 Cube::getMax() {
	return max;
}

void Cube::setMin(const glm::vec3 v) {
	min = v;
}

void Cube::setMax(const glm::vec3 v) {
	max = v;
}

void Cube::setCenter(glm::vec3 pt) {
	centerPoint = pt;
}

glm::vec3 Cube::getCenter() {
	return centerPoint;
}
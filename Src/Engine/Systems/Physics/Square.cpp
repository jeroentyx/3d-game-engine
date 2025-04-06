#include "Square.h"
Square::Square() {
}

void Square::setCenter(glm::vec3 pt) {
	centerPoint = pt;
}

glm::vec3 Square::getCenter() {
	return centerPoint;
}
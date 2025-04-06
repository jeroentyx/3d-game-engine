#pragma once
#include "Math.h"
#include "FixedPhysicsValues.h"
#include "IShape.h"


struct AABB3D
{
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 centerPoint;
	glm::vec3 scale; 
	glm::vec3 offset; //temporarily offset from actual body instead
public:
	AABB3D() :
		min{ glm::vec3() },
		max{ glm::vec3() },
		centerPoint{ glm::vec3() },
		scale{ glm::vec3(1.0f, 1.0f, 1.0f) },
		offset{ glm::vec3(0.0f, 0.0f, 0.0f) }
	{
	}
	void setCenter(glm::vec3 pt) {
		centerPoint = pt;
	}
	
	void setScale(glm::vec3 sc) {
		scale = sc;
	}
	
	void setOffset(glm::vec3 os) {
		offset = os;
	}

	glm::vec3 getCenter() {
		return centerPoint;
	}

	glm::vec3 getMin() {
		return min;
	}

	glm::vec3 getMax() {
		return max;
	}

	glm::vec3 getScale() {
		return scale;
	}

	glm::vec3 getOffset() {
		return offset;
	}
};

struct OBB
{
	glm::vec3 pos, axisX, axisY, axisZ, HalfSize;
};
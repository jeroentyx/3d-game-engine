#pragma once
#include "Math.h"
#include "ColliderBox.h"
#include <vector>
struct IShape
{
	glm::vec3 centerPoint = glm::vec3{};
public:
	virtual ~IShape() = default;
	virtual void setCenter(glm::vec3 pt) = 0;
	virtual glm::vec3 getCenter() = 0;
};


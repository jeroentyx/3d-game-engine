#pragma once
#include "IShape.h"
struct Cube : public IShape
{
	Cube();
	glm::vec3 getMin();
	glm::vec3 getMax();
	void setMin(const glm::vec3);
	void setMax(const glm::vec3);

	virtual void setCenter(glm::vec3) override;
	virtual glm::vec3 getCenter() override;

	glm::vec3 min;
	glm::vec3 max;
};


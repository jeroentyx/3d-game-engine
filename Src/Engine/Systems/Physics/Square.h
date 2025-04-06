#pragma once
#include "IShape.h"
struct Square : public IShape
{
	Square();
	virtual void setCenter(glm::vec3) override;
	virtual glm::vec3 getCenter() override;
};


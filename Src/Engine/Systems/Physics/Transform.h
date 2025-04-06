#pragma once
#include "Math.h"

struct Transform {
	glm::vec3 t_pos;
	glm::vec3 t_scale;
	glm::quat t_rot;

	glm::mat4 quatRotToMat4() {
		return glm::toMat4(t_rot);
	}

	glm::vec3 getVec3Rot() {
		glm::vec3 v3 = glm::eulerAngles(t_rot);
		return v3;
	}

	void setFromVec3Rot(glm::vec3 rot) {
		t_rot = glm::quat{ rot };
	}
};
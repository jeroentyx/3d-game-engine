#pragma once
#include "RigidBody.h"

struct Contact {
	glm::vec3 c_pos = glm::vec3{};
	float c_penetration = 0;
};


struct Manifold {
	std::shared_ptr<RigidBody> m_a;
	std::shared_ptr<RigidBody> m_b;
	Contact m_contact;

	float m_intersectTime = 0.0f;

	glm::vec3 m_normal = glm::vec3{};
	glm::vec3 m_gravity = glm::vec3{};
	float m_friction = 0.0f;
	
	bool m_isStaticCollision = true;

	friend inline bool operator==(const Manifold& lhs, const Manifold& rhs) {
		return lhs.m_a == rhs.m_a && lhs.m_b == rhs.m_b;
	}

	friend inline bool operator!=(const Manifold& lhs, const Manifold& rhs) {
		return !(lhs.m_a == rhs.m_a && lhs.m_b == rhs.m_b);
	}

};
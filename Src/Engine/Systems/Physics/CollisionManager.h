#pragma once
#include "ColliderBox.h"
#include "Manifold.h"
#include <queue>

//bool AABBOverlap(const AABB3D& box1, const glm::vec3& v1, const AABB3D& box2, const glm::vec3& v2);
bool DynamicAABBTest(const AABB3D box1, const glm::vec3 v1, const AABB3D box2, const glm::vec3 v2, Manifold& manifold);
bool StaticAABBTest(const AABB3D box1, const AABB3D box2);
void ResolveDynamicManifold(const Manifold& manifold, float dt, bool friction, std::queue<std::pair<uint32_t, uint32_t>> q);
void ResolveStaticManifold(const Manifold& manifold);
bool raySphere(glm::vec3 ray, glm::vec3 dir, glm::vec3 objPos, glm::vec3 camPos, float rad, float minDist);
bool rayBox(glm::vec3 ray, glm::vec3 dir, float dist, glm::vec3 min, glm::vec3 max, float minDist);
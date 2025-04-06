#include "CollisionManager.h"
bool StaticAABBTest(const AABB3D box1, const AABB3D box2) {
	glm::vec3 min1 = box1.min, min2 = box2.min;
	glm::vec3 max1 = box1.max, max2 = box2.max;

	// if a separating axis is found, then there is no intersection
	for (int i = 0; i < 3; ++i)
	{
		if (max1[i] < min2[i] || min1[i] > max2[i])
			return false;
	}

	return true;
}

bool DynamicAABBTest(const AABB3D box1, const glm::vec3 v1, const AABB3D box2, const glm::vec3 v2, Manifold& manifold) {
	// exit early if both are already overlapping
	if (StaticAABBTest(box1, box2))
	{
		glm::vec3 relativeVec = box2.centerPoint - box1.centerPoint;
		int intersectAxis = 0;
		float penetrationDepth = std::numeric_limits<float>::max();

		glm::vec3 aabb1HalfExtents = (box1.max - box1.min) * 0.5f;
		glm::vec3 aabb2HalfExtents = (box2.max - box2.min) * 0.5f;
		glm::vec3 combinedHalfExtents = aabb1HalfExtents + aabb2HalfExtents;

		for (int i = 0; i < 3; ++i)
		{
			float axisPenetration = combinedHalfExtents[i] - fabsf(relativeVec[i]);
			if (fabs(axisPenetration) >= 0.0f && axisPenetration < penetrationDepth)
			{
				intersectAxis = i;
				penetrationDepth = axisPenetration;
			}
		}

		manifold.m_intersectTime = 0.0f;
		manifold.m_normal = glm::vec3(0.0f, 0.0f, 0.0f);
		manifold.m_normal[intersectAxis] = -1.0f;

		manifold.m_contact.c_penetration = penetrationDepth;
		manifold.m_isStaticCollision = true;

		return true;
	}

	glm::vec3 relativeVel = (v2 - v1) + glm::vec3(glm::epsilon<float>(), glm::epsilon<float>(), glm::epsilon<float>());

	float tFirst = 0.0f;
	float tLast = fdt;

	glm::vec3 min1 = box1.min, min2 = box2.min;
	glm::vec3 max1 = box1.max, max2 = box2.max;

	float firstIntersect = fdt;
	int intersectAxis = 0;
	float penetrationDepth = 0.0f;

	//check for all axis
	for (int i = 0; i < 3; ++i)
	{
		if (relativeVel[i] <= 0.0f)
		{
			if (max2[i] < min1[i])
				return false;

			if (max1[i] < min2[i])
				tFirst = std::max((max1[i] - min2[i]) / relativeVel[i], tFirst);

			if (max2[i] > min1[i])
				tLast = std::min((min1[i] - max2[i]) / relativeVel[i], tLast);
		}

		else if (relativeVel[i] > 0.0f)
		{
			if (min2[i] > max1[i])
				return false;

			if (max2[i] < min1[i])
				tFirst = std::max((min1[i] - max2[i]) / relativeVel[i], tFirst);

			if (max1[i] > min2[i])
				tLast = std::min((max1[i] - min2[i]) / relativeVel[i], tLast);
		}

		if (tFirst > tLast)
			return false;

		else if (tFirst < firstIntersect && tFirst > 0)
		{
			firstIntersect = tFirst;
			intersectAxis = i;
			penetrationDepth = relativeVel[i] * (tLast - tFirst);
		}
	}

	manifold.m_intersectTime = tFirst;
	manifold.m_normal = glm::vec3(0.0f, 0.0f, 0.0f);
	manifold.m_normal[intersectAxis] = -1.0f;

	manifold.m_contact.c_penetration = penetrationDepth;
	manifold.m_isStaticCollision = false;
	return true;
}

void ResolveStaticManifold(const Manifold& manifold) {

	if (manifold.m_a->getBodyType() == BodyType::STATIC || manifold.m_a->getBodyType() == BodyType::KINEMATIC) {
		if (manifold.m_b->getBodyType() == BodyType::STATIC || manifold.m_b->getBodyType() == BodyType::KINEMATIC) {
			return;
		}
		else {

			if (glm::dot(manifold.m_a->getAABBCenter() - manifold.m_b->getAABBCenter(), manifold.m_normal) > 0)
				manifold.m_b->setAABBCenter(manifold.m_b->getAABBCenter() + (-manifold.m_normal) * manifold.m_contact.c_penetration);
			else
				manifold.m_b->setAABBCenter(manifold.m_b->getAABBCenter() + manifold.m_normal * manifold.m_contact.c_penetration);
		}
	}
	else if (manifold.m_b->getBodyType() == BodyType::STATIC || manifold.m_b->getBodyType() == BodyType::KINEMATIC) {
		if (glm::dot(manifold.m_b->getAABBCenter() - manifold.m_a->getAABBCenter(), manifold.m_normal) > 0)
			manifold.m_a->setAABBCenter(manifold.m_a->getAABBCenter() + (-manifold.m_normal) * manifold.m_contact.c_penetration);
		else
			manifold.m_a->setAABBCenter(manifold.m_a->getAABBCenter() + manifold.m_normal * manifold.m_contact.c_penetration);
	}
	else {
		if (glm::dot(manifold.m_a->getAABBCenter() - manifold.m_b->getAABBCenter(), manifold.m_normal) > 0) {
			manifold.m_a->setAABBCenter(manifold.m_a->getAABBCenter() - (-manifold.m_normal) * manifold.m_contact.c_penetration);
			manifold.m_b->setAABBCenter(manifold.m_b->getAABBCenter() + (-manifold.m_normal) * manifold.m_contact.c_penetration);
		}
		else {
			manifold.m_a->setAABBCenter(manifold.m_a->getAABBCenter() - manifold.m_normal * manifold.m_contact.c_penetration);
			manifold.m_b->setAABBCenter(manifold.m_b->getAABBCenter() + manifold.m_normal * manifold.m_contact.c_penetration);
		}
	}
	manifold.m_a->setLinearVelocity(glm::vec3(manifold.m_a->getLinearVelocity().x, 0, manifold.m_a->getLinearVelocity().z));
	manifold.m_b->setLinearVelocity(glm::vec3(manifold.m_b->getLinearVelocity().x, 0, manifold.m_b->getLinearVelocity().z));
}


void ResolveDynamicManifold(const Manifold& manifold, float dt, bool friction, std::queue<std::pair<uint32_t, uint32_t>> q) {
	//UNREFERENCED_PARAMETER(dt);
	(void)dt;
	(void)friction;
	//static collision will be halted until fixed due to time constraints
	//set flag to show collision occured
	manifold.m_a->setCollisionFlag(true);
	manifold.m_b->setCollisionFlag(true);
	manifold.m_a->setCurrentCollisionFlag(true);
	manifold.m_b->setCurrentCollisionFlag(true);

	if (manifold.m_a->getObjectType() == ObjectType::SURGE || manifold.m_b->getObjectType() == ObjectType::SURGE) {
		std::pair<int, int> a;
		a.first = manifold.m_a->getID();
		a.second = manifold.m_b->getID();
		q.push(a);
	}

	if (manifold.m_isStaticCollision && !manifold.m_a->getIgnoreCollisionResponse() && !manifold.m_b->getIgnoreCollisionResponse()) {
		ResolveStaticManifold(manifold);
	}
	else if (!manifold.m_a->getIgnoreCollisionResponse() && !manifold.m_b->getIgnoreCollisionResponse()){
		glm::vec3 rb2rb1 = manifold.m_a->getAABBCenter() - manifold.m_b->getAABBCenter();
		glm::vec3 rb1rb2 = manifold.m_b->getAABBCenter() - manifold.m_a->getAABBCenter();

		glm::vec3 rb2v = glm::vec3(), rb1v = glm::vec3();

		if (manifold.m_a->getBodyType() == BodyType::STATIC)
			rb1v = glm::vec3();
		else
			rb1v = manifold.m_a->getLinearVelocity();

		if (manifold.m_b->getBodyType() == BodyType::STATIC)
			rb2v = glm::vec3();
		else
			rb2v = manifold.m_b->getLinearVelocity();

		manifold.m_a->setAABBCenter(manifold.m_a->getAABBCenter() + rb1v * manifold.m_intersectTime);
		manifold.m_b->setAABBCenter(manifold.m_b->getAABBCenter() + rb2v * manifold.m_intersectTime);

		glm::vec3 impulse = manifold.m_normal;
		if (glm::dot(rb1rb2, manifold.m_normal) > 0) {
			impulse = -impulse;
		}

		glm::vec3 relativeVel = rb2v - rb1v;
		float mag = fabsf(glm::dot(-(((manifold.m_a->getRestitution() + manifold.m_b->getRestitution()) * 0.5f) + 1.0f) * (rb2v - rb1v), impulse));

		impulse = impulse * mag / (manifold.m_a->getInvMass() + manifold.m_b->getInvMass());

		if (manifold.m_a->getBodyType() == BodyType::DYNAMIC) {
			manifold.m_a->applyImpulse(impulse);
			manifold.m_a->setSolveTime(manifold.m_a->getSolveTime() - manifold.m_intersectTime);
		}

		if (manifold.m_b->getBodyType() == BodyType::DYNAMIC) {
			manifold.m_b->applyImpulse(-impulse);
			manifold.m_b->setSolveTime(manifold.m_a->getSolveTime() - manifold.m_intersectTime);
		}
	}
	manifold.m_a->setPosition(manifold.m_a->getAABBCenter() - manifold.m_a->getAABBOffset());
	manifold.m_b->setPosition(manifold.m_b->getAABBCenter() - manifold.m_b->getAABBOffset());
}

bool raySphere(glm::vec3 ray, glm::vec3 dir, glm::vec3 objPos, glm::vec3 camPos, float rad, float minDist) {
	(void)ray;
	(void)minDist;
	glm::vec3 v = objPos - camPos;
	long double a = glm::dot(dir, dir);
	long double b = 2.0 * glm::dot(v, dir);
	long double c = glm::dot(v, v) - (long double)((long double)rad * (long double)rad);
	long double b24ac = b * b + (-4.0) * a * c;

	if (b24ac == 0)
		return true;
	else if (b24ac > 0) {
		long double x1 = (-b - glm::sqrt(b24ac)) / (2.0 * a);
		long double x2 = (-b + glm::sqrt(b24ac)) / (2.0 * a);

		if (x1 >= 0.0 || x2 >= 0.0)
			return true;
		if (x1 < 0.0 || x2 >= 0.0)
			return true;
	}
	return false;
}

bool rayBox(glm::vec3 ray, glm::vec3 dir, float dist, glm::vec3 min, glm::vec3 max, float minDist) {
	glm::vec3 endPt = dir * dist;
	glm::vec3 rayDir = endPt - ray;
	float tMinX = -INFINITY;
	float tMaxX = INFINITY;
	float tMinY = -INFINITY;
	float tMaxY = INFINITY;
	float tMinZ = -INFINITY;
	float tMaxZ = INFINITY;

	if (rayDir.x > 0.0f/* || rayDir.x < 0.0f*/) {
		tMinX = (min.x - ray.x) / rayDir.x;
		tMaxX = (max.x - ray.x) / rayDir.x;
	}
	if (tMinX > tMaxX)
		std::swap(tMinX, tMaxX);
	
	if (rayDir.y > 0.0f/* || rayDir.y < 0.0f*/) {
		tMinY = (min.y - ray.y) / rayDir.y;
		tMaxY = (max.y - ray.y) / rayDir.y;
	}
	if (tMinY > tMaxY)
		std::swap(tMinY, tMaxY);

	if (rayDir.z > 0.0f/* || rayDir.z < 0.0f*/) {
		tMinZ = (min.z - ray.z) / rayDir.z;
		tMaxZ = (max.z - ray.z) / rayDir.z;
	}
	if (tMinZ > tMaxZ)
		std::swap(tMinZ, tMaxZ);

	float tMin = (tMinX > tMinY) ? tMinX : tMinY;
	float tMax = (tMaxX < tMaxY) ? tMaxX : tMaxY;

	if (tMinX > tMaxY || tMinY > tMaxX) return false;
	if (tMin > tMaxZ || tMinZ > tMax) return false;
	if (tMinZ > tMin) tMin = tMinZ;
	if (tMaxZ < tMax) tMax = tMaxZ;

	minDist = tMin;

	return true;
}
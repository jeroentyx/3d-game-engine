#pragma once
#include "Transform.h"
#include "BodyType.h"
#include "CollisionType.h"
#include "SleepType.h"
#include "FixedPhysicsValues.h"
#include "ColliderBox.h"
#include "Tools/FixedTimer.h"
#include "ObjectType.h"
#include <memory>
#include <limits>

enum class CollidingStatus : unsigned char
{
	NONE,
	ENTER,
	EXIT,
	STAY,
};

struct Colliding_Entity
{
	uint32_t ce_id;
	//EntityID ce_id;
	CollidingStatus ce_status;

	Colliding_Entity() :
		ce_id{ std::numeric_limits<uint32_t>::max() },
		ce_status{ CollidingStatus::NONE }{

	}

	Colliding_Entity(uint32_t id, CollidingStatus stats) :
		ce_id{ id },
		ce_status{ stats }{

	}

	void setStatus(CollidingStatus stats) {
		ce_status = stats;
	}

	bool operator==(const Colliding_Entity rhs) {
		return (rhs.ce_id == ce_id);
	}
};

class RigidBody {
public:
	uint32_t rb_id;
	AABB3D aabb;
	BodyType rb_bodyType; //how this body reacts to other bodies
	Transform rb_transform; //everything transformation related
	ObjectType rb_objectType;
	float rb_mass; //to be manually set
	float rb_invMass;
	glm::vec3 rb_prevPosition;
	glm::vec3 rb_linearVelocity;
	glm::vec3 rb_angularVelocity;
	glm::vec3 rb_force;
	glm::vec3 rb_impulse;
	glm::vec3 rb_torque;
	glm::vec3 rb_gravityDirection; //1 for positive in axis, -1 for negative, 0 for none
	glm::vec3 rb_faceDirection;
	glm::vec3 rb_frameOffset; //how much body moved in this frame
	glm::mat3x3 rb_inertia; //manually set UNUSED
	glm::mat3x3 rb_inertiaW; //manually set UNUSED
	float rb_friction;
	bool rb_hasFriction;
	bool rb_hasGravity;
	float rb_restitution;
	bool rb_collisionFlag;
	float rb_solveTime;
	bool rb_isStatic;
	bool rb_isDynamic;
	bool rb_isKinematic;
	bool rb_ignoreCollisionResponse;
	bool rb_previousCollisionFlag;
	bool rb_currentCollisionFlag;
	std::vector<Colliding_Entity> rb_colliders;

	RigidBody() :
		rb_id{ UINT32_MAX },
		aabb{ AABB3D() },
		rb_bodyType{ BodyType::STATIC },
		rb_transform{ Transform() },
		rb_objectType{ ObjectType::NORMALBLOCK },
		rb_mass{ 0.0f },
		rb_invMass{ 0.0f },
		rb_prevPosition{ glm::vec3() },
		rb_linearVelocity{ glm::vec3() },
		rb_angularVelocity{ glm::vec3() },
		rb_force{ glm::vec3() },
		rb_impulse{ glm::vec3() },
		rb_torque{ glm::vec3() },
		rb_gravityDirection{ glm::vec3(0, -1, 0) },
		rb_faceDirection{ glm::vec3{1,0,0} },
		rb_frameOffset{ glm::vec3() },
		rb_inertia{ glm::mat3x3() },
		rb_inertiaW{ glm::mat3x3() },
		rb_friction{ 0.5f },
		rb_hasFriction{ true },
		rb_hasGravity{ true },
		rb_restitution{ 0.0f },
		rb_collisionFlag{ false },
		rb_solveTime{ (float)Eos::FixedTimer::GetFixedDeltaTime() },
		rb_isStatic{ true },
		rb_ignoreCollisionResponse{ false },
		rb_previousCollisionFlag{ false },
		rb_currentCollisionFlag{ false }
	{
		setMass();
	}

	uint32_t getID() { return rb_id; }
	AABB3D getAABB() { return aabb; }
	void initAABB() {
		aabb.centerPoint = rb_transform.t_pos;
		aabb.min = rb_transform.t_pos - (rb_transform.t_scale * 0.5f);
		aabb.max = rb_transform.t_pos + (rb_transform.t_scale * 0.5f);
	}
	void updateAABB() {
		/*aabb.centerPoint = rb_transform.t_pos;
		aabb.min = rb_transform.t_pos - (rb_transform.t_scale * 0.5f);
		aabb.max = rb_transform.t_pos + (rb_transform.t_scale * 0.5f);*/
		aabb.centerPoint = rb_transform.t_pos + aabb.offset; //aabb box should be +1 +1 +1
		aabb.min = aabb.centerPoint - (aabb.scale * 0.5f);
		aabb.max = aabb.centerPoint + (aabb.scale * 0.5f);
		/*aabb.centerPoint += rb_frameOffset;
		aabb.min = aabb.centerPoint - (rb_transform.t_scale * 0.5f);
		aabb.max = aabb.centerPoint + (rb_transform.t_scale * 0.5f);*/
		/*static bool check = true;
		if (check) {

			std::cout << "aabb center" << aabb.centerPoint.x << "," << aabb.centerPoint.y << "," << aabb.centerPoint.z << std::endl;
			std::cout << "aabb scale" << aabb.scale.x << "," << aabb.scale.y << "," << aabb.scale.z << std::endl;
			std::cout << "obj center" << rb_transform.t_pos.x << "," << rb_transform.t_pos.y << "," << rb_transform.t_pos.z << std::endl;
			std::cout << "obj scale" << rb_transform.t_scale.x << "," << rb_transform.t_scale.y << "," << rb_transform.t_scale.z << std::endl;
			check = false;
		}*/
	}
	BodyType getBodyType() { return rb_bodyType; }
	Transform getTransform() { return rb_transform; }
	ObjectType getObjectType() { return rb_objectType; }
	float getMass() { return rb_mass; }
	float getInvMass() { return rb_invMass; }
	glm::vec3 getPosition() { return rb_transform.t_pos; }
	glm::vec3 getRotation() {
		return rb_transform.getVec3Rot();
	}
	glm::quat getQuatRot() { return rb_transform.t_rot; }
	glm::vec3 getScale() { return rb_transform.t_scale; }
	glm::vec3 getPrevPosition() { return rb_prevPosition; }
	glm::vec3 getLinearVelocity() { return rb_linearVelocity; }
	glm::vec3 getAngularVelocity() { return rb_angularVelocity; }
	glm::vec3 getForce() { return rb_force; }
	glm::vec3 getAcceleration() { return rb_force * rb_invMass; }
	glm::vec3 getImpulse() { return rb_impulse; }
	glm::vec3 getTorque() { return rb_torque; }
	glm::vec3 getGravityDirection() { return rb_gravityDirection; }
	glm::vec3 getFaceDirection() {
		return rb_faceDirection;
	}
	glm::vec3 getFrameOffset() {
		return rb_frameOffset;
	}
	bool getFrictionFlag() { return rb_hasFriction; }
	bool getGravityFlag() { return rb_hasGravity; }
	void resetForce() { rb_force = glm::vec3(); }
	void resetTorque() { rb_torque = glm::vec3(); }
	float getFriction() { return rb_friction; }
	float getRestitution() { return rb_restitution; }
	bool getCollisionFlag() { return rb_collisionFlag; }
	float getSolveTime() { return rb_solveTime; }
	bool getIgnoreCollisionResponse() { return rb_ignoreCollisionResponse; }
	bool getPreviousCollisionFlag() { return rb_previousCollisionFlag; }
	bool getCurrentCollisionFlag() { return rb_currentCollisionFlag; }

	void setID(EntityID id) { rb_id = id._id; }
	void setBodyType(BodyType type) { rb_bodyType = type; }
	void setBodyTypeInt(int type) {
		if (type == 1)
			rb_bodyType = BodyType::STATIC;
		else if (type == 0)
			rb_bodyType = BodyType::DYNAMIC;
	}
	void setMass() {
		glm::vec3 halfextents = (aabb.getMax() - aabb.getMin()) * 0.5f;
		float ex2 = (4.0f) * halfextents.x * halfextents.x;
		float ey2 = (4.0f) * halfextents.y * halfextents.y;
		float ez2 = (4.0f) * halfextents.z * halfextents.z;

		rb_mass = 8.0f * halfextents.x * halfextents.y * halfextents.z * 1.0f;

		float x = (1.0f / 12.0f) * rb_mass * (ey2 + ez2);
		float y = (1.0f / 12.0f) * rb_mass * (ex2 + ez2);
		float z = (1.0f / 12.0f) * rb_mass * (ex2 + ey2);


		glm::mat3x3 I{ {x, 0, 0},
						{0, y, 0},
						{0, 0, z} };
		glm::mat3x3 newrot = glm::toMat3(glm::qua(rb_transform.t_rot));
		I = newrot * I * glm::transpose(newrot);
		glm::mat3x3 identity = glm::identity<glm::mat3x3>();
		I += (identity * glm::dot(rb_transform.t_pos, rb_transform.t_pos) - glm::outerProduct(rb_transform.t_pos, rb_transform.t_pos)) * rb_mass;
		rb_inertia = I;
		rb_inertiaW = I;
		setInvMassFromMass();
	}
	void setInvMass(float i) { rb_invMass = i; }
	void setInvMassFromMass() { rb_invMass = 1 / (rb_mass + FLT_EPSILON); }
	void setPosition(glm::vec3 pos) { rb_transform.t_pos = pos; }
	void setRotation(glm::vec3 rot) { rb_transform.setFromVec3Rot(rot); }
	void setRotation(glm::quat rot) { rb_transform.t_rot = rot; }
	void setScale(glm::vec3 sca) { rb_transform.t_scale = sca; }
	void setPrevPosition(glm::vec3 pos) { rb_prevPosition = pos; }
	void setLinearVelocity(glm::vec3 vel) { rb_linearVelocity = vel; }
	void setAngularVelocity(glm::vec3 vel) { rb_angularVelocity = vel; }
	void setGravityDirection(glm::vec3 gd) { rb_gravityDirection = gd; }
	void setGravityDirection(float x, float y, float z) { rb_gravityDirection = glm::vec3{ x,y,z }; }
	void setFaceDirection(glm::vec3 fd) { rb_faceDirection = fd; }
	void setFaceDirection(float x, float y, float z) { rb_faceDirection = glm::vec3{ x,y,z }; }
	void setFrameOffset(glm::vec3 offset) { rb_frameOffset = offset; }
	void setFrictionFlag(bool ean) { rb_hasFriction = ean; }
	void setGravityFlag(bool ean) { rb_hasGravity = ean; }
	void setFriction(float friction) { rb_friction = friction; }
	void setRestitution(float res) { rb_restitution = res; }
	void setCollisionFlag(bool ean) { rb_collisionFlag = ean; }
	void setSolveTime(float t) { rb_solveTime = t; }
	void setIgnoreCollisionResponse(bool ean) { rb_ignoreCollisionResponse = ean; }
	void setPreviousCollisionFlag(bool ean) { rb_previousCollisionFlag = ean; }
	void setCurrentCollisionFlag(bool ean) { rb_currentCollisionFlag = ean; }
	void applyForce(glm::vec3 force) { rb_force += force/* * rb_mass*/; }
	void applyForceAtPoint(glm::vec3 force, glm::vec3 point) {
		rb_force += force * rb_mass;
		rb_torque += glm::cross(point - rb_transform.t_pos, rb_force);
		rb_angularVelocity += glm::inverse(rb_inertiaW) * rb_torque;
	}
	void applyImpulse(glm::vec3 impulse) {
		rb_linearVelocity += impulse * rb_invMass;
	}
	void applyImpulseAtPoint(glm::vec3 impulse, glm::vec3 point) {
		rb_linearVelocity += rb_invMass * impulse;
		rb_angularVelocity += rb_inertiaW * glm::cross(point - rb_transform.t_pos, impulse);
	}
	void applyTorque(glm::vec3 torque) { rb_torque += torque; }

	//for serializer only
	void setForce(glm::vec3 force) { rb_force = force; }
	void setImpulse(glm::vec3 impulse) { rb_impulse = impulse; }
	void setTorque(glm::vec3 torque) { rb_torque = torque; }

	void setTransform(glm::vec3 pos, glm::vec3 sca, glm::vec3 rot) {
		setPosition(pos);
		setScale(sca);
		setRotation(rot);
	}

	void setBodyTypeInternal() {
		if (rb_isStatic)
			rb_bodyType = BodyType::STATIC;
		else if (rb_isDynamic)
			rb_bodyType = BodyType::DYNAMIC;
		else
			rb_bodyType = BodyType::KINEMATIC;
	}

	inline void setBodyTypeIntCase(int t) {
		switch (t) {
		case 0:
			rb_isStatic = true;
			rb_isDynamic = false;
			rb_isKinematic = false;
			break;
		case 1:
			rb_isStatic = false;
			rb_isDynamic = true;
			rb_isKinematic = false;
			break;
		case 2:
			rb_isStatic = false;
			rb_isDynamic = false;
			rb_isKinematic = true;
			break;
		default:
			rb_isStatic = true;
			rb_isDynamic = false;
			rb_isKinematic = false;
		}
		setBodyTypeInternal();
	}

	void setObjectType(ObjectType type) {
		rb_objectType = type;
	}

	void setIsStatic(bool ean) {
		rb_isStatic = ean;
		if (ean)
			setBodyTypeIntCase(0);
	}

	void setIsDynamic(bool ean) {
		rb_isDynamic = ean;
		if (ean)
			setBodyTypeIntCase(1);
	}

	void setIsKinematic(bool ean) {
		rb_isKinematic = ean;
		if (ean)
			setBodyTypeIntCase(2);
	}

	bool getIsStatic() {
		return rb_isStatic;
	}

	bool getIsDynamic() {
		return rb_isDynamic;
	}

	bool getIsKinematic() {
		return rb_isKinematic;
	}

	void applyGravity(float g, float _dt) {
		if (rb_hasGravity) {
			applyForce(rb_gravityDirection * g);
		}
	}

	void applyFriction() {
		glm::vec3 invertedBool = glm::vec3{ 1 - abs(rb_gravityDirection.x), 1 - abs(rb_gravityDirection.y), 1 - abs(rb_gravityDirection.z) } *-1.0f;
		applyForce(invertedBool * rb_mass * 9.81f * 0.5f * rb_linearVelocity);
	}

	void rotateAlongAxis(glm::vec3 angle) {
		float x = angle.x;
		float y = angle.y;
		float z = angle.z;

		(void)x; (void)y; (void)z;

		glm::vec3 currRot = glm::degrees(rb_transform.getVec3Rot());
		glm::vec3 newRot = currRot + angle;

		setRotation(glm::radians(newRot));
	}

	bool isEntityInvolved(uint32_t id) {
		for (auto& it : rb_colliders) {
			if (id == it.ce_id)
				return true;
		}
		return false;
	}

	CollidingStatus getStatus(uint32_t id) {
		for (auto& it : rb_colliders) {
			if (id == it.ce_id)
				return it.ce_status;
		}
		return CollidingStatus::NONE;
	}

	void setCollidingStatus(uint32_t id, CollidingStatus status) {
		for (auto& it : rb_colliders) {
			if (id == it.ce_id)
				it.ce_status = status;
		}
	}

	void removeCollidingEntity(uint32_t id) {
		for (auto& it : rb_colliders) {
			if (id == it.ce_id) {
				rb_colliders.erase(std::remove(rb_colliders.begin(), rb_colliders.end(), it), rb_colliders.end());
			}
		}
	}

	void addCollidingEntity(uint32_t id, CollidingStatus status) {
		Colliding_Entity ce{ id, status };
		rb_colliders.push_back(ce);
	}

	void setAABBCenter(glm::vec3 pt) {
		aabb.setCenter(pt);
	}

	void setAABBScale(glm::vec3 sc) {
		aabb.setScale(sc);
	}

	void setAABBOffset(glm::vec3 os) {
		aabb.setOffset(os);
	}

	glm::vec3 getAABBCenter() {
		return aabb.getCenter();
	}

	glm::vec3 getAABBScale() {
		return aabb.getScale();
	}

	glm::vec3 getAABBOffset() {
		return aabb.getOffset();
	}
};
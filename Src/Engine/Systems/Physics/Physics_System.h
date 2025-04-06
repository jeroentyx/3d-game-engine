#pragma once
#include "../../Core/Interfaces/ISystem.h"
#include "../../Core/Interfaces/IComponent.h"
#include "../../ComponentSystemID.h"
#include "Src/Engine/Systems/Physics/RigidBody.h"
#include "Src/Engine/Systems/Physics/Scene.h"
#include "Tools/FixedTimer.h"
#include "Src/Engine/Core/Manager/SystemManager.h"
#include "Src/Engine/Core/Manager/ComponentManager.h"
#include <map>
#include "tracy\Tracy.hpp"

struct AABB_Values {
	glm::vec3 scale;
	glm::vec3 offset;

	AABB_Values(glm::vec3 s, glm::vec3 o) {
		scale = s;
		offset = o;
	}
};

class Physics_Component : public IComponent 
{
public:
	std::shared_ptr<RigidBody> body;
	friend class Physics_System;

	virtual void Init() override;
	glm::vec3 getAABBscale();
	glm::vec3 getAABBoffset();
	AABB_Values getAABBDataSet();

	//for reflection system's access to these variables
	glm::vec3 pc_linearVelocity;
	glm::vec3 pc_angularVelocity;
	glm::vec3 pc_force;
	glm::vec3 pc_impulse;
	glm::vec3 pc_torque;
	glm::vec3 pc_gravityDirection;
	glm::vec3 pc_faceDirection;
	glm::vec3 pc_aabbCenter;
	glm::vec3 pc_aabbScale;
	glm::vec3 pc_aabbOffset;
	float pc_mass;
	float pc_friction;
	float pc_restitution;
	float pc_solveTime;
	bool pc_hasFriction;
	bool pc_hasGravity;
	bool pc_collisionFlag;
	bool pc_isStatic;
	bool pc_isDynamic;
	bool pc_isKinematic;
	bool pc_prevStatic;
	bool pc_prevDynamic;
	bool pc_prevKinematic;
	bool pc_ignoreCollisionResponse;
	int pc_objectType;

	Physics_Component() : pc_torque{ glm::vec3{0.0f,0.0f,0.0f} },
						  pc_solveTime{0.0f},
						  pc_force{ glm::vec3{0.0f,0.0f,0.0f} },
		                  pc_restitution{0.0f},
						  pc_mass{0.0f},
		                  pc_linearVelocity{0.0f},
						  pc_isStatic{true},
						  pc_isDynamic{false},
						  pc_isKinematic{ false },
						  pc_prevStatic{true},
						  pc_prevDynamic{ false },
						  pc_prevKinematic{ false },
						  pc_impulse{ glm::vec3{0.0f,0.0f,0.0f} },
		                  pc_hasGravity{true},
						  pc_hasFriction{true},
						  pc_gravityDirection{ glm::vec3{0.0f,0.0f,0.0f} },
						  pc_faceDirection{ glm::vec3{1.0f,0.0f,0.0f} },
						  pc_aabbCenter{glm::vec3()},
						  pc_aabbScale{ glm::vec3{1.0f,1.0f,1.0f} },
						  pc_aabbOffset{ glm::vec3{0.0f,0.0f,0.0f} },
						  pc_friction{0.0f},
						  pc_collisionFlag{false},
						  pc_angularVelocity{ glm::vec3{0.0f,0.0f,0.0f} },
					      pc_ignoreCollisionResponse{ false },
						  pc_objectType{0},
		body{ new RigidBody }
	{

	}

	//function to update serialising data
	void compToBody(); //pc data into rb data
	void compToBody2(); //pc data into rb data
	void bodyToComp(); //rb data into pc data
	void bodyToComp2(); //rb data into pc data

	void invertDynamicBooleanBasedOnStaticBoolean();


	property_vtable()
};
property_begin(Physics_Component)
{
	property_var(pc_linearVelocity)
  , property_var(pc_angularVelocity)
  , property_var(pc_force)
  , property_var(pc_impulse)
  , property_var(pc_torque)
  , property_var(pc_gravityDirection)
  , property_var(pc_faceDirection)
  , property_var(pc_aabbCenter)
  , property_var(pc_aabbScale)
  , property_var(pc_aabbOffset)
  , property_var(pc_mass)
  , property_var(pc_friction)
  , property_var(pc_restitution)
  , property_var(pc_solveTime)
  , property_var(pc_hasFriction)
  , property_var(pc_hasGravity)
  , property_var(pc_collisionFlag)
  , property_var(pc_isStatic)
  , property_var(pc_isDynamic)
  , property_var(pc_isKinematic)
  , property_var(pc_ignoreCollisionResponse)
  , property_var(pc_objectType)
}property_vend_h(Physics_Component)

class Physics_System : public ISystem
{
	std::map<EntityID, std::shared_ptr<Physics_Component>> pComps;
	Scene scene;
public:
	void Init();
	void Update();
	void Exit();

	void AddingNewComponent(EntityID entityID, COMPONENTNAME compName, std::shared_ptr<IComponent> compPtr);
	void RemovingComponent(EntityID entityID, COMPONENTNAME compName);
	void ClearAllComponents();
	Scene getScene() { return scene; }
	std::queue<std::pair<uint32_t, uint32_t>> getSurgeQueue() { return scene.getSurgeQueue(); }
	std::map<EntityID, std::shared_ptr<Physics_Component>> getComponentMap();
};


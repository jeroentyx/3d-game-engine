#include "Physics_System.h"

void Physics_System::Init()
{
	//initalise scene
	for (auto& i : pComps) {
		scene.AddBody((i.second->body));
	}
	scene.setFriction(true);
	scene.setGravity(9.81f);
	scene.setSleep(false);
	scene.setUpdate(true);
}

void Physics_System::Update()
{
#ifndef _DEBUG
	//ZoneScopedN("Physics System")
#endif

	for (auto& i : pComps) {
		(void)i;
		scene.RemoveAllBodies();
	}

	for (auto& i : pComps) {
		if (GET_ENTITY_MANAGER.getActive(i.first))
			scene.AddBody((i.second->body));
	}

	for (auto& i : pComps) {
		if (GET_ENTITY_MANAGER.getActive(i.first))
		{
			i.second->body->setPosition(GET_COMPONENT_MANAGER.GetTransformComponent(i.first)->Translation);
			i.second->body->setRotation(GET_COMPONENT_MANAGER.GetTransformComponent(i.first)->Rotation);
			i.second->body->setScale(GET_COMPONENT_MANAGER.GetTransformComponent(i.first)->Scale);
		}
	}
	scene.Step(static_cast<float>(Eos::FixedTimer::GetFixedDeltaTime()));
	//update main transform component based on rb transform
	for (auto& i : pComps) {
		if (GET_ENTITY_MANAGER.getActive(i.first))
		{
			GET_COMPONENT_MANAGER.GetTransformComponent(i.first)->Translation = i.second->body->getTransform().t_pos;
			GET_COMPONENT_MANAGER.GetTransformComponent(i.first)->Rotation = i.second->body->getTransform().getVec3Rot();
			GET_COMPONENT_MANAGER.GetTransformComponent(i.first)->Scale = i.second->body->getTransform().t_scale;
			//i.second->bodyToComp();
			i.second->bodyToComp2();
		}
	}
	

#ifndef _DEBUG
	//FrameMark
#endif
}

void Physics_System::Exit()
{
	//delete?
}

void Physics_System::AddingNewComponent(EntityID entityID, COMPONENTNAME compName, std::shared_ptr<IComponent> compPtr)
{
	if (compName == COMPONENTNAME::Comp_Physics_Component)
	{

		if (!pComps.contains(entityID))
		{
			std::dynamic_pointer_cast<Physics_Component>(compPtr)->body->setTransform(
				GET_COMPONENT_MANAGER.GetTransformComponent(entityID)->Translation,
				GET_COMPONENT_MANAGER.GetTransformComponent(entityID)->Scale,
				GET_COMPONENT_MANAGER.GetTransformComponent(entityID)->Rotation);
			//std::dynamic_pointer_cast<Physics_Component>(compPtr)->body->updateAABB();
			std::dynamic_pointer_cast<Physics_Component>(compPtr)->body->initAABB();
			//std::dynamic_pointer_cast<Physics_Component>(compPtr)->bodyToComp();
			std::dynamic_pointer_cast<Physics_Component>(compPtr)->bodyToComp2();
			std::dynamic_pointer_cast<Physics_Component>(compPtr)->body->setMass();
			std::dynamic_pointer_cast<Physics_Component>(compPtr)->body->setID(entityID);
			pComps.insert({ entityID, std::dynamic_pointer_cast<Physics_Component>(compPtr) });
		}
	}
}

void Physics_System::RemovingComponent(EntityID entityID, COMPONENTNAME compName)
{
	if (compName == COMPONENTNAME::Comp_Physics_Component)
	{
		pComps.erase(entityID);
	}


}

void Physics_System::ClearAllComponents()
{
	pComps.clear();
}

std::map<EntityID, std::shared_ptr<Physics_Component>> Physics_System::getComponentMap() {
	return pComps;
}


void Physics_Component::Init()
{
	//compToBody();
	compToBody2();
}

AABB_Values Physics_Component::getAABBDataSet() {
	AABB_Values v(pc_aabbScale, pc_aabbOffset);
	return v;
}

glm::vec3 Physics_Component::getAABBoffset() {
	return pc_aabbOffset;
}

glm::vec3 Physics_Component::getAABBscale() {
	return pc_aabbScale;
}

void Physics_Component::bodyToComp() {
	pc_mass = body->getMass();
	pc_linearVelocity = body->getLinearVelocity();
	pc_angularVelocity = body->getAngularVelocity();
	pc_force = body->getForce();
	pc_impulse = body->getImpulse();
	pc_torque = body->getTorque();
	pc_gravityDirection = body->getGravityDirection();
	pc_faceDirection = body->getFaceDirection();
	pc_aabbCenter = body->getAABBCenter();
	pc_aabbScale = body->getAABBScale();
	pc_aabbOffset = body->getAABBOffset();
	pc_friction = body->getFriction();
	pc_hasFriction = body->getFrictionFlag();
	pc_hasGravity = body->getGravityFlag();
	pc_restitution = body->getRestitution();
	pc_collisionFlag = body->getCollisionFlag();
	pc_solveTime = body->getSolveTime();
	pc_isStatic = body->getIsStatic();
	pc_isDynamic = body->getIsDynamic();
	pc_isKinematic = body->getIsKinematic();
	pc_prevStatic = body->getIsStatic();
	pc_prevDynamic = body->getIsDynamic();
	pc_prevKinematic = body->getIsKinematic();
	pc_ignoreCollisionResponse = body->getIgnoreCollisionResponse();
	if (body->getObjectType() == ObjectType::NORMALBLOCK)
		pc_objectType = 0;
	else if (body->getObjectType() == ObjectType::GRAVITYBLOCK)
		pc_objectType = 1;
	else if (body->getObjectType() == ObjectType::VIRUS)
		pc_objectType = 2;
	else if (body->getObjectType() == ObjectType::DATA)
		pc_objectType = 3;
	else if (body->getObjectType() == ObjectType::SURGE)
		pc_objectType = 4;
}

void Physics_Component::bodyToComp2() {
	pc_mass = body->getMass();
	pc_linearVelocity = body->getLinearVelocity();
	pc_angularVelocity = body->getAngularVelocity();
	pc_force = body->getForce();
	pc_impulse = body->getImpulse();
	pc_torque = body->getTorque();
	pc_gravityDirection = body->getGravityDirection();
	pc_faceDirection = body->getFaceDirection();
	pc_aabbCenter = body->getAABBCenter();
	pc_aabbScale = body->getAABBScale();
	pc_aabbOffset = body->getAABBOffset();
	pc_friction = body->getFriction();
	pc_hasFriction = body->getFrictionFlag();
	pc_hasGravity = body->getGravityFlag();
	pc_restitution = body->getRestitution();
	pc_collisionFlag = body->getCollisionFlag();
	pc_solveTime = body->getSolveTime();
	pc_isStatic = body->getIsStatic();
	pc_isDynamic = body->getIsDynamic();
	pc_isKinematic = body->getIsKinematic();
	pc_prevStatic = body->getIsStatic();
	pc_prevDynamic = body->getIsDynamic();
	pc_prevKinematic = body->getIsKinematic();
	pc_ignoreCollisionResponse = body->getIgnoreCollisionResponse();
	if (body->getObjectType() == ObjectType::NORMALBLOCK)
		pc_objectType = 0;
	else if (body->getObjectType() == ObjectType::GRAVITYBLOCK)
		pc_objectType = 1;
	else if (body->getObjectType() == ObjectType::VIRUS)
		pc_objectType = 2;
	else if (body->getObjectType() == ObjectType::DATA)
		pc_objectType = 3;
	else if (body->getObjectType() == ObjectType::SURGE)
		pc_objectType = 4;
}

//to be placed in init?
void Physics_Component::compToBody() {
	body->setMass();
	body->setLinearVelocity(pc_linearVelocity);
	body->setAngularVelocity(pc_angularVelocity);
	body->setForce(pc_force);
	body->setImpulse(pc_impulse);
	body->setTorque(pc_torque);
	body->setGravityDirection(pc_gravityDirection);
	body->setFaceDirection(pc_faceDirection);
	body->setAABBCenter(pc_aabbCenter);
	body->setAABBScale(pc_aabbScale);
	body->setAABBOffset(pc_aabbOffset);
	body->setFriction(pc_friction);
	body->setFrictionFlag(pc_hasFriction);
	body->setGravityFlag(pc_hasGravity);
	body->setRestitution(pc_restitution);
	body->setCollisionFlag(pc_collisionFlag);
	body->setSolveTime(pc_solveTime);
	bool changed = false;

	/// <summary>
	///  BACKWARD
	/// </summary>
	body->setIsStatic(pc_isStatic);
	body->setIsDynamic(!pc_isStatic);
	body->setIsKinematic(false);
	body->setIgnoreCollisionResponse(pc_ignoreCollisionResponse);
	if (pc_objectType == 0)
		body->setObjectType(ObjectType::NORMALBLOCK);
	else if (pc_objectType == 1)
		body->setObjectType(ObjectType::GRAVITYBLOCK);
	else if (pc_objectType == 2)
		body->setObjectType(ObjectType::VIRUS);
	else if (pc_objectType == 3)
		body->setObjectType(ObjectType::DATA);
	else if (pc_objectType == 4)
		body->setObjectType(ObjectType::SURGE);


	//init aabb and others
	body->updateAABB();
	body->setBodyTypeInternal();
}

void Physics_Component::compToBody2() {
	body->setMass();
	body->setLinearVelocity(pc_linearVelocity);
	body->setAngularVelocity(pc_angularVelocity);
	body->setForce(pc_force);
	body->setImpulse(pc_impulse);
	body->setTorque(pc_torque);
	body->setGravityDirection(pc_gravityDirection);
	body->setFaceDirection(pc_faceDirection);
	body->setAABBCenter(pc_aabbCenter);
	body->setAABBScale(pc_aabbScale);
	body->setAABBOffset(pc_aabbOffset);
	body->setFriction(pc_friction);
	body->setFrictionFlag(pc_hasFriction);
	body->setGravityFlag(pc_hasGravity);
	body->setRestitution(pc_restitution);
	body->setCollisionFlag(pc_collisionFlag);
	body->setSolveTime(pc_solveTime);
	bool changed = false;
	
	if (!changed && pc_isStatic && !pc_prevStatic) {
		pc_isDynamic = false;
		pc_prevDynamic = false;
		pc_isKinematic = false;
		pc_prevKinematic = false;
		pc_prevStatic = true;
		changed = true;
	}

	if (!changed && pc_isDynamic && !pc_prevDynamic) {
		pc_isStatic = false;
		pc_prevStatic = false;
		pc_isKinematic = false;
		pc_prevKinematic = false;
		pc_prevDynamic = true;
		changed = true;
	}

	if (!changed && pc_isKinematic && !pc_prevKinematic) {
		pc_isDynamic = false;
		pc_prevDynamic = false;
		pc_isStatic = false;
		pc_prevStatic = false;
		pc_prevKinematic = true;
		changed = true;
	}

	body->setIsStatic(pc_isStatic);
	body->setIsDynamic(pc_isDynamic);
	body->setIsKinematic(pc_isKinematic);
	body->setIgnoreCollisionResponse(pc_ignoreCollisionResponse);
	if (pc_objectType == 0)
		body->setObjectType(ObjectType::NORMALBLOCK);
	else if (pc_objectType == 1)
		body->setObjectType(ObjectType::GRAVITYBLOCK);
	else if (pc_objectType == 2)
		body->setObjectType(ObjectType::VIRUS);
	else if (pc_objectType == 3)
		body->setObjectType(ObjectType::DATA);
	else if (pc_objectType == 4)
		body->setObjectType(ObjectType::SURGE);


	//init aabb and others
	body->updateAABB();
	body->setBodyTypeInternal();
}

void Physics_Component::invertDynamicBooleanBasedOnStaticBoolean() {
	pc_isDynamic = !pc_isStatic;
}
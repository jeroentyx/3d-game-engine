	#include "../Src/pch.h"
#include "ScriptWrappers.hpp"
#include "../Src/Engine/Core/Manager/ComponentManager.h"
#include "MonoUtils.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <type_traits>
//#include Physics
#include <mono/metadata/appdomain.h>



namespace Eos::Scripting
{
	float Eos_Time_GetDeltaTime()
	{
		return static_cast<float>(Eos::FrameRateManager::GetInstance().delta());
	}
	float Eos_Time_GetTime()
	{
		return static_cast<float>(Eos::FrameRateManager::GetInstance().GetTime());
	}
	float Eos_Time_GetFixedDeltaTime()
	{
		return static_cast<float>(Eos::FrameRateManager::GetInstance().GetFixedDeltaTime());
	}
	//Console Logging
	void Eos_Console_LogInfo(MonoString* Message)
	{
		std::string msg = ConvertMonoStringToCppString(Message);
		PE_CORE_INFO(msg);
	}
	void Eos_Console_LogWarn(MonoString* Message)
	{
		std::string msg = ConvertMonoStringToCppString(Message);
		PE_CORE_INFO(msg);
	}
	void Eos_Console_LogDebug(MonoString* Message)
	{
		std::string msg = ConvertMonoStringToCppString(Message);
		PE_CORE_INFO(msg);
	}
	void Eos_Console_LogError(MonoString* Message)
	{
		std::string msg = ConvertMonoStringToCppString(Message);
		PE_CORE_INFO(msg);
	}
	void Eos_Console_LogCritical(MonoString* Message)
	{
		std::string msg = ConvertMonoStringToCppString(Message);
		PE_CORE_INFO(msg);
	}

	QuaternionWrapper Eos_Quaternion_Euler(glm::vec3 euler)
	{
		glm::mat4 transform = glm::eulerAngleYXZ(euler.y, euler.x, euler.z);
		glm::quat rotation(transform);
		
		return QuaternionWrapper(rotation.x, rotation.y, rotation.z, rotation.w);
	}

	//Input
	bool Eos_Input_IsKeyPressed(KeyCode key)
	{
		return Inputs::GetInstance().IsKeyPressed(key);
	}
	bool Eos_Input_IsKeyTriggered(KeyCode key)
	{
		return Inputs::GetInstance().IsKeyTriggered(key);
	}
	bool Eos_Input_IsMouseButton(MouseButton button)
	{
		return Inputs::GetInstance().IsMouseButtonPressed(button);
	}
	void Eos_Input_GetMousePosition(glm::vec2* position)
	{
		glm::vec2 res = Inputs::GetInstance().GetMouseNDCCoordinates();
		//glm::vec2 res = glm::vec2(Inputs::GetInstance().GetMouseX(),Inputs::GetInstance().GetMouseY());
		*position = res;
	}
	void Eos_Input_SetMousePosition(glm::vec2* position)
	{
		Inputs::GetInstance().SetMouseNDCCoordinates(*position);
	}


	float Eos_Input_GetAxis(MonoString* axistype)
	{
		std::string msg = ConvertMonoStringToCppString(axistype);
		//Z-Axis
		if (msg == "Vertical")
		{
			if(Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::W) || 
				Inputs::GetInstance().IsButtonPressed(Eos::ControllerBtn::UP_LA))
				return -1.0f;
			if (Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::S) ||
				Inputs::GetInstance().IsButtonPressed(Eos::ControllerBtn::DOWN_LA))
				return 1.0f;
			else
				return 0.0f;
		}
		//X-Axis
		if (msg == "Horizontal")
		{
			if (Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::A) || 
				Inputs::GetInstance().IsButtonPressed(Eos::ControllerBtn::LEFT_LA))
				return -1.0f;
			if (Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::D) ||
				Inputs::GetInstance().IsButtonPressed(Eos::ControllerBtn::RIGHT_LA))
				return 1.0f;
			else
				return 0.0f;
		}
		if (msg == "MouseX")
		{
			return static_cast<float>(Context::GetInstance().cursorDelta().first);
		}
		if (msg == "MouseY")
		{
			return static_cast<float>(Context::GetInstance().cursorDelta().second);
		}

		else
			return 0;


	}
	Cursor Eos_Input_GetCursorMode()
	{
		return Inputs::GetInstance().GetCursorMode();
	}
	void Eos_Input_SetCursorMode(Cursor Mode)
	{
		Inputs::GetInstance().SetCursorMode(Mode);
	}
	float Eos_Input_GetMouseWheel()
	{
		return static_cast<float>(Context::GetInstance().scrollData());
	}
	bool Eos_Input_IsJoystickConnected()
	{

		return Inputs::GetInstance().IsJoystickConnected();
	}
	bool Eos_Input_IsControllerButtonPressed(ControllerBtn btn)
	{
		return Inputs::GetInstance().IsButtonPressed(btn);
	}
	void Eos_Input_PlayerControllerMovement(glm::vec2* pos)
	{
	}
	void Eos_Input_CameraControllerMovement(glm::vec2* position)
	{
		//Get mouse position
		auto input = Inputs::GetInstance().GetMousePosition();
		if (Inputs::GetInstance().IsButtonPressed(ControllerBtn::DOWN_RA))
		{
			input.second += 25.0;
			PE_CORE_INFO("DOWN RA");
			Inputs::GetInstance().SetCursorPos(input.first,input.second);
			
		}
		if (Inputs::GetInstance().IsButtonPressed(ControllerBtn::UP_RA))
		{
			PE_CORE_INFO("UP RA");
			input.second += -25.0;
			Inputs::GetInstance().SetCursorPos(input.first, input.second);
		}
		if (Inputs::GetInstance().IsButtonPressed(ControllerBtn::LEFT_RA))
		{
			input.first += -25.0;
			PE_CORE_INFO("LEFT RA");
			Inputs::GetInstance().SetCursorPos(input.first, input.second);
		}
		if (Inputs::GetInstance().IsButtonPressed(ControllerBtn::RIGHT_RA))
		{
			PE_CORE_INFO("RIGHT RA");
			input.first += 25.0;
			Inputs::GetInstance().SetCursorPos(input.first, input.second);
		}

		*position = glm::vec2(Context::GetInstance().cursorDelta().first, Context::GetInstance().cursorDelta().second);
	   
	}
	void Eos_Entity_CreateComponent(EntityWrapper EntityID, MonoString* type)
	{
		UNREFERENCED_PARAMETER(EntityID);
		UNREFERENCED_PARAMETER(type);
		//ComponentManager::GetInstance().AddComponent(EntityID, compname);
	}
	bool Eos_Entity_HasComponent(EntityWrapper Entity, MonoString* type)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);

		if (ID._id == -1)
			return false;

		std::string compname = mono_string_to_utf8(type);

		if (compname == "Transform")
			return ComponentManager::GetInstance().EntityHaveComponent(ID, COMPONENTNAME::Comp_Transform_Component);
		else if (compname == "RigidBody")
			return ComponentManager::GetInstance().EntityHaveComponent(ID, COMPONENTNAME::Comp_Physics_Component);
		else if (compname == "Collider")
			return ComponentManager::GetInstance().EntityHaveComponent(ID, COMPONENTNAME::Comp_Physics_Component);
		else if (compname == "Camera")
			return ComponentManager::GetInstance().EntityHaveComponent(ID, COMPONENTNAME::Comp_Camera_Component);
		else if (compname == "Animation")
			return ComponentManager::GetInstance().EntityHaveComponent(ID, COMPONENTNAME::Comp_Animated_Mesh_Component);
		else if (compname == "AudioComponent")
			return ComponentManager::GetInstance().EntityHaveComponent(ID, COMPONENTNAME::Comp_Audio_Component);
		else if (compname == "UIComponent")
			return ComponentManager::GetInstance().EntityHaveComponent(ID, COMPONENTNAME::Comp_GUI_Component);
		else if (compname == "StaticMeshComponent")
			return ComponentManager::GetInstance().EntityHaveComponent(ID, COMPONENTNAME::Comp_Static_Mesh_Component);
		
	
		return false;

	}
	//Will work in the future
	bool Eos_EntityExits(EntityWrapper EntityID)
	{
		UNREFERENCED_PARAMETER(EntityID);
		return false;
	}

	void Eos_Entity_SetActive(EntityWrapper entity, bool active)
	{
		EntityID ID(entity._id, entity._generations);
		GET_ENTITY_MANAGER.setActive(ID, active);
	}

	

	void Eos_Entity_DeleteComponent(EntityWrapper entityID)
	{
		EntityID ID = EntityID(entityID._id, entityID._generations);
		EntityManager::GetInstance().DeleteEntity(ID);
	}

	EntityWrapper Eos_Entity_FindWithTag(MonoString* Message)
	{
		std::string msg = ConvertMonoStringToCppString(Message);

		auto& map = GET_ENTITY_MANAGER.usedIDs;

		for (auto pair : map)
		{
			if (pair.second == msg)
			{
				EntityWrapper entity;
				entity._id = pair.first._id;
				entity._generations = pair.first._generations;
				return entity;
			}
		}
		
		return EntityWrapper{ 0,0 };
	}

	MonoArray* Eos_Entity_FindGameObjectsWithTag(MonoString* Message)
	{
		std::vector<EntityWrapper> returnList;
		MonoArray* monoArray;
		std::string msg = ConvertMonoStringToCppString(Message);

		auto& map = GET_ENTITY_MANAGER.usedIDs;

		for (auto pair : map)
		{
			if (pair.second == msg)
			{
				EntityWrapper entity;
				entity._id = pair.first._id;
				entity._generations = pair.first._generations;
				returnList.push_back(entity);
			}
		}

		monoArray = Scripting::ConvertCppContainersToMonoArray(returnList);

		return monoArray;
	}

	MonoArray* Eos_Entity_GetAllChild(EntityWrapper entity)
	{
		std::vector<EntityWrapper> returnList;
		MonoArray* monoArray;
		EntityID ID = EntityID(entity._id, entity._generations);

		auto vect = GET_ENTITY_MANAGER.findAllChild(ID, true);

		for (auto ent : vect)
		{
			EntityWrapper entity;
			entity._id = ent._id;
			entity._generations = ent._generations;
			returnList.push_back(entity);
		}

		monoArray = Scripting::ConvertCppContainersToMonoArray(returnList);

		return monoArray;
	}

	MonoString* Eos_Entity_GetName(EntityWrapper entity)
	{
		EntityID ID = EntityID(entity._id, entity._generations);
		auto& map = GET_ENTITY_MANAGER.usedIDs;
		if (map.find(ID) != map.end())
		{
			MonoString* msg = ConvertCppStringToMonoString(ScriptSystem::GetDomain(), map.find(ID)->second);
			return msg;
		}
		
		return nullptr;
	}

	void Eos_Entity_SetName(EntityWrapper entity, MonoString* Message)
	{
		UNREFERENCED_PARAMETER(entity);
		UNREFERENCED_PARAMETER(Message);
	}

	EntityWrapper Eos_Entity_GetParent(EntityWrapper entity)
	{
		EntityID ID = EntityID(entity._id, entity._generations);
		EntityID parent = EntityManager::GetInstance().getParentOfChild(ID);
		return EntityWrapper(parent._id, parent._generations);
	}

	void Eos_Entity_SetParent(EntityWrapper child, EntityWrapper parent)
	{
		EntityID child1 = EntityID(child._id, child._generations);
		EntityID parent1 = EntityID(parent._id, parent._generations);

		EntityManager::GetInstance().attachChildtoParent(parent1, child1);
	}

	void Eos_Entity_DetachParent(EntityWrapper child)
	{
		EntityID child1 = EntityID(child._id, child._generations);

		EntityManager::GetInstance().detachChildfromParent(child1);
	}

	void Eos_StaticMeshComponent_Load(EntityWrapper entt, MonoString* filepath)
	{
		EntityID ID = EntityID(entt._id, entt._generations);
		std::string compname = mono_string_to_utf8(filepath);
		auto comp = std::dynamic_pointer_cast<Static_Mesh_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Static_Mesh_Component));
		comp->directory = compname;
	}

	void Eos_Entity_Destroy(EntityWrapper entity)
	{
		EntityID ID = EntityID(entity._id, entity._generations);
		EntityManager::GetInstance().DeleteEntity(ID);
	}
	
	void Eos_TransformComponent_GetTransform(EntityWrapper Entity, std::shared_ptr<Transform_Component> outTransform)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);

		if (!outTransform)
			PE_CORE_ERROR("No Transform can be found");
			
		outTransform = std::dynamic_pointer_cast<Transform_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Transform_Component));
	}
	void Eos_TransformComponent_SetTransform(EntityWrapper Entity, std::shared_ptr<Transform_Component> inTransform)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto component = std::dynamic_pointer_cast<Transform_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Transform_Component));
		//EosAssert(component, "Transform Component Not Found");
		component = inTransform;
	}
	void Eos_TransformComponent_GetTranslation(EntityWrapper Entity, glm::vec3* outTranslation)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto component = std::dynamic_pointer_cast<Transform_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Transform_Component));
		//EosAssert(component, "Transform Component Not Found");
		*outTranslation = component->Translation;
	}
	void Eos_TransformComponent_SetTranslation(EntityWrapper Entity, glm::vec3* inTranslation)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto component = std::dynamic_pointer_cast<Transform_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Transform_Component));
		//EosAssert(component, "Transform Component Not Found");
		component->Translation = *inTranslation;
	}
	void Eos_TransformComponent_GetRotation(EntityWrapper Entity, QuaternionWrapper* outRotation)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto component = std::dynamic_pointer_cast<Transform_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Transform_Component));
		//EosAssert(component, "Transform Component Not Found");

		glm::vec3 euler = component->Rotation;
		
		
		glm::quat myquat(euler);
		outRotation->x = myquat.x;
		outRotation->y = myquat.y;
		outRotation->z = myquat.z;
		outRotation->w = myquat.w;

	}
	void Eos_TransformComponent_SetRotation(EntityWrapper Entity, QuaternionWrapper* inRotation)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto component = std::dynamic_pointer_cast<Transform_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Transform_Component));
		//EosAssert(component, "Transform Component Not Found");

		glm::qua rotationQua{ inRotation->w,inRotation->x,inRotation->y, inRotation->z };
		glm::vec3 rotationtest = glm::eulerAngles(rotationQua);

		component->Rotation = rotationtest;
	}
	void Eos_TransformComponent_GetScale(EntityWrapper Entity, glm::vec3* outScale)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto component = std::dynamic_pointer_cast<Transform_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Transform_Component));
		//EosAssert(component, "Transform Component Not Found");
		*outScale = component->Scale;
	}
	void Eos_TransformComponent_SetScale(EntityWrapper Entity, glm::vec3* inScale)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto component = std::dynamic_pointer_cast<Transform_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Transform_Component));
		//EosAssert(component , "Transform Component Not Found");
		component->Scale = *inScale;
	}


	//Physics
	void Eos_Physics_SetFixedTimeStep(float fixedTimeStep)
	{
		UNREFERENCED_PARAMETER(fixedTimeStep);
		//Need to get the physics system to set the fts
	}

	float Eos_Physics_GetFixedTimeStep()
	{
		return static_cast<float>(Eos::FixedTimer::GetFixedDeltaTime());
	}


	void Eos_Physics_SetFriction(glm::vec3* outGravity)
	{
		UNREFERENCED_PARAMETER(outGravity);
	}

	void Eos_Physics_GetFriction(glm::vec3* outGravity)
	{
		UNREFERENCED_PARAMETER(outGravity);
	}



	//RigidBody

	void Eos_PhysicsComponent_UseGravity(EntityWrapper Entity, bool use)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		comp->body->setGravityFlag(use);
	}

	void Eos_PhysicsComponent_SetGravity(EntityWrapper Entity, glm::vec3* inGravity)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		comp->body->setGravityDirection(*inGravity);
	}

	void Eos_PhysicsComponent_GetGravity(EntityWrapper Entity, glm::vec3* outGravity)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		*outGravity = comp->body->getGravityDirection();
	}

	void Eos_PhysicsComponent_RotateAlongAxis(EntityWrapper Entity, glm::vec3* angle)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		comp->body->rotateAlongAxis(*angle);
	}

	void Eos_PhysicsComponent_SetStatic(EntityWrapper Entity, bool use)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		comp->body->setIsStatic(use);
	}

	void Eos_PhysicsComponent_SetDyanmic(EntityWrapper Entity, bool use)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		comp->body->setIsDynamic(use);
	}

	void Eos_PhysicsComponent_SetKinematics(EntityWrapper Entity, bool use)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		comp->body->setIsKinematic(use);
	}

	bool Eos_PhysicsComponent_GetIsTrigger(EntityWrapper entity1)
	{
		EntityID ID = EntityID(entity1._id, entity1._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		return comp->body->getCollisionFlag();
	}

	void Eos_PhysicsComponent_SetIsTrigger(EntityWrapper entity1, bool isTriggered)
	{
		EntityID ID = EntityID(entity1._id, entity1._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		comp->body->setCollisionFlag(isTriggered);
	}

	BodyType Eos_PhysicsComponent_GetBodyType(EntityWrapper Entity)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		return comp->body->getBodyType();
	}

	void Eos_PhysicsComponent_AddForce(EntityWrapper Entity, glm::vec3* force)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		comp->body->applyForce(*force);
	}

	void Eos_PhysicsComponent_MovePosition(EntityWrapper Entity, glm::vec3* pos)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		comp->body->setPosition(*pos);
	}

	float Eos_PhysicsComponent_GetMass(EntityWrapper Entity)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		return comp->body->getMass();
	}

	void Eos_PhysicsComponent_SetMass(EntityWrapper Entity, float mass)
	{
		UNREFERENCED_PARAMETER(mass);
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		comp->body->setMass();
	}

	void Eos_PhysicsComponent_SetRotate(EntityWrapper Entity, QuaternionWrapper* rotation)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		//Converts Quaternion to Vector3
		glm::qua rotationQua{ rotation->w,rotation->x,rotation->y, rotation->z };
		glm::vec3 rotationtest = glm::eulerAngles(rotationQua);

		comp->body->setRotation(rotationtest);
	}

	void Eos_PhysicsComponent_GetRotate(EntityWrapper Entity, QuaternionWrapper* outrotation)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		glm::vec3 euler = comp->body->getRotation();
		glm::quat myquat(euler);
		outrotation->x = myquat.x;
		outrotation->y = myquat.y;
		outrotation->z = myquat.z;
		outrotation->w = myquat.w;
	}

	void Eos_PhysicsComponent_GetLinearVelocity(EntityWrapper Entity, glm::vec3* outVelocity)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		*outVelocity = comp->body->getLinearVelocity();
	}

	void Eos_PhysicsComponent_SetLinearVelocity(EntityWrapper Entity, glm::vec3* _velocity)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		comp->body->setLinearVelocity(*_velocity);
		
	}

	void Eos_PhysicsComponent_GetAngularVelocity(EntityWrapper Entity, glm::vec3* outVelocity)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		*outVelocity = comp->body->getAngularVelocity();
	}

	void Eos_PhysicsComponent_SetAngularVelocity(EntityWrapper Entity, glm::vec3* _velocity)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Physics_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Physics_Component));
		comp->body->setAngularVelocity(*_velocity);
	}

	/*void Eos_Camera_SetMainCameraTransition()
	{
		
		
	}

	void Eos_Camera_GetMainCameraCenter(glm::vec3* outCenter)
	{
		std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS));
		*outCenter = std::remove_const<const glm::vec3>::type (rs->m_persCamera.GetCenter());
	}

	void Eos_Camera_SetMainCameraCenter(glm::vec3* inCenter)
	{
		std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS));
		rs->m_persCamera.SetCenter(*inCenter);
	}

	void Eos_Camera_GetMainCameraEye(glm::vec3* OutEye)
	{
		std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS));
		*OutEye = std::remove_const<const glm::vec3>::type(rs->m_persCamera.GetEye());
	}

	void Eos_Camera_SetMainCameraEye(glm::vec3* InEye)
	{
		std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS));
		rs->m_persCamera.SetEye(*InEye);
	}

	void Eos_Camera_GetMainCameraUp(glm::vec3* OutUp)
	{
		std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS));
		*OutUp = std::remove_const<const glm::vec3>::type(rs->m_persCamera.GetUp());
	}

	void Eos_Camera_SetMainCameraUp(glm::vec3* InUp)
	{
		std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS));
		rs->m_persCamera.SetCenter(*InUp);
	}*/

	AnimationStates Eos_AnimationComponent_GetState(EntityWrapper Entity)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Animated_Mesh_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Animated_Mesh_Component));
		return comp->GetState();
	}

	void Eos_AnimationComponent_SetState(EntityWrapper Entity, MonoString* state)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Animated_Mesh_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Animated_Mesh_Component));
		comp->SetCurrentState(ConvertMonoStringToCppString(state));
	}

	bool Eos_AnimationComponent_IsAnimationDone(EntityWrapper Entity)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Animated_Mesh_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Animated_Mesh_Component));
		return comp->IsAnimationCompleted();
	}

	void Eos_AnimationComponent_ResetAnimation(EntityWrapper Entity)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Animated_Mesh_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Animated_Mesh_Component));
		comp->ResetCompletedState();
	}
	
	void Eos_Camera_GetCameraPosition(EntityWrapper entity, glm::vec3* outpos)
	{
		EntityID ID = EntityID(entity._id, entity._generations);
		auto comp = std::dynamic_pointer_cast<Camera_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Camera_Component));
		*outpos = comp->GetPosition();
	}

	void Eos_Camera_SetCameraPosition(EntityWrapper entity, glm::vec3* inpos)
	{
		EntityID ID = EntityID(entity._id, entity._generations);
		auto comp = std::dynamic_pointer_cast<Camera_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Camera_Component));
		comp->SetPosition(*inpos);
	}

	void Eos_Camera_GetTargetPosition(EntityWrapper entity, glm::vec3* outpos)
	{
		EntityID ID = EntityID(entity._id, entity._generations);
		auto comp = std::dynamic_pointer_cast<Camera_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Camera_Component));
		*outpos = comp->GetTargetPosition();
	}

	void Eos_Camera_SetTargetPosition(EntityWrapper entity, glm::vec3* inpos)
	{
		EntityID ID = EntityID(entity._id, entity._generations);
		auto comp = std::dynamic_pointer_cast<Camera_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Camera_Component));
		comp->SetTargetPosition(*inpos);
	}

	void Eos_Camera_GetTargetRotation(EntityWrapper entity, glm::vec3* outrot)
	{
		EntityID ID = EntityID(entity._id, entity._generations);
		auto comp = std::dynamic_pointer_cast<Camera_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Camera_Component));
		comp->SetTargetRotation(*outrot);
	}

	void Eos_Camera_SetTargetRotation(EntityWrapper entity, glm::vec3* inrot)
	{
		EntityID ID = EntityID(entity._id, entity._generations);
		auto comp = std::dynamic_pointer_cast<Camera_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Camera_Component));
		comp->SetTargetRotation(*inrot);
	}

	int Eos_Camera_GetScreenSizeX()
	{
		return Context::GetInstance().scr_width;
	}

	int Eos_Camera_GetScreenSizeY()
	{
		return Context::GetInstance().scr_height;
	}

	bool Eos_Camera_GetActive(EntityWrapper entity)
	{
		EntityID ID = EntityID(entity._id, entity._generations);
		auto comp = std::dynamic_pointer_cast<Camera_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Camera_Component));
		return comp->GetActive();
	}

	void Eos_Camera_SetActive(EntityWrapper entity, bool active)
	{
		EntityID ID = EntityID(entity._id, entity._generations);
		auto comp = std::dynamic_pointer_cast<Camera_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Camera_Component));
		comp->SetActive(active);
	}

	

	void Eos_Audio_SetSFXvol(float vol)
	{
		GET_AUDIO_SYSTEM->SetSFXVol(vol);
	}

	void Eos_AudioComponent_PlayBGM(EntityWrapper Entity, MonoString* audioName)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Audio_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Audio_Component));
		comp->PlayBGM(ConvertMonoStringToCppString(audioName));
	}

	void Eos_AudioComponent_PlaySFX(EntityWrapper Entity, MonoString* audioName)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Audio_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_Audio_Component));
		comp->PlaySFX(ConvertMonoStringToCppString(audioName));
	}

	void Eos_Audio_IncreaseBGMvol()
	{
		GET_AUDIO_SYSTEM->IncreaseBGMVol();
	}

	void Eos_Audio_IncreaseSFXvol()
	{
		GET_AUDIO_SYSTEM->IncreaseSFXVol();
	}

	void Eos_Audio_IncreaseMastervol()
	{
		GET_AUDIO_SYSTEM->IncreaseMasterVol();
	}

	void Eos_Audio_DecreaseBGMvol()
	{
		GET_AUDIO_SYSTEM->DecreaseBGMVol();
	}

	void Eos_Audio_DecreaseSFXvol()
	{
		GET_AUDIO_SYSTEM->DecreaseSFXVol();
	}

	void Eos_Audio_DecreaseMastervol()
	{
		GET_AUDIO_SYSTEM->DecreaseMasterVol();
	}

	void Eos_Audio_Mute()
	{
		GET_AUDIO_SYSTEM->Mute();
	}

	void Eos_Audio_Unmute()
	{
		GET_AUDIO_SYSTEM->Unmute();
	}
	void Eos_SceneManager_LoadNextScene(MonoString* scenefile)
	{
		std::string msg = ConvertMonoStringToCppString(scenefile);
		SceneManager::GetInstance().SetNextScene(msg);
	}
	void Eos_ScriptSystem_StopRunTimeScript()
	{
		ScriptSystem::OnRuntimeStop();
	}
	ENGINESTATE Eos_CoreManager_GetGameState()
	{
		return CoreManager::GetInstance().getEngineState();
	}
	void Eos_CoreManager_SetGameState(ENGINESTATE state)
	{
		if (state == ENGINESTATE::PLAY)
		{
			GfxImGui::GetInstance().isPlaying = true;
		}
		else
		{
			GfxImGui::GetInstance().isPlaying = false;
		}
		CoreManager::GetInstance().setEngineState(state);
	}


	EntityWrapper Eos_PrefabManager_Instantiate(MonoString* tileName, glm::vec3* position, glm::vec3* rotation)
	{
		std::string tile = ConvertMonoStringToCppString(tileName);
		auto Entity = PrefabManager::GetInstance().CreatePrefabInstance(tile);

		auto transform = std::dynamic_pointer_cast<Transform_Component>(ComponentManager::GetInstance().GetComponent(Entity, COMPONENTNAME::Comp_Transform_Component));

		transform->Translation = *position;
		transform->Rotation    = *rotation;

		return EntityWrapper{ Entity._id, Entity._generations };

	}
	float Eos_GUIComponent_GetOpacity(EntityWrapper Entity)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<GUI_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_GUI_Component));
		return comp->GetOpacity();
	}
	void Eos_GUIComponent_SetOpacity(EntityWrapper Entity, float opaque)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<GUI_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_GUI_Component));
		comp->SetOpacity(opaque);
	}
	void Eos_GUIComponent_GetColor(EntityWrapper Entity, glm::vec3* inColour)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<GUI_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_GUI_Component));
		*inColour = comp->GetColor();
	}
	void Eos_GUIComponent_SetColor(EntityWrapper Entity, glm::vec3* outColour)
	{
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<GUI_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_GUI_Component));
		comp->SetColor(*outColour);

	}
	void Eos_GUIComponent_SetDepth(EntityWrapper Entity, float d_value)
	{
		//Do for both cpp and c# side
		//Set in cpp side
		EntityID entt;
		entt._id = Entity._id;
		entt._generations = Entity._generations;
		std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(entt);
		tc.get()->Translation.z = d_value;
		
	}
	float Eos_GUIComponent_GetDepth(EntityWrapper Entity)
	{
		EntityID entt = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Transform_Component>(ComponentManager::GetInstance().GetComponent(entt, COMPONENTNAME::Comp_Transform_Component));
		return comp.get()->Translation.z;
	}
	MonoString* Eos_GUIComponent_GetFile(EntityWrapper Entity)
	{
		MonoString* s;
		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<GUI_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_GUI_Component));
		s = ConvertCppStringToMonoString(ScriptSystem::GetDomain(), comp->GetFilePath());
		return s;
	}
	void Eos_GUIComponent_SetFile(EntityWrapper Entity, MonoString* string)
	{

		EntityID ID = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<GUI_Component>(ComponentManager::GetInstance().GetComponent(ID, COMPONENTNAME::Comp_GUI_Component));
		std::string temp = ConvertMonoStringToCppString(string);
		comp->SetFilePath(temp);

	}
	void Eos_ParticleComponent_SetNumber(EntityWrapper Entity, int d_value)
	{
		EntityID entt = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Particle_Component>(ComponentManager::GetInstance().GetComponent(entt, COMPONENTNAME::Comp_Particle_Component));
		comp->nParticles = d_value;
	}
	int Eos_ParticleComponent_GetNumber(EntityWrapper Entity)
	{
		EntityID entt = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Particle_Component>(ComponentManager::GetInstance().GetComponent(entt, COMPONENTNAME::Comp_Particle_Component));
		return comp->nParticles;
	}
	void Eos_ParticleComponent_SetSize(EntityWrapper Entity, float d_value)
	{
		EntityID entt = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Particle_Component>(ComponentManager::GetInstance().GetComponent(entt, COMPONENTNAME::Comp_Particle_Component));
		comp->particleSize = d_value;
	}
	float Eos_ParticleComponent_GetSize(EntityWrapper Entity)
	{
		EntityID entt = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Particle_Component>(ComponentManager::GetInstance().GetComponent(entt, COMPONENTNAME::Comp_Particle_Component));
		return comp->particleSize;
	}
	void Eos_ParticleComponent_SetLife(EntityWrapper Entity, float d_value)
	{
		EntityID entt = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Particle_Component>(ComponentManager::GetInstance().GetComponent(entt, COMPONENTNAME::Comp_Particle_Component));
		comp->particleLifeTime = d_value;
	}
	float Eos_ParticleComponent_GetLife(EntityWrapper Entity)
	{
		EntityID entt = EntityID(Entity._id, Entity._generations);
		auto comp = std::dynamic_pointer_cast<Particle_Component>(ComponentManager::GetInstance().GetComponent(entt, COMPONENTNAME::Comp_Particle_Component));
		return comp->particleLifeTime;
	}
}



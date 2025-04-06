#pragma once
#include "../Src/pch.h"
#include "ScriptData.h"
#include "../Src/Engine/Systems/Physics/BodyType.h"
#include "../Src/Engine/Components/Transform/TransformComponent.h"
#include "../Src/Engine/Core/Manager/EntityManager.h"
#include "../Src/Engine/Core/Manager/PrefabManager.h"
#include "../Src/Inputs/Inputs.h"
#include <mono/metadata/object.h>
#include <mono/metadata/object-forward.h>

namespace Eos::Scripting
{
	//Time
	float Eos_Time_GetDeltaTime();
	float Eos_Time_GetTime();
	float Eos_Time_GetFixedDeltaTime();


	//Console Logging
	void Eos_Console_LogInfo(MonoString* Message);
	void Eos_Console_LogWarn(MonoString* Message);
	void Eos_Console_LogDebug(MonoString* Message);
	void Eos_Console_LogError(MonoString* Message);
	void Eos_Console_LogCritical(MonoString* Message);


	//Quaternion Math
	QuaternionWrapper Eos_Quaternion_Euler(glm::vec3 euler);

	//Input
	bool Eos_Input_IsKeyPressed(KeyCode key);
	bool Eos_Input_IsKeyTriggered(KeyCode key);
	bool Eos_Input_IsMouseButton(MouseButton button);
	void Eos_Input_GetMousePosition(glm::vec2* position);
	void Eos_Input_SetMousePosition(glm::vec2* position);
	float Eos_Input_GetAxis(MonoString * axistype);
	Cursor Eos_Input_GetCursorMode();
	void Eos_Input_SetCursorMode(Cursor Mode);
	float Eos_Input_GetMouseWheel();

	//Controller
	bool Eos_Input_IsJoystickConnected();
	bool Eos_Input_IsControllerButtonPressed(ControllerBtn btn);
	void Eos_Input_CameraControllerMovement(glm::vec2* pos);
	void Eos_Input_PlayerControllerMovement(glm::vec2* pos);

	//Entity
	void Eos_Entity_CreateComponent(EntityWrapper EntityID, MonoString* type);
	bool Eos_Entity_HasComponent(EntityWrapper EntityID, MonoString* type);
	bool Eos_EntityExits(EntityWrapper EntityID);
	void Eos_Entity_SetActive(EntityWrapper EntityID, bool active);
	void Eos_Entity_DeleteComponent(EntityWrapper EntityID);
	EntityWrapper Eos_Entity_FindWithTag(MonoString* Message);
	MonoArray* Eos_Entity_FindGameObjectsWithTag(MonoString* Message);
	MonoArray* Eos_Entity_GetAllChild(EntityWrapper entity);
	MonoString* Eos_Entity_GetName(EntityWrapper entity);
	void Eos_Entity_SetName(EntityWrapper entity, MonoString* Message);
	EntityWrapper Eos_Entity_GetParent(EntityWrapper entity);
	void Eos_Entity_SetParent(EntityWrapper entity, EntityWrapper parent);
	void Eos_Entity_DetachParent(EntityWrapper entity);
	void Eos_Entity_CloneEntity(EntityWrapper entity);


	void Eos_StaticMeshComponent_Load(EntityWrapper entt, MonoString* filepath);


	//Will make it happen
	void Eos_Entity_Destroy(EntityWrapper entity);

	//Transform
	void Eos_TransformComponent_GetTransform(EntityWrapper EntityID, std::shared_ptr<Transform_Component> outTransform);
	void Eos_TransformComponent_SetTransform(EntityWrapper EntityID, std::shared_ptr<Transform_Component> inTransform);
	void Eos_TransformComponent_GetTranslation(EntityWrapper EntityID, glm::vec3* outTranslation);
	void Eos_TransformComponent_SetTranslation(EntityWrapper EntityID, glm::vec3* inTranslation);
	void Eos_TransformComponent_GetRotation(EntityWrapper EntityID, QuaternionWrapper* outRotation);
	void Eos_TransformComponent_SetRotation(EntityWrapper EntityID, QuaternionWrapper* inRotation);
	void Eos_TransformComponent_GetScale(EntityWrapper EntityID, glm::vec3* outScale);
	void Eos_TransformComponent_SetScale(EntityWrapper EntityID, glm::vec3* inScale);

	//Physics System
	//Raycasting --- 
	//void Eos_Physics_Raycast(RaycastHit* hit, glm::vec3* origin, glm::vec3* direction, float maxDistance);
	void Eos_Physics_SetFixedTimeStep(float fixedTimeStep);
	float Eos_Physics_GetFixedTimeStep();
	void Eos_Physics_SetFriction(glm::vec3* outGravity);
	void Eos_Physics_GetFriction(glm::vec3* outGravity);

	//Double check with Irfan if he have these functions
	//void Eos_Physics_SetSolverIterations(unsigned int solverIterations);
	//unsigned int  Eos_Physics_GetSolverIterations();
	//void Eos_Physics_SetSolverVelocityIterations(unsigned int  solverVelocityIterations);
	//unsigned int  Eos_Physics_GetSolverVelocityIterations();


	//RigidBody	
	void Eos_PhysicsComponent_AddForce(EntityWrapper EntityID, glm::vec3* force);
	void Eos_PhysicsComponent_MovePosition(EntityWrapper EntityID, glm::vec3* pos);
	void Eos_PhysicsComponent_GetLinearVelocity(EntityWrapper EntityID, glm::vec3* outVelocity);
	void Eos_PhysicsComponent_SetLinearVelocity(EntityWrapper EntityID, glm::vec3* velocity);
	void Eos_PhysicsComponent_GetAngularVelocity(EntityWrapper EntityID, glm::vec3* outVelocity);
	void Eos_PhysicsComponent_SetAngularVelocity(EntityWrapper EntityID, glm::vec3* velocity);
	void Eos_PhysicsComponent_SetRotate(EntityWrapper EntityID, QuaternionWrapper* inrotation);
	void Eos_PhysicsComponent_GetRotate(EntityWrapper EntityID, QuaternionWrapper* outrotation);
	float Eos_PhysicsComponent_GetMass(EntityWrapper EntityID);
	void Eos_PhysicsComponent_SetMass(EntityWrapper EntityID, float mass);
	BodyType Eos_PhysicsComponent_GetBodyType(EntityWrapper EntityID);
	void Eos_PhysicsComponent_UseGravity(EntityWrapper EntityID, bool use);
	void Eos_PhysicsComponent_SetGravity(EntityWrapper EntityID, glm::vec3* inGravity);
	void Eos_PhysicsComponent_GetGravity(EntityWrapper EntityID, glm::vec3* outGravity);
	void Eos_PhysicsComponent_RotateAlongAxis(EntityWrapper EntityID, glm::vec3* angle);
	void Eos_PhysicsComponent_SetStatic(EntityWrapper EntityID, bool use);
	void Eos_PhysicsComponent_SetDyanmic(EntityWrapper EntityID, bool use);
	void Eos_PhysicsComponent_SetKinematics(EntityWrapper EntityID, bool use);
	


	//Collisionbox
	bool Eos_PhysicsComponent_GetIsTrigger(EntityWrapper entity1);
	void Eos_PhysicsComponent_SetIsTrigger(EntityWrapper entity1, bool isTriggered);
	

	//Deprectated
	//Camera
	/*void      Eos_Camera_GetMainCameraCenter(glm::vec3* outCenter);
	void      Eos_Camera_SetMainCameraCenter(glm::vec3* inCenter);
	void      Eos_Camera_GetMainCameraEye(glm::vec3* OutEye);
	void      Eos_Camera_SetMainCameraEye(glm::vec3* InEye);
	void      Eos_Camera_GetMainCameraUp(glm::vec3* OutUp);
	void      Eos_Camera_SetMainCameraUp(glm::vec3* InUp);*/


	void      Eos_Camera_GetCameraPosition(EntityWrapper entity, glm::vec3* outpos);
	void      Eos_Camera_SetCameraPosition(EntityWrapper entity, glm::vec3* inpos);
	void      Eos_Camera_GetTargetPosition(EntityWrapper entity, glm::vec3* outpos);
	void      Eos_Camera_SetTargetPosition(EntityWrapper entity, glm::vec3* inpos);
	void      Eos_Camera_GetTargetRotation(EntityWrapper entity, glm::vec3* outrot);
	void      Eos_Camera_SetTargetRotation(EntityWrapper entity, glm::vec3* inrot);
	int       Eos_Camera_GetScreenSizeX();
	int       Eos_Camera_GetScreenSizeY();
	bool      Eos_Camera_GetActive(EntityWrapper entity);
	void      Eos_Camera_SetActive(EntityWrapper entity, bool active);

	/*
	* FOR FUTURE REFERENCES
	void      Eos_Camera_SetMainCameraTransition(glm::vec3* inTranslation);
	void	  Eos_Camera_SetAsPrimary(EntityWrapper EntityID);
	int		  Eos_Camera_GetScreenSizeX();
	int		  Eos_Camera_GetScreenSizey();
	glm::vec3 Eos_Camera_GetFront(EntityWrapper EntityID);
	glm::vec3 Eos_Camera_GetUp(EntityWrapper EntityID);
	glm::vec3 Eos_Camera_GetRight(EntityWrapper EntityID);
	void	  Eos_Camera_SetActiveCamera(EntityWrapper EntityID);

	*/
	

	//Animation
	AnimationStates Eos_AnimationComponent_GetState(EntityWrapper Entity);
	void    Eos_AnimationComponent_SetState(EntityWrapper Entity, MonoString* state);
	bool    Eos_AnimationComponent_IsAnimationDone(EntityWrapper Entity);
	void    Eos_AnimationComponent_ResetAnimation(EntityWrapper Entity);

	

	void Eos_Audio_SetSFXvol(float vol);
	void Eos_AudioComponent_PlayBGM(EntityWrapper Entity, MonoString* audioName);
	void Eos_AudioComponent_PlaySFX(EntityWrapper Entity, MonoString* audioName);
	void Eos_Audio_IncreaseBGMvol();
	void Eos_Audio_IncreaseSFXvol();
	void Eos_Audio_IncreaseMastervol();
	void Eos_Audio_DecreaseBGMvol();
	void Eos_Audio_DecreaseSFXvol();
	void Eos_Audio_DecreaseMastervol();
	void Eos_Audio_Mute();
	void Eos_Audio_Unmute();

	//SceneManager
	void Eos_SceneManager_LoadNextScene(MonoString* scenefile);
	void Eos_ScriptSystem_StopRunTimeScript();


	//CoreManager
	ENGINESTATE Eos_CoreManager_GetGameState();
	void Eos_CoreManager_SetGameState(ENGINESTATE state);

	//PrefabManager
	EntityWrapper Eos_PrefabManager_Instantiate(MonoString* tileName, glm::vec3* position, glm::vec3* rotation);

	//GUI Component 
	float Eos_GUIComponent_GetOpacity(EntityWrapper Entity);
	void  Eos_GUIComponent_SetOpacity(EntityWrapper Entity, float opaque);
	void  Eos_GUIComponent_GetColor(EntityWrapper Entity, glm::vec3* inColour);
	void  Eos_GUIComponent_SetColor(EntityWrapper Entity,glm::vec3* outColour);

	void  Eos_GUIComponent_SetDepth(EntityWrapper Entity,float d_value);
	float  Eos_GUIComponent_GetDepth(EntityWrapper Entity);
	MonoString*  Eos_GUIComponent_GetFile(EntityWrapper Entity);
	void  Eos_GUIComponent_SetFile(EntityWrapper Entity, MonoString* string);



	//Particle 
	void  Eos_ParticleComponent_SetNumber(EntityWrapper Entity, int d_value);
	int   Eos_ParticleComponent_GetNumber(EntityWrapper Entity);
	void  Eos_ParticleComponent_SetSize(EntityWrapper Entity, float d_value);
	float Eos_ParticleComponent_GetSize(EntityWrapper Entity);
	void  Eos_ParticleComponent_SetLife(EntityWrapper Entity, float d_value);
	float Eos_ParticleComponent_GetLife(EntityWrapper Entity);
	




}
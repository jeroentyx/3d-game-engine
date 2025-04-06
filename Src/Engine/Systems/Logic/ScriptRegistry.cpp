#include "../Src/pch.h"
#include "ScriptRegistry.hpp"
#include "ScriptWrappers.hpp"
#include "../Src/Engine/Components/Transform/TransformComponent.h"
#include "../Src/Engine/Components/Mesh/MeshComponent.h"
#include <mono/metadata/reflection.h>


namespace Eos
{
 
	extern MonoImage* sCoreAssemblyImage;

    //Registering the component needs some reworking for entity



	void ScriptRegistry::RegisterAll()
	{


        //Time
        mono_add_internal_call("EosScriptCore.Time::GetDeltaTime_Native", Scripting::Eos_Time_GetDeltaTime);
        mono_add_internal_call("EosScriptCore.Time::GetTime_Native", Scripting::Eos_Time_GetTime);
        mono_add_internal_call("EosScriptCore.Time::GetFixedDeltaTime_Native", Scripting::Eos_Time_GetFixedDeltaTime);

        //Math Quaternion
        mono_add_internal_call("EosScriptCore.Quaternion::Euler_Native", Scripting::Eos_Quaternion_Euler);

        //Logging
        mono_add_internal_call("EosScriptCore.Log::LogInfo_Native", Scripting::Eos_Console_LogInfo);
        mono_add_internal_call("EosScriptCore.Log::LogWarn_Native", Scripting::Eos_Console_LogWarn);
        mono_add_internal_call("EosScriptCore.Log::LogDebug_Native", Scripting::Eos_Console_LogDebug);
        mono_add_internal_call("EosScriptCore.Log::LogError_Native", Scripting::Eos_Console_LogError);
        mono_add_internal_call("EosScriptCore.Log::LogCritical_Native", Scripting::Eos_Console_LogCritical);


        //Entity
        mono_add_internal_call("EosScriptCore.GameObject::CreateComponent_Native", Scripting::Eos_Entity_CreateComponent);
        mono_add_internal_call("EosScriptCore.GameObject::HasComponent_Native", Scripting::Eos_Entity_HasComponent);
        mono_add_internal_call("EosScriptCore.GameObject::EntityExists_Native", Scripting::Eos_EntityExits);
        mono_add_internal_call("EosScriptCore.GameObject::SetActive_Native", Scripting::Eos_Entity_SetActive);
        mono_add_internal_call("EosScriptCore.GameObject::FindGameObjectByTag_Native", Scripting::Eos_Entity_FindWithTag);
        mono_add_internal_call("EosScriptCore.GameObject::GetAllChild_Native", Scripting::Eos_Entity_GetAllChild);
        mono_add_internal_call("EosScriptCore.GameObject::FindGameObjectsWithTag_Native", Scripting::Eos_Entity_FindGameObjectsWithTag);
        mono_add_internal_call("EosScriptCore.GameObject::GetName_Native", Scripting::Eos_Entity_GetName);
        mono_add_internal_call("EosScriptCore.GameObject::SetName_Native", Scripting::Eos_Entity_SetName);
        mono_add_internal_call("EosScriptCore.MonoComponent::Destroy_Native", Scripting::Eos_Entity_Destroy);
        mono_add_internal_call("EosScriptCore.Transform::GetParent_Native", Scripting::Eos_Entity_GetParent);
        mono_add_internal_call("EosScriptCore.Transform::SetParent_Native", Scripting::Eos_Entity_SetParent);
        mono_add_internal_call("EosScriptCore.Transform::DetachParent_Native", Scripting::Eos_Entity_DetachParent);

        //Physics System
        mono_add_internal_call("EosScriptCore.Physics::SetFixedTimestep_Native", Scripting::Eos_Physics_SetFixedTimeStep);
        mono_add_internal_call("EosScriptCore.Physics::GetFixedTimestep_Native", Scripting::Eos_Physics_GetFixedTimeStep);

        //Mouse Input
        mono_add_internal_call("EosScriptCore.Input::IsKeyPressed_Native", Scripting::Eos_Input_IsKeyPressed);
        mono_add_internal_call("EosScriptCore.Input::IsKeyTriggered_Native", Scripting::Eos_Input_IsKeyTriggered);
        mono_add_internal_call("EosScriptCore.Input::IsMouseButtonPressed_Native", Scripting::Eos_Input_IsMouseButton);
        mono_add_internal_call("EosScriptCore.Input::GetMousePosition_Native", Scripting::Eos_Input_GetMousePosition);
        mono_add_internal_call("EosScriptCore.Input::SetCursorMode_Native", Scripting::Eos_Input_SetCursorMode);
        mono_add_internal_call("EosScriptCore.Input::GetCursorMode_Native", Scripting::Eos_Input_GetCursorMode);
        mono_add_internal_call("EosScriptCore.Input::GetAxis_Native", Scripting::Eos_Input_GetAxis);
        mono_add_internal_call("EosScriptCore.Input::SetMousePosition_Native", Scripting::Eos_Input_SetMousePosition);
        mono_add_internal_call("EosScriptCore.Input::GetMouseWheel_Native", Scripting::Eos_Input_GetMouseWheel);
        mono_add_internal_call("EosScriptCore.Input::IsJoystickConnected_Native", Scripting::Eos_Input_IsJoystickConnected);
        mono_add_internal_call("EosScriptCore.Input::IsControllerButtonPressed_Native", Scripting::Eos_Input_IsControllerButtonPressed);
        mono_add_internal_call("EosScriptCore.Input::CameraControllerMovement_Native", Scripting::Eos_Input_CameraControllerMovement);
        mono_add_internal_call("EosScriptCore.Input::PlayerControllerMovement_Native", Scripting::Eos_Input_PlayerControllerMovement);

        //Static Mesh
        mono_add_internal_call("EosScriptCore.StaticMeshComponent::LoadModel_Native", Scripting::Eos_StaticMeshComponent_Load);


        //Transform
        mono_add_internal_call("EosScriptCore.Transform::GetTransform_Native", Scripting::Eos_TransformComponent_GetTransform);
        mono_add_internal_call("EosScriptCore.Transform::SetTransform_Native", Scripting::Eos_TransformComponent_SetTransform);
        mono_add_internal_call("EosScriptCore.Transform::GetTranslation_Native", Scripting::Eos_TransformComponent_GetTranslation);
        mono_add_internal_call("EosScriptCore.Transform::SetTranslation_Native", Scripting::Eos_TransformComponent_SetTranslation);
        mono_add_internal_call("EosScriptCore.Transform::GetRotation_Native", Scripting::Eos_TransformComponent_GetRotation);
        mono_add_internal_call("EosScriptCore.Transform::SetRotation_Native", Scripting::Eos_TransformComponent_SetRotation);
        mono_add_internal_call("EosScriptCore.Transform::GetScale_Native", Scripting::Eos_TransformComponent_GetScale);
        mono_add_internal_call("EosScriptCore.Transform::SetScale_Native", Scripting::Eos_TransformComponent_SetScale);

        //Rigidbody
        mono_add_internal_call("EosScriptCore.RigidBody::UseGravity_Native", Scripting::Eos_PhysicsComponent_UseGravity);
        mono_add_internal_call("EosScriptCore.RigidBody::AddForce_Native", Scripting::Eos_PhysicsComponent_AddForce);
        mono_add_internal_call("EosScriptCore.RigidBody::MovePosition_Native", Scripting::Eos_PhysicsComponent_MovePosition);
        mono_add_internal_call("EosScriptCore.RigidBody::GetLinearVelocity_Native", Scripting::Eos_PhysicsComponent_GetLinearVelocity);
        mono_add_internal_call("EosScriptCore.RigidBody::SetLinearVelocity_Native", Scripting::Eos_PhysicsComponent_SetLinearVelocity);
        mono_add_internal_call("EosScriptCore.RigidBody::GetAngularVelocity_Native", Scripting::Eos_PhysicsComponent_GetAngularVelocity);
        mono_add_internal_call("EosScriptCore.RigidBody::SetAngularVelocity_Native", Scripting::Eos_PhysicsComponent_SetAngularVelocity);
        mono_add_internal_call("EosScriptCore.RigidBody::GetRotate_Native", Scripting::Eos_PhysicsComponent_GetRotate);
        mono_add_internal_call("EosScriptCore.RigidBody::SetRotate_Native", Scripting::Eos_PhysicsComponent_SetRotate);
        mono_add_internal_call("EosScriptCore.RigidBody::GetMass_Native", Scripting::Eos_PhysicsComponent_GetMass);
        mono_add_internal_call("EosScriptCore.RigidBody::SetMass_Native", Scripting::Eos_PhysicsComponent_SetMass);
        mono_add_internal_call("EosScriptCore.RigidBody::SetGravity_Native", Scripting::Eos_PhysicsComponent_SetGravity);
        mono_add_internal_call("EosScriptCore.RigidBody::GetGravity_Native", Scripting::Eos_PhysicsComponent_GetGravity);
        mono_add_internal_call("EosScriptCore.RigidBody::GetBodyType_Native", Scripting::Eos_PhysicsComponent_GetBodyType);
        mono_add_internal_call("EosScriptCore.RigidBody::RotateAlongAxis_Native", Scripting::Eos_PhysicsComponent_RotateAlongAxis);
        mono_add_internal_call("EosScriptCore.RigidBody::SetStatic_Native", Scripting::Eos_PhysicsComponent_SetStatic);
        mono_add_internal_call("EosScriptCore.RigidBody::SetDynamic_Native", Scripting::Eos_PhysicsComponent_SetDyanmic);
        mono_add_internal_call("EosScriptCore.RigidBody::SetKinematics_Native", Scripting::Eos_PhysicsComponent_SetKinematics);
        

        //Collision
        mono_add_internal_call("EosScriptCore.Collider::GetIsTrigger", Scripting::Eos_PhysicsComponent_GetIsTrigger);
        mono_add_internal_call("EosScriptCore.Collider::SetIsTrigger", Scripting::Eos_PhysicsComponent_SetIsTrigger);


        //Camera
        mono_add_internal_call("EosScriptCore.Camera::GetCameraPosition_Native", Scripting::Eos_Camera_GetCameraPosition);
        mono_add_internal_call("EosScriptCore.Camera::SetCameraPosition_Native", Scripting::Eos_Camera_SetCameraPosition);
        mono_add_internal_call("EosScriptCore.Camera::GetTargetPosition_Native", Scripting::Eos_Camera_GetTargetPosition);
        mono_add_internal_call("EosScriptCore.Camera::SetTargetPosition_Native", Scripting::Eos_Camera_SetTargetPosition);
        mono_add_internal_call("EosScriptCore.Camera::GetTargetRotation_Native", Scripting::Eos_Camera_GetTargetRotation);
        mono_add_internal_call("EosScriptCore.Camera::SetTargetRotation_Native", Scripting::Eos_Camera_SetTargetRotation);
        mono_add_internal_call("EosScriptCore.Camera::GetScreenSizeX_Native", Scripting::Eos_Camera_GetScreenSizeX);
        mono_add_internal_call("EosScriptCore.Camera::GetScreenSizeY_Native", Scripting::Eos_Camera_GetScreenSizeY);
        mono_add_internal_call("EosScriptCore.Camera::SetActive_Native", Scripting::Eos_Camera_SetActive);
        mono_add_internal_call("EosScriptCore.Camera::GetActive_Native", Scripting::Eos_Camera_GetActive);

        //Animation
        mono_add_internal_call("EosScriptCore.Animation::GetAnimateState_Native", Scripting::Eos_AnimationComponent_GetState);
        mono_add_internal_call("EosScriptCore.Animation::SetAnimateState_Native", Scripting::Eos_AnimationComponent_SetState);
        mono_add_internal_call("EosScriptCore.Animation::IsAnimationCompleted_Native", Scripting::Eos_AnimationComponent_IsAnimationDone);
        mono_add_internal_call("EosScriptCore.Animation::ResetCompletedAnimation_Native", Scripting::Eos_AnimationComponent_ResetAnimation);

        

        //AudioComponent
        mono_add_internal_call("EosScriptCore.AudioComponent::PlayBGM_Native", Scripting::Eos_AudioComponent_PlayBGM);
        mono_add_internal_call("EosScriptCore.AudioComponent::PlaySFX_Native", Scripting::Eos_AudioComponent_PlaySFX);
        //Audio System
        mono_add_internal_call("EosScriptCore.Audio::IncreaseBGMvol_Native", Scripting::Eos_Audio_IncreaseBGMvol);
        mono_add_internal_call("EosScriptCore.Audio::DecreaseBGMvol_Native", Scripting::Eos_Audio_DecreaseBGMvol);
        mono_add_internal_call("EosScriptCore.Audio::SetSFXvol_Native", Scripting::Eos_Audio_SetSFXvol);
        mono_add_internal_call("EosScriptCore.Audio::IncreaseSFXvol_Native", Scripting::Eos_Audio_IncreaseSFXvol);
        mono_add_internal_call("EosScriptCore.Audio::DecreaseSFXvol_Native", Scripting::Eos_Audio_DecreaseSFXvol);
        mono_add_internal_call("EosScriptCore.Audio::IncreaseMastervol_Native", Scripting::Eos_Audio_IncreaseMastervol);
        mono_add_internal_call("EosScriptCore.Audio::DecreaseMastervol_Native", Scripting::Eos_Audio_DecreaseMastervol);
        mono_add_internal_call("EosScriptCore.Audio::Mute_Native", Scripting::Eos_Audio_Mute);
        mono_add_internal_call("EosScriptCore.Audio::Unmute_Native", Scripting::Eos_Audio_Unmute);

        //SceneManager
        mono_add_internal_call("EosScriptCore.SceneMananger::LoadNextScene_Native", Scripting::Eos_SceneManager_LoadNextScene);
        mono_add_internal_call("EosScriptCore.SceneMananger::UnloadGameState_Native", Scripting::Eos_ScriptSystem_StopRunTimeScript);


        //CoreManager
        mono_add_internal_call("EosScriptCore.GameStateManager::GetGameState_Native", Scripting::Eos_CoreManager_GetGameState);
        mono_add_internal_call("EosScriptCore.GameStateManager::SetGameState_Native", Scripting::Eos_CoreManager_SetGameState);

        //PrefabManager
        mono_add_internal_call("EosScriptCore.Object::InstantiatePrefab_Native", Scripting::Eos_PrefabManager_Instantiate);

        //UI Component
        mono_add_internal_call("EosScriptCore.UIComponent::GetOpacity_Native", Scripting::Eos_GUIComponent_GetOpacity);
        mono_add_internal_call("EosScriptCore.UIComponent::SetOpacity_Native", Scripting::Eos_GUIComponent_SetOpacity);
        mono_add_internal_call("EosScriptCore.UIComponent::GetColor_Native", Scripting::Eos_GUIComponent_GetColor);
        mono_add_internal_call("EosScriptCore.UIComponent::SetColor_Native", Scripting::Eos_GUIComponent_SetColor);

        //added function- Jeroen 
        // I haven't check whether it is working yet
        mono_add_internal_call("EosScriptCore.UIComponent::SetDepth_Native", Scripting::Eos_GUIComponent_SetDepth);
        mono_add_internal_call("EosScriptCore.UIComponent::GetDepth_Native", Scripting::Eos_GUIComponent_GetDepth);
        mono_add_internal_call("EosScriptCore.UIComponent::SetFilePath_Native", Scripting::Eos_GUIComponent_SetFile);
        mono_add_internal_call("EosScriptCore.UIComponent::GetFilePath_Native", Scripting::Eos_GUIComponent_GetFile);

        //Particles
        mono_add_internal_call("EosScriptCore.ParticleComponent::SetParticleNumber_Native", Scripting::Eos_ParticleComponent_SetNumber);
        mono_add_internal_call("EosScriptCore.ParticleComponent::GetParticleNumber_Native", Scripting::Eos_ParticleComponent_GetNumber);
        mono_add_internal_call("EosScriptCore.ParticleComponent::SetParticleSize_Native", Scripting::Eos_ParticleComponent_SetSize);
        mono_add_internal_call("EosScriptCore.ParticleComponent::GetParticleSize_Native", Scripting::Eos_ParticleComponent_GetSize);
        mono_add_internal_call("EosScriptCore.ParticleComponent::SetParticleLife_Native", Scripting::Eos_ParticleComponent_SetLife);
        mono_add_internal_call("EosScriptCore.ParticleComponent::GetParticleLife_Native", Scripting::Eos_ParticleComponent_GetLife);

	}
}
#include "AnimatedMeshComponent.h"
#include "Src/pch.h"
#include <stb_image/stb_image.h>

#include "Src/Engine/Core/Manager/AssetManager.h"

Animated_Mesh_Component::Animated_Mesh_Component() : 
	currentState{AnimationStates::Idle},
	mesh{nullptr},
	skeleton{nullptr},
	animTrack{nullptr}
{
	//idleAni.Init("Assets/Animations/Surge_Idle_3.fbx");
	//animtionInfo = AnimationCompiler::readOwnFormat("SurgeIdleV7.eosanim");

	//AnimationCompiler::readFBX_to_ownFormat("Assets/Animations/Test/BlueData2CorruptedIdle.fbx");
	//idleInfo = AnimationCompiler::readOwnFormat("BlueDataIdle1D.eosanim");
	//AnimationCompiler::readOwnFormat("Assets/Animations/SurgeIdleV7.eosmesh", 
	//	"Assets/Animations/SurgeIdleV7.eosskeleton", 
	//	"Assets/Animations/SurgeIdleV7.eosanimation");
	/*idleInfo = AnimationCompiler::readOwnFormat("Assets/Animations/BlueData2CorruptedIdle.eosmesh",
		"Assets/Animations/BlueData2CorruptedIdle.eosskeleton",
		"Assets/Animations/BlueData2CorruptedIdle.eosanimation");*/

	//AnimationCompiler::readFBX_to_ownFormat("Assets/Animations/Test/SurgeWalk.fbx");
	//walkInfo = AnimationCompiler::readOwnFormat("Assets/Animations/SurgeWalk.eosanim");
	
	//AnimationCompiler::readFBX_to_ownFormat("Assets/Animations/Test/SurgePortalEnd2.fbx");
	//finishInfo = AnimationCompiler::readOwnFormat("Assets/Animations/SurgePortalEnd2.eosanim");

	//mesh = &Eos::AssetManager::GetInstance().GetMesh("UINode2C");
	//skeleton = &Eos::AssetManager::GetInstance().GetSkeleton("UINode2C");
	//animTrack = &Eos::AssetManager::GetInstance().GetAnimation("UINode2C");

	listOfAnimations[AnimationStates::Idle] = idleInfo;
	listOfAnimations[AnimationStates::Walk] = walkInfo;

	aniClips[AnimationStates::Idle] = { AnimationWrapMode::Loop, idleInfo };
	aniClips[AnimationStates::Walk] = { AnimationWrapMode::Loop, walkInfo };
	aniClips[AnimationStates::Finish] = { AnimationWrapMode::Single, finishInfo };
}

Animated_Mesh_Component::Animated_Mesh_Component(std::string filename) :
	currentState{AnimationStates::Idle}
{
}

Animated_Mesh_Component::~Animated_Mesh_Component()
{
}

// Gets the current animation state
AnimationStates Animated_Mesh_Component::GetState()
{
	return currentState;
}

// Sets the current animation state
void Animated_Mesh_Component::SetState(AnimationStates newState)
{
	//std::cout << "Set state callled" << std::endl;
	//internalTime = 0.0f;
	currentState = newState;
}

bool Animated_Mesh_Component::IsAnimationCompleted() { return isCompleted; }

void Animated_Mesh_Component::ResetCompletedState() { isCompleted = false; }

void Animated_Mesh_Component::Init()
{

}

void Animated_Mesh_Component::RunTimeLoadAnimation(const std::string& filepath)
{
	directory = Eos::FileManager::GetFileNameWithExtension(filepath);

	//animationObj.Init("Assets/Animations/" + directory);

	isLoaded = true;
}

void Animated_Mesh_Component::SetCurrentState(std::string newState)
{
	current_State = newState;
}

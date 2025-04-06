#pragma once

#include <map>
#include <tuple>

#include "Src\Engine\Core\Interfaces\IComponent.h"
#include "Src/Engine/Systems/Graphics/Animation/AnimationWrapModes.h"
#include "Src\Engine\Systems\Graphics\Animation\AnimationHelper.h"		// todo: remove
#include "Src\Engine\Systems\Graphics\Animation\AnimStateMachine.h"
#include "Src/Engine/Systems/Graphics/Compiler/AnimationCompiler.h"

// Will remove with animation state machine
enum class AnimationStates : unsigned char
{
	Idle,
	Walk,
	Finish
};



/*struct A_State
{
	std::string stateName;
	AnimationWrapMode mode;
	std::string animTrackName;
};*/

class Animated_Mesh_Component : public IComponent
{
	std::string directory;

public:
	AnimationStates currentState;
	AnimationStates prevState = AnimationStates::Idle;

public:
	std::map<AnimationStates, AnimationCompiler::Ex_Info> listOfAnimations;
	std::map<AnimationStates, 
		std::pair<AnimationWrapMode, AnimationCompiler::Ex_Info>> aniClips;
	//TestAnimation animationObj;

	bool isAniPaused = false;
	bool isLoaded = true;
	bool isCompleted = false;
	float internalTime = 0.0f;

	Animated_Mesh_Component();
	Animated_Mesh_Component(std::string filename);
	~Animated_Mesh_Component();

	AnimationStates GetState();
	void SetState(AnimationStates newState);

	bool IsAnimationCompleted();
	void ResetCompletedState();

	virtual void Init() override;
	void RunTimeLoadAnimation(const std::string& filepath);

	// New items from animation compiler
	unsigned int vao;
	unsigned int diffuseTexture;
	std::shared_ptr<Eos::Shader> m_shader;

	//AnimationCompiler::Ex_Info animtionInfo;
	AnimationCompiler::Ex_Info idleInfo;
	AnimationCompiler::Ex_Info walkInfo;
	AnimationCompiler::Ex_Info finishInfo;

	glm::mat4 identity{ glm::mat4(1.0f) };

	std::vector<glm::mat4> currentPose;


	// Pointer to data - pointer used so changing/swapping between data is easier
	AnimationCompiler::MeshInfo* mesh;
	AnimationCompiler::SkeletonInfo* skeleton;
	AnimationCompiler::AnimationTrack* animTrack;

	// Could have a list of animation tracks?

	// File directories
	std::string meshDirectory;
	std::string skeletonDirectory;
	std::string animationDirectory;

	std::string current_State;
	AnimationWrapMode currentplayingMode = AnimationWrapMode::Loop;
	std::string previous_state;
	void SetCurrentState(std::string newState);

	// Eg: Walk Loop SurgeWalk
	std::vector<A_State> list_animations;

	A_State m_state;
	//A_State state
	/*list_animations
		- Editor
		- Serialize
		- Deserialize*/

	// Properties
	/*
	animatePhysics - When turned on, animations will be executed in the physics loop.
	clip - The default animation.
	cullingType - Controls culling of this Animation component.
	isPlaying - Is an animation currently being played?
	localBounds - AABB of this Animation animation component in local space.
	playAutomatically - Should the default animation clip (the Animation.clip property) automatically start playing on startup?
	this[string] - Returns the animation state named name.
	wrapMode - 	How should time beyond the playback range of the clip be treated?
	*/

	// Public methods
	/*
	AddClip	 Adds a clip to the animation with name newName.
	Blend	Blends the animation named animation towards targetWeight over the next time seconds.
	CrossFade	Fades the animation with name animation in over a period of time seconds and fades other animations out.
	CrossFadeQueued	Cross fades an animation after previous animations has finished playing.
	GetClipCount	Get the number of clips currently assigned to this animation.
	IsPlaying	Is the animation named name playing?
	Play	Plays an animation without blending.
	PlayQueued	Plays an animation after previous animations has finished playing.
	RemoveClip	Remove clip from the animation list.
	Rewind	Rewinds the animation named name.
	Sample	Samples animations at the current state.
	Stop	Stops all playing animations that were started with this Animation.
	*/

	void Play();
	bool IsPlaying();
	void Stop();

	// Maybe this can be private
	//AnimStateMachine animSM;
	//void ChangeState();
	//void AddState();
	//void RemoveState();

	property_vtable()
};

property_begin(Animated_Mesh_Component)
{
	property_var(meshDirectory),
	property_var(m_state)
}property_vend_h(Animated_Mesh_Component)
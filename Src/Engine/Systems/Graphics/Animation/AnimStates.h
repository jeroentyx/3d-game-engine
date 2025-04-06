#pragma once

#include "IStates.h"
//#include "Src\Engine\Systems\Graphics\Animation\AnimationHelper.h"
#include <string>

class IdleState : public IState<class AnimStateMachine>
{
public:
	std::string stateName = "Idle";
	//TestAnimation ani;

	virtual ~IdleState();

	virtual void Enter(AnimStateMachine* pOwner) override;
	virtual void Execute(AnimStateMachine* pOwner) override;
	//virtual TestAnimation GetAnimation() override;
	virtual void Exit(AnimStateMachine* pOwner) override;
};

class WalkState : public IState<class AnimStateMachine>
{
public:
	std::string stateName = "Walk";
	//TestAnimation ani;

	virtual ~WalkState();

	virtual void Enter(AnimStateMachine* pOwner) override;
	virtual void Execute(AnimStateMachine* pOwner) override;
	//virtual TestAnimation GetAnimation() override;
	virtual void Exit(AnimStateMachine* pOwner) override;
};

class DeathState : public IState<class AnimStateMachine>
{
public:
	std::string stateName = "Death";
	//TestAnimation ani;

	virtual ~DeathState();

	virtual void Enter(AnimStateMachine* pOwner) override;
	virtual void Execute(AnimStateMachine* pOwner) override;
	//virtual TestAnimation GetAnimation() override;
	virtual void Exit(AnimStateMachine* pOwner) override;
};

class NewState : public IState<class AnimStateMachine>
{
public:
	std::string stateName;
	//TestAnimation ani;

	NewState(std::string name);
	virtual ~NewState();

	virtual void Enter(AnimStateMachine* pOwner) override;
	virtual void Execute(AnimStateMachine* pOwner) override;
	//virtual TestAnimation GetAnimation() override;
	virtual void Exit(AnimStateMachine* pOwner) override;
};
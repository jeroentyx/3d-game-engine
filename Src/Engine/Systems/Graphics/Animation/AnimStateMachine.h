#pragma once

#include "StateMachineBase.h"
#include <vector>
#include <unordered_map>

class AnimStateMachine : public StateMachine<AnimStateMachine>
{
	//IState<AnimStateMachine>* m_idleState;
	//IState<AnimStateMachine>* m_walkState;
	//IState<AnimStateMachine>* m_deathState;

	std::vector<IState<AnimStateMachine>*> m_states;
	IState<AnimStateMachine>* m_currentState;

	// Dont really need this
	bool m_done;
	bool moving;

public:
	AnimStateMachine();
	~AnimStateMachine();
	void Update();

	void Done() { m_done = true; }
	bool HasDone() { return m_done; }

	IState<AnimStateMachine>* GetCurrentState();

	void ExtChangeState();
	void ExtAddState();
	void ExtGoToNext();
	void ExtRemoveState();

	friend class IdleState;
	friend class WalkState;
	//friend class DeathState;
};
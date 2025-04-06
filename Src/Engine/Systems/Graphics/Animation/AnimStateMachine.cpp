#include "AnimStateMachine.h"
#include "AnimStates.h"

AnimStateMachine::AnimStateMachine() :
	moving{false}, 
	m_done{false},
	m_currentState{nullptr}
{
	IState<AnimStateMachine>* m_idleState = new IdleState();
	IState<AnimStateMachine>* m_walkState = new WalkState();
	//m_deathState = new DeathState();

	m_states.push_back(m_idleState);
	m_states.push_back(m_walkState);

	StateMachine<AnimStateMachine>::Init(this, m_idleState);
}

AnimStateMachine::~AnimStateMachine()
{
	//delete m_idleState;
	//delete m_walkState;
	//delete m_deathState;

	for (IState<AnimStateMachine>* state : m_states)
		delete state;
}

void AnimStateMachine::Update()
{
	StateMachine<AnimStateMachine>::Update();
}

// Retrives the current state from the state machine
IState<AnimStateMachine>* AnimStateMachine::GetCurrentState()
{
	// Need to return devried class and not the IState class
	return m_currentState;
}

void AnimStateMachine::ExtChangeState()
{
	// But what about the condition to change the state?

}

void AnimStateMachine::ExtAddState()
{
	// modify the container of states
	// add states
}

void AnimStateMachine::ExtGoToNext()
{
	if (!moving)
	{
		StateMachine<AnimStateMachine>::ChangeState(m_states[0]);
		moving = true;
	}
	else
	{
		StateMachine<AnimStateMachine>::ChangeState(m_states[1]);
		moving = false;
	}
}

void AnimStateMachine::ExtRemoveState()
{
	// modify the container of states
	// remove state
}

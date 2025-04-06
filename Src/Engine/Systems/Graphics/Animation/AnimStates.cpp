#include "AnimStates.h"
#include "AnimStateMachine.h"

#include <iostream>

// Idle State definitions
/*****************************************************************************/
IdleState::~IdleState()
{
}

void IdleState::Enter(AnimStateMachine* pOwner)
{
	(void)pOwner;
	//std::cout << "Enter idle state" << std::endl;
}

void IdleState::Execute(AnimStateMachine* pOwner)
{
	(void)pOwner;
	//if (/*move*/)
	//{
	//	pOwner->ChangeState(pOwner->m_walkState);
	//}
	//else if (/*die*/)
	//{
	//	pOwner->ChangeState(pOwner->m_deathState);
	//}

	// increment animation pose
}

//TestAnimation IdleState::GetAnimation()
//{
//	return ani;
//}

void IdleState::Exit(AnimStateMachine* pOwner)
{
	(void)pOwner;
	//std::cout << "Exit from idle state" << std::endl;
}

// Walk State definitions
/*****************************************************************************/
WalkState::~WalkState()
{
}

void WalkState::Enter(AnimStateMachine* pOwner)
{
	(void)pOwner;
	//std::cout << "Enter walk state" << std::endl;
}

void WalkState::Execute(AnimStateMachine* pOwner)
{
	(void)pOwner;
	//if (/*stop moving*/)
	//{
	//	pOwner->ChangeState(pOwner->m_idleState);
	//}
}

//TestAnimation WalkState::GetAnimation()
//{
//	return ani;
//}

void WalkState::Exit(AnimStateMachine* pOwner)
{
	(void)pOwner;
	//std::cout << "Exit from walk state" << std::endl;
}

// Death State definitions
/*****************************************************************************/
DeathState::~DeathState()
{
}

void DeathState::Enter(AnimStateMachine* pOwner)
{
	(void)pOwner;
	//std::cout << "Enter death state" << std::endl;
}

void DeathState::Execute(AnimStateMachine* pOwner)
{
	(void)pOwner;
}

//TestAnimation DeathState::GetAnimation()
//{
//	return ani;
//}

void DeathState::Exit(AnimStateMachine* pOwner)
{
	(void)pOwner;
	//std::cout << "Exit from death state" << std::endl;
}



// For editor creation of new states
/*****************************************************************************/

NewState::NewState(std::string name) :
	stateName{name}
{
}

NewState::~NewState()
{
}

void NewState::Enter(AnimStateMachine* pOwner)
{
	(void)pOwner;
}

void NewState::Execute(AnimStateMachine* pOwner)
{
	(void)pOwner;
}

//TestAnimation NewState::GetAnimation()
//{
//	return ani;
//}

void NewState::Exit(AnimStateMachine* pOwner)
{
	(void)pOwner;
}


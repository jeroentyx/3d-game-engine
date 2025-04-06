#pragma once

#include "IStates.h"

template <class T>
class StateMachine
{
	T* m_pOwner = nullptr;
	IState<T>* m_pCurrentState = nullptr;
	bool m_firstRun = true;

public:
    StateMachine() {};
    ~StateMachine() {};

	void Init(T* pOwner, IState<T>* initialState);
	void Update();
	void ChangeState(IState<T>* newState);
};

template <class T>
inline void StateMachine<T>::Init(T* pOwner, IState<T>* initialState)
{
    m_pCurrentState = initialState;
    m_pOwner = pOwner;
    //std::cout << "State Machine ready\n";
}

template <class T>
inline void StateMachine<T>::Update()
{
    if (m_firstRun)
    {
        m_firstRun = false;
        m_pCurrentState->Enter(m_pOwner);
    }
    m_pCurrentState->Execute(m_pOwner);
}

template <class T>
inline void StateMachine<T>::ChangeState(IState<T>* newState)
{
    m_pCurrentState->Exit(m_pOwner);
    m_pCurrentState = newState;
    m_pCurrentState->Enter(m_pOwner);
}
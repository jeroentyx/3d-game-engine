#pragma once

//class TestAnimation;

template <class T>
class IState
{
public:
    virtual ~IState() {}
    virtual void Enter(T* pOwner) = 0;
    virtual void Execute(T* pOwner) = 0;
    //virtual TestAnimation GetAnimation() = 0;
    virtual void Exit(T* pOwner) = 0;
};
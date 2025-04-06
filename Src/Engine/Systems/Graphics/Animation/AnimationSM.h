#pragma once

#include <vector>

class IState
{
public:
    IState() {}
    virtual ~IState() {}

    // When a state enters and exits  
    virtual void OnEntry() = 0;
    virtual void OnExit() = 0;

    // Called in the main game loop 
    virtual void Update() = 0;
};

class AnimState : public IState
{
public:
    AnimState();
    ~AnimState();
    //Our overrides 
    virtual void OnEntry() = 0;
    virtual void OnExit() = 0;

    virtual void Update() = 0;
};





class IdleState : public AnimState
{
public:
    virtual void OnEntry() override;
    virtual void OnExit() override;
    virtual void Update() override;
};

class WalkState : public AnimState
{
public:
    virtual void OnEntry() override;
    virtual void OnExit() override;
    virtual void Update() override;
};




//class AnimStateList
//{
//public:
//    AnimStateList(IGame* game);
//    ~AnimStateList();
//
//    AnimState* GoToNext();
//    AnimState* GoToPrevious();
//
//    void SetCurrentState(int nextState);
//    void AddState(AnimState* newState);
//
//    void Destroy();
//
//    AnimState* GetCurrent();
//
//protected:
//    IGame* m_game = nullptr;
//    std::vector<AnimState*> m_states;
//    int m_currentStateIndex = -1;
//};
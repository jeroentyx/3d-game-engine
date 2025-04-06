#pragma once

#include "Tools\Singleton.h"
#include <stack>
#include <deque>
#include <memory>

#include "Src\Events\ICommand.h"
#include "Src\Events\ListOfCommands.h"
#include "Src\Inputs\Inputs.h"


class HistoryManager final :
    public Singleton<HistoryManager> {
    //A Simple Undo/Redo Manager
    //thats keep track of the hardcoded Actions in
    //ListOfCommands, Handle the Undo/Redo/Execute here
public:
    typedef std::stack<std::shared_ptr<ICommand>> m_HistoryPool; //A Container to store all Commands
                                                               //with undo/redo features
    enum class ActionType {
        UNDO = 0,
        REDO = 1,
        NONE

    };

    HistoryManager();
    ~HistoryManager();
    void Init();
    void Update();
    void Undo();
    void Redo();
    void ServerCommand(std::shared_ptr<ICommand> cmd);

    //Variable declarations
    m_HistoryPool m_UndoContainer;
    m_HistoryPool m_RedoContainer;
    void ProcessCommand(std::shared_ptr<ICommand> cmd);
};

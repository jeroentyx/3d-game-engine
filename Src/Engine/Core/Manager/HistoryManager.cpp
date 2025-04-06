//Includes

#include "Src\Engine\Core\Manager\HistoryManager.h"



HistoryManager::HistoryManager()
{
    //Empty
}
HistoryManager::~HistoryManager()
{
    //Empty
}
void HistoryManager::Init()
{
    //Empty
}
void HistoryManager::Update()
{
    //TODO When user click a certain keyboard button, it will do either Redo / Undo
    //Need to put the commands at the right place also

    //
    //  If(Inputs::GetKeyPress() // do Undo <- Something like this
    //


   // if (Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::A))
   // {
   //     Undo();
   // }
   // if (Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::D))
   // {
   //     Redo();
   // }
    //if (Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::LeftControl) &&
    //    Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::RightControl))
    //{
    //    if (Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::Z))
    //    {
    //    }
    //    else if (Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::R))
    //    {
    //        Redo();
    //    }
    //}

}
void HistoryManager::Undo()
{
    if (this->m_UndoContainer.size() <= 0) {
        return; // No commands in Undo Container, just exit
    }
    else {
        //Basically just get the newest UserActions
        //Execute undo
        //Push this command to the top of Undo Stack
        //Replace this newest undo, by the next newest undo
        m_UndoContainer.top()->Undo();
        m_RedoContainer.push(m_UndoContainer.top());
        m_UndoContainer.pop();
    }

}
void HistoryManager::Redo()
{
    if (m_RedoContainer.size() <= 0) {
        return;
    }

    //If User want to redo
    //Take the newest Action,
    //And Execute Redo
    m_RedoContainer.top()->Redo();
    m_UndoContainer.push(m_RedoContainer.top());
    m_RedoContainer.pop();

}
void HistoryManager::ServerCommand(std::shared_ptr<ICommand> cmd)
{
    ProcessCommand(cmd);
}
void HistoryManager::ProcessCommand(std::shared_ptr<ICommand> cmd)
{
    cmd->Execute();
    m_UndoContainer.push(cmd);
}


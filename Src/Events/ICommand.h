#pragma once

//
//  Command is all the actions in the editor
//

struct ICommand {
    //Execute, when a action is called, like creation of new entities, deletions
    virtual void Execute() = 0;

    //If creating a character, undoing = character is not created
    virtual void Undo() = 0;

    //If creating a character, redoing = character create a second time
    virtual void Redo() = 0;

    virtual ~ICommand() = default;
};

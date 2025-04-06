#pragma once

#include "ICommand.h"
#include <glm\glm.hpp>
#include <string>
#include <tuple>
#include "Src\Engine\Core\Manager\EntityID.h"

//
//  If an Entity is deleted accidentally, 
//  Create new Entity, then add its Component data
//  back
//
class DeleteEntity : public ICommand {

public:
    EntityID m_entity;
    std::string m_EntityName;

    DeleteEntity() = delete; //delete default constructor
    DeleteEntity(EntityID entt_id, std::string entt_name);

    //TODO:

    //Delete The entity 
    virtual void Execute() override;

    //Create New Entity with all the components of the previous deleted entt
    //Pass in all the datas/Components from previously deleted entt to curr entity
    virtual void Undo() override;

    virtual void Redo() override;

    ~DeleteEntity() = default;
    //Need to put this When i delete the entity
};




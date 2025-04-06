#include "Src/Engine/Core/Manager/EntityManager.h"

//Delete The entity 


//delete default constructor

DeleteEntity::DeleteEntity(EntityID entt_id, std::string entt_name) :
    m_entity(entt_id),
    m_EntityName(entt_name)
{

}

void DeleteEntity::Execute()
{

    EntityManager::GetInstance().DeleteEntity(m_entity);
}

//Create New Entity with all the components of the previous deleted entt
//Pass in all the datas/Components from previously deleted entt to curr entity

void DeleteEntity::Undo()
{
    // m_entity = EntityManager::GetInstance().CreateEntity(m_EntityName);
    // {
    //     //Pass all the data/Components back
    // }
    m_entity = EntityManager::GetInstance().CreateEntity(m_EntityName);

}

inline void DeleteEntity::Redo()
{
    // EntityManager::GetInstance().DeleteEntity(m_entity);
    //delete entt again, and save all datas
    EntityManager::GetInstance().DeleteEntity(m_entity);
}

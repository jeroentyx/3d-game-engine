#include "SceneHierachy.h"
#include "Src\Engine\Core\Manager\HistoryManager.h"

namespace Eos
{
    static bool showWindow = true;

    SceneHierachy::SceneHierachy() :
        sceneHierachypool()
    {
    }
    void SceneHierachy::LoadAllEntitiesInHierachy()
    {
        ImRect windowRect = { ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax() };
        //usedID no longer a vector.
        //std::vector<std::pair<int, std::string>> sceneMap = EntityManager::GetInstance().usedIDs;
        std::vector<std::pair<EntityID, std::string>> sceneMap;
        for (auto itr = EntityManager::GetInstance().usedIDs.begin(); itr != EntityManager::GetInstance().usedIDs.end(); ++itr)
        {
            sceneMap.emplace_back(itr->first, itr->second);
        }

        for (int i = 0; i < sceneMap.size(); i++)
        {

            //Draw to Hierachy
            std::string entityName = "\t";
            entityName += sceneMap[i].second;
            ImGui::Selectable(entityName.c_str());

            if (ImGui::IsItemClicked())
            {
                m_SelectedID = sceneMap[i].first;
                SendSelectionIDEvent sendEvent{ m_SelectedID };
                PE_PUBLISH_EVENT(sendEvent);
            }

        }
    }
    void SceneHierachy::Update()
    {
        ImGui::Begin("SceneHierachy", &active, ImGuiWindowFlags_HorizontalScrollbar);
        
        //LoadAllEntitiesInHierachy();
        makeTree(GET_PARENTCHILD_MAP, NULLENTITY);

        ImGui::PushID(0423);
        if (ImGui::BeginPopupContextWindow("Hierachy"))
        {
            if (ImGui::Selectable("  Create Entity\t\t"))
            {
                EntityID ent = EntityManager::GetInstance().CreateEntity();
                ComponentManager::GetInstance().AddComponent(ent, COMPONENTNAME::Comp_Transform_Component);

                std::shared_ptr<IComponent> gcp = ComponentManager::GetInstance().GetComponent(ent, COMPONENTNAME::Comp_Transform_Component);
                IComponent* ic = gcp.get();
                Transform_Component* tc = static_cast<Transform_Component*>(ic);
                tc->Scale = glm::vec3(1, 1, 1);

                //Set Inspector selected to this
                m_SelectedID = ent;
                



                SendSelectionIDEvent sendEvent{ m_SelectedID };
                PE_PUBLISH_EVENT(sendEvent);
            }

            if (m_SelectedID != NULLENTITY)
            {


                if (ImGui::Selectable("  Delete Entity\t\t"))
                {
                    
                    uint32_t m_DeleteEntity = m_SelectedID._id;
                    UNREFERENCED_PARAMETER(m_DeleteEntity);


                    //
                    // Logic Error TODO: Pls fix
                    //
                    EntityManager::GetInstance().DeleteEntity(m_SelectedID);


                    //Function description
                    //Undo-Redo first you need to create new ICommand c1 of type Shared_ptr, then server it (mean invoke it w/ put to histogram)
                    
                    //TODO Here~
                    //EntityManager::GetInstance().DeleteEntity(m_SelectedID);
                    //EntityID enttCpy = m_SelectedID;
                    //std::string enttName = EntityManager::GetInstance().getEntityName(m_SelectedID);
                    //std::shared_ptr<ICommand> c1(new Eos::DeleteEntity(m_SelectedID, enttName));
                    //HistoryManager::GetInstance().ServerCommand(c1);

                    std::string enttName = EntityManager::GetInstance().getEntityName(m_SelectedID);
                    PE_CORE_TRACE(enttName);
                    std::shared_ptr<ICommand> c1(new DeleteEntity(m_SelectedID, enttName));
                    HistoryManager::GetInstance().ServerCommand(c1);
                    //commented out the debug information
                    //EntityManager::GetInstance().ListAllEntities();

                    //
                    //	Publish the EntityID out
                    //
                    m_SelectedID = NULLENTITY;
                    SendSelectionIDEvent sendEvent{ m_SelectedID };
                    PE_PUBLISH_EVENT(sendEvent);

                }

                if (ImGui::Selectable("  Clone Entity\t\t"))
                {
                    if (!EntityManager::GetInstance().Clone(m_SelectedID))
                        PE_CORE_ERROR("Cannot Clone");
                }
            }


            ImGui::EndPopup();
        }
        ImGui::PopID();
        
        ImGui::End();
    }
    void SceneHierachy::makeTree(const std::map<EntityID, EntityID> &map, EntityID parent)
    {
        for (auto itr = map.begin(); itr != map.end(); ++itr)
        {
            if (itr->second == parent)
            {
                ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
                if(itr->first == m_SelectedID)
                    base_flags |= ImGuiTreeNodeFlags_Selected;
                if (haveChild(map, itr->first) == false)
                {
                    base_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                    ImGui::TreeNodeEx(GET_ENTITY_MANAGER.getEntityName(itr->first).c_str(), base_flags);
                    if (ImGui::IsItemClicked())
                    {
                        m_SelectedID = itr->first;
                        SendSelectionIDEvent sendEvent{ m_SelectedID };
                        PE_PUBLISH_EVENT(sendEvent);
                    }
                    /*
                    if (ImGui::BeginDragDropSource()) 
                    {
                        EntityID ID = m_SelectedID;
                        char temp[sizeof(ID)];
                        memcpy(temp, &ID, sizeof(ID));
                        ImGui::SetDragDropPayload("ENTITYNODE", temp, sizeof(ID), ImGuiCond_Once);
                        ImGui::EndDragDropSource();
                    }
                    if (ImGui::BeginDragDropTarget()) 
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYNODE")) 
                        {
                            EntityID ID;
                            memcpy(&ID, payload->Data, sizeof(ID));
                            GET_ENTITY_MANAGER.detachChildfromParent(ID);
                            GET_ENTITY_MANAGER.attachChildtoParent(itr->first, ID);
                        }
                        ImGui::EndDragDropTarget();
                    }
                    */
                }
                else 
                {
                    bool node_open = ImGui::TreeNodeEx(GET_ENTITY_MANAGER.getEntityName(itr->first).c_str(), base_flags);
                    if (ImGui::IsItemClicked())
                    {
                        m_SelectedID = itr->first;
                        SendSelectionIDEvent sendEvent{ m_SelectedID };
                        PE_PUBLISH_EVENT(sendEvent);
                    }
                    if (node_open)
                    {
                        makeTree(map, itr->first);
                        ImGui::TreePop();
                    }
                    /*
                    if (ImGui::BeginDragDropSource())
                    {
                        EntityID ID = m_SelectedID;
                        char temp[sizeof(ID)];
                        memcpy(temp, &ID, sizeof(ID));
                        ImGui::SetDragDropPayload("ENTITYNODE", temp, sizeof(EntityID), ImGuiCond_Once);
                        ImGui::EndDragDropSource();
                    }
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYNODE"))
                        {
                            EntityID ID;
                            memcpy(&ID, payload->Data, sizeof(ID));
                            GET_ENTITY_MANAGER.detachChildfromParent(ID);
                            GET_ENTITY_MANAGER.attachChildtoParent(itr->first, ID);
                        }
                        ImGui::EndDragDropTarget();
                    }
                    */
                }
            }
            
        }
    }
    bool SceneHierachy::haveChild(const std::map<EntityID, EntityID>& map, EntityID parent)
    {
        for (auto itr = map.begin(); itr != map.end(); ++itr)
        {
            if (itr->second == parent) 
            {
                return true;
            }
        }
        return false;
    }
}
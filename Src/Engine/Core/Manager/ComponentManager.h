#pragma once
#include <map>
#include <string>
#include <array>
#include "ComponentCreator.h"
#include "..\..\ComponentSystemID.h"
#include "..\Src\Engine\Systems\Graphics\Graphics_System.h"
#include "..\Src\Engine\Systems\Physics\Physics_System.h"
#include "..\Src\Engine\Systems\Logic\ScriptingSystem.hpp"
#include "..\..\Core\Interfaces\IComponent.h"
#include "..\Src\Engine\Components\GUIComponent.h"
#include "Tools\Singleton.h"
#include "src\pch.h"

#define GET_COMPONENT_MANAGER ComponentManager::ComponentManager::GetInstance()

//#define GET_SCRIPT_MAP ComponentManager::GetInstance().GetScriptMap()
#define GET_TRANSFORM_MAP ComponentManager::GetInstance().GetTransformMap()
#define GET_ALL_COMPONENTS_MAP ComponentManager::GetInstance().GetAllComponentsMap()

class CoreManager;

class ComponentManager :
	public Singleton<ComponentManager>, public ISerializable
{
	//alias
	using ComponentMapType = std::map<COMPONENTNAME, std::shared_ptr<ComponentCreator>>;
	using UUID = int32_t;

	std::map<COMPONENTNAME, SYSNAME> registeredComponents;
	ComponentMapType ComponentMap;

	size_t totalcomp;

	std::multimap<EntityID, std::shared_ptr<IComponent>> allComponents;
	std::map<EntityID, std::shared_ptr<Transform_Component>> transforms;
	
	std::map<EntityID, std::tuple<glm::vec3, glm::vec3, glm::vec3>> prevTransforms;
	std::map<EntityID, std::tuple<glm::vec3, glm::vec3, glm::vec3>> transformOffset;

public:
	ComponentManager();


	//Add a new component to given Entity
	std::shared_ptr<IComponent> AddComponent(EntityID, COMPONENTNAME);

	//Read a new component to given Entity
	void ReadComponent(EntityID, COMPONENTNAME, JsonReader&);

	//Remove a component from given Entity
	void RemoveComponent(EntityID, COMPONENTNAME);

	//Remove all components from a specific entity
	void RemoveAllComponentsFromEntity(EntityID);

	//Clear all Components and update all systems to clear their component list too
	void ClearAllComponents();

	//Reading from file to initialize all entities, componenets and systems
	bool Load(const std::string& DataToBeRead, JsonReader& _reader, EntityID ID) override;
	bool LoadPreFab(EntityID id, JsonReader& reader);

	//Saving the File into the json string then into the file
	bool Save(const std::string& DataToBeSave, JsonWriter& _writer, EntityID ID) override;
	bool SavePreFab(EntityID id, JsonWriter& reader);

	//Stupid unelegant way to convert enum to string, update it to use ListAllEntitiesOfComponent()
	std::string COMPNAMEtoString(COMPONENTNAME);

	//Get IComponent Pointer to a requested component belonging to an entity.Remember to dynamic cast.
	std::shared_ptr<IComponent> GetComponent(EntityID, COMPONENTNAME);

	//Getting the list of components based on the entity
	std::vector<std::shared_ptr<IComponent>> ListofComponentsInEntity(EntityID);

	//Check if Entity has given Component
	bool EntityHaveComponent(EntityID, COMPONENTNAME);
	void ListAllEntitiesOfComponent(EntityID entityID);
	const std::multimap<EntityID, std::shared_ptr<IComponent>>& GetAllComponentsMap();

	//Transform Component functions
	//Make sure to check if an entity have transform first or not
	bool EntityHaveTransform(EntityID);
	//Get a shared pointer to the transform component of an entity.
	std::shared_ptr<Transform_Component> GetTransformComponent(EntityID);
	std::map<EntityID, std::shared_ptr<Transform_Component>>& GetTransformMap();
	void updatePrevTransform();
	void calculateTransformOffset();
	void parentChildUpdate(); //Used to update all transfrom components of childs


	bool Clone(EntityID newentity, EntityID oldentity);


private:
	//Register a new Component type with their specific creator
	void AddComponentCreator(COMPONENTNAME name, std::shared_ptr<ComponentCreator> creator);
	//Register a new Component and the associated System
	void PairCompAndSys(COMPONENTNAME, SYSNAME);


};
//OLD INT ENTITYID STUFF
/*
class ComponentManager : 
	public Singleton<ComponentManager>, public ISerializable
{
	//alias
	using ComponentMapType = std::map<COMPONENTNAME, std::shared_ptr<ComponentCreator>>;
	using UUID = int32_t;

	std::map<COMPONENTNAME, SYSNAME> registeredComponents;
	ComponentMapType ComponentMap;
	
	size_t totalcomp;

	std::multimap<int, std::shared_ptr<IComponent>> allComponents;
	std::map<int, std::shared_ptr<Transform_Component>> transforms;
	//std::map<int, std::shared_ptr<Mesh_Component>> meshes;

public:
	ComponentManager();


	//Add a new component to given Entity
	std::shared_ptr<IComponent> AddComponent(int, COMPONENTNAME);

	//Read a new component to given Entity
	void ReadComponent(int, COMPONENTNAME, JsonReader&);

	//Remove a component from given Entity
	void RemoveComponent(int, COMPONENTNAME);

	//Remove all components from a specific entity
	void RemoveAllComponentsFromEntity(int);

	//Clear all Components and update all systems to clear their component list too
	void ClearAllComponents();

	//Reading from file to initialize all entities, componenets and systems
	bool Load(const std::string& DataToBeRead, JsonReader& _reader) override;

	//Saving the File into the json string then into the file
	bool Save(const std::string& DataToBeSave, JsonWriter& _writer) override;

	//Stupid unelegant way to convert enum to string, update it to use ListAllEntitiesOfComponent()
	std::string COMPNAMEtoString(COMPONENTNAME);

	//Get IComponent Pointer to a requested component belonging to an entity.Remember to dynamic cast.
	std::shared_ptr<IComponent> GetComponent(int, COMPONENTNAME);

	//Getting the list of components based on the entity
	std::vector<std::shared_ptr<IComponent>> ListofComponentsInEntity(int);

	//Check if Entity has given Component
	bool EntityHaveComponent(int, COMPONENTNAME);
	void ListAllEntitiesOfComponent(int entityID);
	const std::multimap<int, std::shared_ptr<IComponent>>& GetAllComponentsMap();

	//Transform Component functions
	//Make sure to check if an entity have transform first or not
	bool EntityHaveTransform(int);
	//Get a shared pointer to the transform component of an entity.
	std::shared_ptr<Transform_Component> GetTransformComponent(int);
	std::map<int, std::shared_ptr<Transform_Component>>& GetTransformMap();

	
private:
	//Register a new Component type with their specific creator
	void AddComponentCreator(COMPONENTNAME name, std::shared_ptr<ComponentCreator> creator);
	//Register a new Component and the associated System
	void PairCompAndSys(COMPONENTNAME, SYSNAME);
	

};
*/
#include "../../Engine/CoreManager.h"

#pragma once
#include <queue>
#include <array>
#include "ComponentManager.h"
#include "SceneManager.h"
#include <string>
#include <bitset>
#include "Src/pch.h"
//#include "EntityID.h"

#define GET_ENTITY_MANAGER EntityManager::EntityManager::GetInstance()
#define GET_PARENTCHILD_MAP EntityManager::EntityManager::GetInstance().getchildParentMap()

#define MAX_ENTITIES 5000
#define MAX_COMPONENTS 32

//using Signature = std::bitset<MAX_COMPONENTS>

class EntityManager :
	public Singleton<EntityManager>, public ISerializable
{
	std::queue<EntityID> availableIDs;
	std::map<EntityID, EntityID> childParentList;
	std::map<EntityID, bool> activeList;
	int totalEntities{ 0 };
public:
	EntityManager();
	~EntityManager() = default;

	EntityID CreateEntity(std::string entityName = "");

	void DeleteEntity(EntityID ID);

	void ClearAllEntities();

	std::string getEntityName(EntityID id);
	void setEntityName(EntityID id, std::string newName);

	//Functions to add/remove child to parent
	const std::map<EntityID, EntityID> &getchildParentMap() const;
	void attachChildtoParent(EntityID parent, EntityID child);
	void detachChildfromParent(EntityID child);
	void ListAllChildOfParent(EntityID parent);
	EntityID getParentOfChild(EntityID child);
	std::vector<EntityID> findAllChild(EntityID, bool); //recursive function to find all Child, granchild, etc.

	//Functions for serialization
	EntityID CreateEntitySerialize(EntityID id, std::string entityName = "");
	void attachChildtoParentSerialize(EntityID parent, EntityID child);
	void addToAvailableIDs(EntityID);

	//Functions for Active/Inactive
	bool getActive(EntityID);
	void setActive(EntityID, bool);

	//Tester function to list all Entities ID & Names
	void ListAllEntities();

	//std::vector<std::pair<int, std::string>> usedIDs;
	std::map<EntityID, std::string> usedIDs;

	//Reading from file to initialize all entities, componenets and systems
	bool Load(const std::string& DataToBeRead, JsonReader& _reader, EntityID ID) override;

	//Saving the File into the json string then into the file
	bool Save(const std::string& DataToBeSave, JsonWriter& _writer, EntityID ID) override;

	bool Clone(EntityID entity);
	EntityID CloneData(EntityID entity);

};

//OLD INT ENTITYID STUFF
/*
class EntityManager :
	public Singleton<EntityManager>, public ISerializable
{
	std::queue<int> availableIDs;
	int totalEntities{ 0 };
public:
	EntityManager();
	~EntityManager() = default;

	int CreateEntity(std::string entityName = "");

	void DeleteEntity(int ID);

	void ClearAllEntities();
	
	//bool EntityExists(int EntityID);

	std::string getEntityName(int id);
	void setEntityName(int id, std::string newName);

	//Tester function to list all Entities ID & Names
	void ListAllEntities();

	//std::vector<std::pair<int, std::string>> usedIDs;
	std::map<int, std::string> usedIDs;
	
	//Reading from file to initialize all entities, componenets and systems
	bool Load(const std::string& DataToBeRead, JsonReader& _reader) override;

	//Saving the File into the json string then into the file
	bool Save(const std::string& DataToBeSave, JsonWriter& _writer) override;

};
*/
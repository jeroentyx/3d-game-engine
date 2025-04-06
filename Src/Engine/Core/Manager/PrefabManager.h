#pragma once
#include "Src/pch.h"

#define GET_PREFABS_MANAGER 


class PrefabManager : public Singleton<PrefabManager>
{
	JsonWriter m_writer;
	std::string m_currentPrefab;

	std::string PrefabFile = "Assets/Data/Prefabs";

public:
	void Init();
	void Update();
	

	// Saving of prefab through editor
	bool SavePrefab(const std::string& DataToBeSave);
	//Getter for prefab
	//std::string& GetActivePrefab();



	// Function to register prefabs to PrefabMap ( to be used during init of PrefabManager)
	void registerPrefabToMap(std::string prefabName, std::string filepath);

	// Register prefab to prefab entiitymap
	void registerPrefabToPrefabEntityMap(std::string prefabname);

	// Creation of prefab
	//PrefabID CreatePrefab(std::string PrefabName);

	std::map<std::string, std::string> GetPrefabMap();
	

	// Create prefab instance using string prefab  name
	EntityID CreatePrefabInstance(std::string prefabName);

	// Clear the map of unserialised prefabs
	void ClearPrefabMap();

	// Clear the map of seriliased prefabs as entityIDs
	void ClearAllPrefabsinstances();

private:

	std::size_t Count(std::filesystem::path path);
	size_t numberofFiles;
	// map of all the seriliased prefabs from json file
	//std::map<PrefabID, std::string> Prefabs;

	// Map of prefabs with prefab name as key and filepath as value
	std::map<std::string, std::string> PrefabMap;

	// Second map of prefab entities that hold the name as key and a vector of entities with the same key name
	std::map<std::string, std::vector<EntityID>> prefabEntities;

	//Reading from file to initialize all prefab, componenets and systems
	EntityID Instanitate(std::string& PrefabName);


	///////////////////////////////
	//Changing Prefabs Values//
	//Loading into Editor
	bool LoadPrefab(std::string& prefabName);

	//Saving the File into the json string then into the file
	//bool SavePrefab(std::string& PrefabName);
	///////////////////////////////
	/*
	* == Missing Stuff ==
	  Function to read in Json file "RegisterPrefab(Json file, std::string prefabName)" 
	  - This will read the json file, create an entity with components based on the json and store it in a map.

	  EntityID CreatePrefabInstance(std::string prefabName);
	  {
	    EntityID temp = Load(PrefabMap[prefabName]);
		return temp;
	  }

	  - std::map<std::string prefabName, std::string filePath> PrefabMap;
	  PrefabMap["Cube"]

	  - std::map<std::string prefabName, std::vector<EntityID>> prefabEntities;
	   prefabEntities[prefabName].push_back( EntityID CreatePrefabInstance(std::string prefabName);)

	   Then we can use these prefab entities map to access for oru prefabs.

	  **The bool Load() function can just be to load a textfile/json of all the json files you want to Register at the engine init.
	  - So on init, call load to read the textfile line by line and insert each path into the RegisterPrefab function above.**
	*/

};
#pragma once

#include "PrefabManager.h"
#include "ComponentManager.h"

void PrefabManager::ClearAllPrefabsinstances()
{
	prefabEntities.clear();
}

void PrefabManager::ClearPrefabMap()
{ 
	PrefabMap.clear();
}

// To be put inside init or load
//PrefabID PrefabManager::CreatePrefab(std::string PrefabName)
//{
//	// Need a new seriliaser for prefab that returns prefabID
//	// Loads or seriliase the prefab json file from prefabMap
//	// Once seriliases, save into Prefabs map, this perfab has components to edit in editor
//}


std::map<std::string, std::string> PrefabManager::GetPrefabMap()
{
	return PrefabMap;
}

EntityID PrefabManager::CreatePrefabInstance(std::string prefabName)
{
	// --- Covert Prefab to entity ---
	// Create a new entity by calling create entity function, copies the data from prefabID into entitiy struct
	
	//Must check if map contain said prefab first or not, if not using [] will add it into the map
	//Causing unwanted behavior


	if (PrefabMap.contains(prefabName)) 
	{
		// aloysisus load function for serilisation goes in here
		return Instanitate(prefabName);
	}
	else 
	{
		PE_CORE_WARN("Cannot Instaniate PreFabs");
		return NULLENTITY;
	}


}

void PrefabManager::registerPrefabToMap(std::string prefabName, std::string filepath)
{
	PrefabMap.insert(std::pair<std::string, std::string>(prefabName, filepath));
	for (auto const& [key, val] : PrefabMap)
	{
		// For debugging to check if insertion is correctly done
		// Everytime a prefab is register, it will show here in the map before serilisation
		//PE_CORE_INFO("{} : {}", key, val);
	}
}

void PrefabManager::registerPrefabToPrefabEntityMap(std::string prefabName)
{
	prefabEntities[prefabName].push_back(CreatePrefabInstance(prefabName));
	//for (auto const& [key, val] : prefabEntities)
	//{
	//	// For debugging to check if pushback is correctly done
	//	// Everytime a prefab is register, it will show here in the map before serilisation
	//	//PE_CORE_INFO("{} : {}", key, val);
	//}
}

// --- I forgotten where to put these particalar code alr, isit after serilisation of the json file? then put into the prefabeneities map? ---
// prefabEntities[prefabName].push_back(EntityID CreatePrefabInstance(std::string prefabName));

// Load needs to take in a string name which is the prefabName when registered etc "cube prefab" and deserialises it and returns an entitiyID for storage in
// second map.
EntityID PrefabManager::Instanitate(std::string& PrefabName)
{

	JsonReader _reader;
	std::string& filename = PrefabMap.find(PrefabName)->second;


	EosAssert(Eos::FileManager::Exists(filename), "Filename Does Not Exist");
	std::string buffer;
	buffer = Eos::FileManager::ReadFile(filename);

	if (!_reader.CheckForError(buffer.c_str()))
	{
		PE_CORE_WARN("Json Syntax Error");
		return NULLENTITY;
	}

	_reader.StartObject();
	
	std::string name;
	_reader.Member("PrefabName")& name;

	//Also up to you to store the name somewhere so i can save it back in Save Function
	//Create the Prefab and Get the PrefabID
	EntityID id;
	id = EntityManager::GetInstance().CreateEntity(name);

	//Calls ComponentManager to load in the Component
	if (!ComponentManager::GetInstance().LoadPreFab(id, _reader))
		return id;

	_reader.EndObject();

	
	return id;

}

void PrefabManager::Init()
{

	for (auto& directoryEntry : std::filesystem::directory_iterator(PrefabFile))
	{
		const auto& _path = directoryEntry.path();
		auto exfilename = std::filesystem::relative(_path, PrefabFile);
		auto filename = FileManager::GetFileNameWithoutExtension(exfilename.string());
		auto parentpath = FileManager::GetParentPath(_path.string()) + "/" + exfilename.string();

		registerPrefabToMap(filename, _path.string());
	}

	numberofFiles = Count(PrefabFile);


	//// Upon construction, register the avilable prefabs to prefabMap
	//registerPrefabToMap("red-tile", "Assets/Data/red_tile.json");
	//registerPrefabToMap("blue-tile", "Assets/Data/blue_tile.json");
	//registerPrefabToMap("yellow-tile", "Assets/Data/yellow_tile.json");

	//// Current prefabs used now
	//registerPrefabToMap("gate", "Assets/Data/Prefabs/prefab_Gate.json");
	//registerPrefabToMap("data-yellow", "Assets/Data/Prefabs/prefab_Data_Yellow.json");
	//registerPrefabToMap("data-blue", "Assets/Data/Prefabs/prefab_Data_Blue.json");
	//registerPrefabToMap("data-red", "Assets/Data/Prefabs/prefab_Data_Red.json");
	//registerPrefabToMap("goal", "Assets/Data/Prefabs/prefab_Goal.json");
}

void PrefabManager::Update()
{
	//Get the number of files in the directory and check if there is an increase
	//Update the directory only when there is an increase

	if (numberofFiles < Count(PrefabFile))
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(PrefabFile))
		{
			const auto& _path = directoryEntry.path();
			auto exfilename = std::filesystem::relative(_path, PrefabFile);
			auto filename = FileManager::GetFileNameWithoutExtension(exfilename.string());
			auto extension = FileManager::GetExtention(exfilename.string());
			auto parentpath = FileManager::GetParentPath(_path.string()) + "/" + exfilename.string();

			if (!PrefabMap.contains(filename) && extension == ".json")
			{
				registerPrefabToMap(filename, _path.string());
				numberofFiles = Count(PrefabFile);
			}

		}
	}
}

bool PrefabManager::SavePrefab(const std::string& DataToBeSave)
{
	UNREFERENCED_PARAMETER(DataToBeSave);
	JsonWriter _writer;
	if (!EntityManager::GetInstance().usedIDs.empty())
	{
		PE_CORE_INFO("PrefabManager saving prefab...");
		_writer.StartObject();


		for (auto ent : EntityManager::GetInstance().usedIDs)
		{
			_writer.Member("PrefabName")& ent.second;

			ComponentManager::GetInstance().SavePreFab(ent.first, _writer);
		}
		_writer.EndObject();
	}
	Eos::FileManager::WriteFile(DataToBeSave, _writer.GetString());
	return true;
}

//std::string& PrefabManager::GetActivePrefab()
//{
//	return m_currentPrefab;
//}

std::size_t PrefabManager::Count(std::filesystem::path path)
{
	using std::filesystem::directory_iterator;
	return std::distance(directory_iterator(path), directory_iterator{});
}
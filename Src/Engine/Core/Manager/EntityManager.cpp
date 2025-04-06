#include"../Manager/EntityManager.h"
#include"../Manager/EntityManager.h"


EntityManager::EntityManager(): 
	totalEntities(0)
{
	PE_CORE_INFO("Entity Manager Created!");

	for (uint32_t i = 0; i < MAX_ENTITIES; ++i)
	{
		availableIDs.push({ i,0 });
	}
}

EntityID EntityManager::CreateEntity(std::string entityName)
{
	if (totalEntities < MAX_ENTITIES)
	{
		
		EntityID ID = availableIDs.front();
		availableIDs.pop();
		//std::vector<EntityID> empty;
		//If no name given, set a Default Name
		if (entityName.size() == 0) 
		{
			//usedIDs.push_back(std::make_pair(ID, ("Entity " + std::to_string(usedIDs.size()))));
			usedIDs.insert(std::make_pair(ID, ("Entity " + std::to_string(usedIDs.size()))));
			//parentChildList.insert(std::make_pair(ID, empty));
			childParentList.insert(std::make_pair(ID, NULLENTITY));
			activeList.insert(std::make_pair(ID, true));
		}
		else 
		{
			//usedIDs.push_back(std::make_pair(ID, entityName));
			usedIDs.insert(std::make_pair(ID, entityName));
			//parentChildList.insert(std::make_pair(ID, empty));
			childParentList.insert(std::make_pair(ID, NULLENTITY));
			activeList.insert(std::make_pair(ID, true));
		}
		++totalEntities;
		return ID;
	}
	else
	{
		PE_CORE_WARN("Too many entities, unable to create new!");
		//std::cout << "Too many entities, unable to create new!" << std::endl;
		EosAssert(false, "Too many entities, unable to create new!");
		return NULLENTITY;
	}
}

void EntityManager::DeleteEntity(EntityID ID)
{
	/*
	if (parentChildList.contains(ID)) 
	{
		//Delete all child entities first
		for (int i = 0; i < parentChildList[ID].size();) 
		{
			DeleteEntity(parentChildList[ID][i]);
		}
		//Delete itself from the list
		parentChildList.erase(ID);
	}
	else
	{
		PE_CORE_WARN("[Entity Manager] DeleteEntity parentChildList: Entity ID cannot be found!");
	}
	*/
	//Check if is valid ID
	std::vector<EntityID> temp = findAllChild(ID, true);
	if (childParentList.contains(ID)) 
	{
		//std::cout << "Deleting all childs found from childParent List" << std::endl;
		for (int i = 0; i < temp.size(); ++i) 
		{
			//std::cout << temp[i]._id << "." << temp[i]._generations << std::endl;
			childParentList.erase(temp[i]);
		}
		childParentList.erase(ID);
	}

	if (activeList.contains(ID)) 
	{
		activeList.erase(ID);
	}
	
	if (usedIDs.contains(ID))
	{
		//std::cout << "Deleting all childs found from usedID List" << std::endl;
		for (int i = 0; i < temp.size(); ++i)
		{
			//std::cout << temp[i]._id << "." << temp[i]._generations << std::endl;
			usedIDs.erase(temp[i]);
			ComponentManager::GetInstance().RemoveAllComponentsFromEntity(temp[i]);
			++(temp[i]._generations);
			availableIDs.push(temp[i]);
			--totalEntities;
		}
		usedIDs.erase(ID);
		ComponentManager::GetInstance().RemoveAllComponentsFromEntity(ID);
		++(ID._generations);
		availableIDs.push(ID);
		--totalEntities;
	}
	else
	{
		PE_CORE_WARN("[Entity Manager] DeleteEntity usedID: Entity ID cannot be found!");
	}
	//updateParentChildList();
}

void EntityManager::ClearAllEntities()
{
	usedIDs.clear();
	activeList.clear();
	childParentList.clear();
	availableIDs = {};
	for (uint32_t i = 0; i < MAX_ENTITIES; ++i)
	{
		availableIDs.push({i, 0});
	}
	GET_COMPONENT_MANAGER.ClearAllComponents();
}

std::string EntityManager::getEntityName(EntityID id)
{
	std::string blank = "";
	if (usedIDs.contains(id))
	{
		return usedIDs[id];
	}
	else
	{
		std::cout << "[Entity Manager] Requested Entity ID does not exist! Get Entity Failed!" << std::endl;
		return blank;
	}
}

void EntityManager::setEntityName(EntityID id, std::string newName)
{
	if (usedIDs.contains(id))
	{
		usedIDs[id] = newName;
	}
	else
	{
		std::cout << "[Entity Manager] Requested Entity ID does not exist! Rename Failed!" << std::endl;
		return;
	}
	
}

const std::map<EntityID, EntityID>& EntityManager::getchildParentMap() const
{
	return childParentList;
}

void EntityManager::attachChildtoParent(EntityID parent, EntityID child)
{
	/*
		if (std::find(parentChildList[parent].begin(), parentChildList[parent].end(), child) != parentChildList[parent].end()) 
		{
			PE_CORE_WARN("[Entity Manager] Entity already a Child of Parent");
		}
		else 
		{
			parentChildList[parent].emplace_back(child);
			std::cout << "Entity " << child._id << "." << child._generations << " attached to Parent "
				<< parent._id << "." << parent._generations << std::endl;
		}
	*/
	if (childParentList.contains(child) && (childParentList.contains(parent) || parent == NULLENTITY))
	{
		childParentList[child] = parent;
		//std::cout << "Entity " << child._id << "." << child._generations << " attached to Parent "
		//	<< parent._id << "." << parent._generations << std::endl;
	}
	else 
	{
		PE_CORE_WARN("[Entity Manager] Error Attaching Child to Parent");
	}
}

EntityID EntityManager::CreateEntitySerialize(EntityID id, std::string entityName)
{
	usedIDs.insert(std::make_pair(id, entityName));
	return id;
}

void EntityManager::attachChildtoParentSerialize(EntityID parent, EntityID child)
{
	childParentList[child] = parent;
}

void EntityManager::addToAvailableIDs(EntityID id)
{
	availableIDs.push(id);
}

void EntityManager::detachChildfromParent(EntityID child)
{
	/*
	auto temp = std::find(parentChildList[parent].begin(), parentChildList[parent].end(), child);
	if (temp != parentChildList[parent].end())
	{
		parentChildList[parent].erase(temp);
		std::cout << "Entity " << child._id << "." << child._generations << " detached from Parent " 
			<< parent._id << "." << parent._generations << std::endl;
	}
	else
	{
		PE_CORE_WARN("[Entity Manager] Entity is not a child of this parent");
	}
	*/
	if (childParentList.contains(child))
	{
		childParentList[child] = NULLENTITY;
		//std::cout << "Entity " << child._id << "." << child._generations << " detached from Parent" << std::endl;
	}
	else
	{
		PE_CORE_WARN("[Entity Manager] Error detaching Child to Parent, Child does not exist!");
	}
}

void EntityManager::ListAllChildOfParent(EntityID parent)
{
	/*
	if (parentChildList.contains(parent)) 
	{
		std::cout << "All Child of Parent: " << parent._id << "." << parent._generations << " || " << usedIDs[parent] << std::endl;
		for (int i = 0; i < parentChildList[parent].size(); ++i) 
		{
			std::cout << parentChildList[parent][i]._id << "." << parentChildList[parent][i]._generations << " || " << usedIDs[parentChildList[parent][i]] << std::endl;
		}
	}
	*/
	if (childParentList.contains(parent))
	{
		std::cout << "All Child of Parent: " << parent._id << "." << parent._generations << " || " << usedIDs[parent] << std::endl;
		for (auto itr = childParentList.begin(); itr != childParentList.end(); ++itr)
		{
			if (itr->second == parent) 
			{
				std::cout << itr->first._id << "." << itr->first._generations << " || " << usedIDs[itr->first] << std::endl;
			}
		}
	}
	
}

EntityID EntityManager::getParentOfChild(EntityID child)
{
	if (childParentList.contains(child)) 
	{
		return childParentList[child];
	}
	else 
	{
		PE_CORE_WARN("Child Entity does not Exist!");
		return NULLENTITY;
	}
}

std::vector<EntityID> EntityManager::findAllChild(EntityID parent, bool root)
{
	std::vector<EntityID> allChild;
	if (childParentList.contains(parent)) 
	{
		if (!root) 
		{
			allChild.emplace_back(parent);
		}
		for (auto itr = childParentList.begin(); itr != childParentList.end(); ++itr)
		{
			if (itr->second == parent)
			{
				std::vector<EntityID> temp = findAllChild(itr->first, false);
				allChild.insert(allChild.end(), temp.begin(), temp.end());
			}
		}
	}
	return allChild;
}

bool EntityManager::getActive(EntityID id)
{
	if (activeList.contains(id)) 
	{
		return activeList[id];
	}
	else 
	{
		std::cout << "[EntityID] getActive: Entity does not exist!" << std::endl;
		return false;
	}
}

void EntityManager::setActive(EntityID id, bool set) 
{
	if (activeList.contains(id))
	{
		activeList[id] = set;
	}
	else
	{
		std::cout << "[EntityID] setActive: Entity does not exist!" << std::endl;
	}
}


void EntityManager::ListAllEntities()
{
	std::cout << "All Entities in Scene:" << std::endl;
	for (auto itr = usedIDs.begin(); itr != usedIDs.end(); ++itr) 
	{
		std::cout << itr->first._id <<"." << itr->first._generations << " || " << itr->second << std::endl;
	}
}


bool EntityManager::Load(const std::string& DataToBeRead, JsonReader& _reader, EntityID test)
{
	UNREFERENCED_PARAMETER(test);
	//Reset the data
	if (!_reader.CheckForError(DataToBeRead.c_str()))
		PE_CORE_CRITICAL("Syntax Error");
		//std::cerr << "Syntax Error" << std::endl;

	size_t numofentt = 0;

	_reader.StartObject();
	_reader.Member("NumOfEntities");
	_reader.StartArray(&numofentt);

	for (size_t i = 0; i < numofentt; ++i)
	{
		_reader.StartObject();

		std::string Name;
		EntityID parent;
		bool active = true;
		_reader.Member("EntityID")& Name;
		_reader.Member("parentID")& parent._id;
		_reader.Member("parentGen")& parent._generations;
		_reader.Member("Active")& active;
		
		//Ask for the componentmanager to load in the components
		//Convert the ID to the string
		EntityID entID = CreateEntity(Name);
		attachChildtoParentSerialize(parent, entID);
		setActive(entID, active);

		
		if (!ComponentManager::GetInstance().Load("", _reader, entID))
			return false;

		_reader.EndObject();
	}
	_reader.EndArray();
	_reader.EndObject();
	



	return true;
}

bool EntityManager::Save(const std::string& DataToBeSave, JsonWriter& _writer, EntityID test)
{
	UNREFERENCED_PARAMETER(DataToBeSave);
	UNREFERENCED_PARAMETER(test);
	if (!usedIDs.empty())
	{
		_writer.StartObject();
		_writer.Member("NumOfEntities");
		_writer.StartArray();

		bool active;

		for (auto ent : usedIDs)
		{

			active = getActive(ent.first);
			_writer.StartObject();
			_writer.Member("EntityID")& ent.second;
			_writer.Member("parentID")& childParentList[ent.first]._id;
			_writer.Member("parentGen")& childParentList[ent.first]._generations;
			_writer.Member("Active")& active;
			ComponentManager::GetInstance().Save("", _writer, ent.first);
			_writer.EndObject();
		}

		_writer.EndArray();
		_writer.EndObject();

	}
	return true;
}

bool EntityManager::Clone(EntityID entity)
{
	if (!usedIDs.empty())
	{
		EntityID newID;
		//taking the same string of the entity 
		auto name = usedIDs.find(entity)->second;

		//To create a new entityID
		newID = CreateEntity(name);

		if (!ComponentManager::GetInstance().Clone(newID, entity))
			PE_CORE_WARN("Unable to clone Entity's Component");

	}

	return true;
}

EntityID EntityManager::CloneData(EntityID entity)
{
	if (!usedIDs.empty())
	{
		EntityID newID;
		//taking the same string of the entity 
		auto name = usedIDs.find(entity)->second;

		//To create a new entityID
		newID = CreateEntity(name);

		if (!ComponentManager::GetInstance().Clone(newID, entity))
			PE_CORE_WARN("Unable to clone Entity's Component");


		return newID;
	}

	return NULLENTITY;
}



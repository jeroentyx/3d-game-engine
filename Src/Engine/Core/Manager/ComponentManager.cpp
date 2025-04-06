#include "ComponentManager.h"
#include "Src/Engine/Systems/Logic/ScriptingSystem.hpp"
#include "..\Src\Engine\Components\LightComponent.h"
#include "..\Src\Engine\Components\GUIComponent.h"
#include "Src\Engine\Components\SmokeComponent.h"
#include "..\Src\Engine\Components\Camera\CameraComponent.h"
#include "..\Src\Engine\Components\Billboard\BillboardComponent.h"

ComponentManager* FACTORY = NULL;

ComponentManager::ComponentManager()
{
	PE_CORE_INFO("Component Manager Created!");

	if (FACTORY == NULL)
		FACTORY = this;

	//Register & Pair new Components here!
	RegisterComponent(Transform_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Transform_Component, SYSNAME::NO_SYSTEM);

	RegisterComponent(Mesh_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Mesh_Component, SYSNAME::SYS_GRAPHICS);

	RegisterComponent(Static_Mesh_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Static_Mesh_Component, SYSNAME::SYS_GRAPHICS);

	RegisterComponent(Animated_Mesh_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Animated_Mesh_Component, SYSNAME::SYS_GRAPHICS);

	RegisterComponent(Camera_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Camera_Component, SYSNAME::SYS_GRAPHICS);

	RegisterComponent(Physics_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Physics_Component, SYSNAME::SYS_PHYSICS);

	RegisterComponent(Light_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Light_Component, SYSNAME::SYS_GRAPHICS);

	RegisterComponent(Script_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Script_Component, SYSNAME::SYS_LOGIC);

	RegisterComponent(Audio_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Audio_Component, SYSNAME::SYS_AUDIO);

	RegisterComponent(UI_Component);
	PairCompAndSys(COMPONENTNAME::Comp_UI_Component, SYSNAME::SYS_UI);

	RegisterComponent(GUI_Component);
	PairCompAndSys(COMPONENTNAME::Comp_GUI_Component, SYSNAME::SYS_GRAPHICS);

	RegisterComponent(Skybox_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Skybox_Component, SYSNAME::SYS_GRAPHICS);

	RegisterComponent(Font_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Font_Component, SYSNAME::SYS_GRAPHICS);
	
	RegisterComponent(Particle_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Particle_Component, SYSNAME::SYS_GRAPHICS);

	RegisterComponent(Smoke_Component);
	PairCompAndSys(COMPONENTNAME::Comp_Smoke_Component, SYSNAME::SYS_GRAPHICS);

	RegisterComponent(BillBoard_Component);
	PairCompAndSys(COMPONENTNAME::Comp_BillBoard_Component, SYSNAME::SYS_GRAPHICS);
};

void ComponentManager::AddComponentCreator(COMPONENTNAME name,  std::shared_ptr<ComponentCreator> creator)
{
	ComponentMap[name] = std::move(creator);
	//ComponentMap.insert(std::make_pair(name, std::move(creator)));
}


void ComponentManager::PairCompAndSys(COMPONENTNAME compName, SYSNAME sysName = SYSNAME::NO_SYSTEM)
{
	if (!registeredComponents.count(compName))
	{
		registeredComponents.insert({ compName, sysName });
		PE_CORE_INFO("[Component Manager] {} registered!", COMPNAMEtoString(compName));
		//std::cout << "[Component Manager] " << COMPNAMEtoString(compName) << " registered!" << std::endl;
	}
	else
		PE_CORE_WARN("[Component Manager] Component already registered!");
		//std::cout << "[Component Manager] Component already registered!" << std::endl;
}

std::string ComponentManager::COMPNAMEtoString(COMPONENTNAME compName)
{
	switch (compName)
	{
	case COMPONENTNAME::Comp_Physics_Component:
		return std::string("Physics Component");
		break;	
	case COMPONENTNAME::Comp_Transform_Component:
		return std::string("Transform Component");
		break;
	case COMPONENTNAME::Comp_Mesh_Component:
		return std::string("Mesh Component");
		break;
	case COMPONENTNAME::Comp_Static_Mesh_Component:
		return std::string("Static Mesh Component");
		break;
	case COMPONENTNAME::Comp_Animated_Mesh_Component:
		return std::string("Animated Mesh Component");
		break;
	case COMPONENTNAME::Comp_UI_Sprite2D_Component:
		return std::string("UI Sprite Component");
		break;	
	case COMPONENTNAME::Comp_GUI_Component:
		return std::string("GUI Component");
		break;
	case COMPONENTNAME::Comp_Script_Component:
		return std::string("Script Component");
		break;
	case COMPONENTNAME::Comp_Light_Component:
		return std::string("Light Component");
		break;
	case COMPONENTNAME::Comp_Camera_Component:
		return std::string("Camera Component");
		break;
	case COMPONENTNAME::Comp_Audio_Component:
		return std::string("Audio Component");
		break;
	case COMPONENTNAME::Comp_Skybox_Component:
		return std::string("Skybox Component");
		break;
	case COMPONENTNAME::Comp_Font_Component:
		return std::string("Font Component");
		break;
	case COMPONENTNAME::Comp_Particle_Component:
		return std::string("Particle Component");
		break;
	case COMPONENTNAME::Comp_Smoke_Component:
		return std::string("Smoke Component");
		break;
	case COMPONENTNAME::Comp_BillBoard_Component:
		return std::string("Billboard Component");
		break;
	default:
		return std::string("Unknown/Unadded Component");
		break;
	}
}

std::shared_ptr<IComponent> ComponentManager::AddComponent(EntityID entityID, COMPONENTNAME compName)
{
	//Check if requested component is valid
	if (registeredComponents.count(compName))
	{
		//Check if Entity already has the component or not
		if (EntityHaveComponent(entityID, compName) == false)
		{
			//Check if requested component has a associated system
			if (registeredComponents.at(compName) != SYSNAME::NO_SYSTEM)
			{
				//Create the component
				//std::shared_ptr<IComponent> temp = ComponentMap[compName]->Create();
				std::shared_ptr<IComponent> temp = ComponentMap[compName].get()->Create();
				//Call the system manager to update the associated system with new component
				SystemManager::GetInstance().getSystem(registeredComponents[compName])->AddingNewComponent(entityID, compName, temp);
				//Add newly created component to allComponents
				temp->componentType = compName;
				allComponents.insert({ entityID, temp });
				return temp;
			}
			else
			{
				std::shared_ptr<IComponent> temp = ComponentMap[compName]->Create();
				temp->componentType = compName;
				if (compName == COMPONENTNAME::Comp_Transform_Component) 
				{
					std::shared_ptr<Transform_Component> tempT = std::dynamic_pointer_cast<Transform_Component>(temp);
					transforms.insert({ entityID, tempT });
				}
				allComponents.insert({ entityID, temp });
				return temp;
			}
		}
		else
		{
			PE_CORE_WARN("[Component Manager] Entity already has requested Component!");
			//std::cout << "[Component Manager] Entity already has requested Component!" << std::endl;
		
		}
	}
	else
	{
		PE_CORE_WARN("[Component Manager] Requested Component does not exist!");
		//std::cout << "[Component Manager] Requested Component does not exist!" << std::endl;
	}
	return nullptr;

}

void ComponentManager::ReadComponent(EntityID entityID, COMPONENTNAME compName, JsonReader& _reader)
{
	UNREFERENCED_PARAMETER(_reader);
	//Check if requested component is valid
	if (registeredComponents.count(compName))
	{
		//Check if Entity already has the component or not
		if (EntityHaveComponent(entityID, compName) == false)
		{
			//Check if requested component has a associated system
			if (registeredComponents.at(compName) != SYSNAME::NO_SYSTEM)
			{
				//Create the component
				//std::shared_ptr<IComponent> temp = ComponentMap[compName]->Create();
				std::shared_ptr<IComponent> temp = ComponentMap[compName].get()->Create();
				//Call the system manager to update the associated system with new component
				SystemManager::GetInstance().getSystem(registeredComponents[compName])->AddingNewComponent(entityID, compName, temp);
				//Add newly created component to allComponents
				temp->componentType = compName;
				allComponents.insert({ entityID, temp });
			}
			else
			{
				std::shared_ptr<IComponent> temp = ComponentMap[compName]->Create();
				temp->componentType = compName;
				allComponents.insert({ entityID, temp });
			}
		}
		else
		{
			PE_CORE_WARN("[Component Manager] Entity already has requested Component!");
			//std::cout << "[Component Manager] Entity already has requested Component!" << std::endl;
		}
	}
	else
	{
		PE_CORE_WARN("[Component Manager] Requested Component does not exist!");
		//std::cout << "[Component Manager] Requested Component does not exist!" << std::endl;
	}

}

void ComponentManager::RemoveComponent(EntityID entityID, COMPONENTNAME compName)
{
	//Check if requested component is valid
	if (registeredComponents.count(compName))
	{
		for (auto itr = allComponents.begin(); itr != allComponents.end();)
			if (itr->first == entityID)
			{
				if (itr->second->componentType == compName)
				{
					if (registeredComponents.at(compName) != SYSNAME::NO_SYSTEM)
					{
						//Call the system manager to update the associated system to remove the component
						SystemManager::GetInstance().getSystem(registeredComponents[compName])->RemovingComponent(entityID, compName);

						//Remove the object from allComponents
						itr = allComponents.erase(itr);
					}
					else
					{
						if (compName == COMPONENTNAME::Comp_Transform_Component) 
						{
							transforms.erase(entityID);
						}
						itr = allComponents.erase(itr);
					}
				}
				else
				{
					++itr;
				}

			}
			else
			{
				++itr;
			}
	}
	else
	{
		PE_CORE_WARN("[Component Manager] Requested Component does not exist!");
		//std::cout << "[Component Manager] Requested Component does not exist!" << std::endl;
	}
}

void ComponentManager::RemoveAllComponentsFromEntity(EntityID entityID)
{
	for (auto itr = allComponents.begin(); itr != allComponents.end(); )
		if (itr->first == entityID)
		{
			COMPONENTNAME compName = itr->second->componentType;
			if (registeredComponents.at(compName) != SYSNAME::NO_SYSTEM)
			{
				//If component has associated system, ask the system to remove the component first
				SystemManager::GetInstance().getSystem(registeredComponents[compName])->RemovingComponent(entityID, compName);
				itr = allComponents.erase(itr);

			}
			else
			{
				if (compName == COMPONENTNAME::Comp_Transform_Component)
				{
					transforms.erase(entityID);
				}
				itr = allComponents.erase(itr);
			}
		}
		else
		{
			++itr;
		}

}

void ComponentManager::ClearAllComponents()
{
	//Ensure all Systems clear their respective component vectors too
	SystemManager::GetInstance().ClearAllComponentsFromSystems();
	allComponents.clear();
	transforms.clear();
}


bool ComponentManager::Load(const std::string& DataToBeRead, JsonReader& _reader, EntityID ID)
{
	UNREFERENCED_PARAMETER(DataToBeRead);
	size_t num = 0;
	_reader.Member("Components");
	_reader.StartArray(&num);

	SerializeManager serializer;
	for (size_t i = 0; i < num; ++i)
	{
		int componentName;
		_reader.StartObject();
		_reader.Member("ComponentType")& componentName;
		auto temp = AddComponent(ID, static_cast<COMPONENTNAME>(componentName));

		//Testing for init if there is script component



		if (_reader.Good())
		{
			size_t numofprops;
			property::entry PropEntry;
			_reader.Member("Properties");
			_reader.StartArray(&numofprops);
			
			for (size_t j = 0; j < numofprops; ++j)
			{
				_reader.StartObject();

				_reader.Member("name")& PropEntry.first;

				std::string test;
				_reader.Member("type")& test;

				if (0 == std::strcmp(test.data(), "string"))
				{
					_reader.Member(PropEntry.first.c_str())& (PropEntry.second.emplace<std::string>());
				}
				else if (0 == std::strcmp(test.data(), "Uint"))
				{
					_reader.Member(PropEntry.first.c_str())& (PropEntry.second.emplace<int>());
				}
				else if (0 == std::strcmp(test.data(), "f32"))
				{
					_reader.Member(PropEntry.first.c_str())& (PropEntry.second.emplace<float>());
				}
				else if (0 == std::strcmp(test.data(), "bool"))
				{
					_reader.Member(PropEntry.first.c_str())& (PropEntry.second.emplace<bool>());
				}
				else if (0 == std::strcmp(test.data(), "vector3"))
				{
					glm::vec3 vector;
					_reader.Member("x")& vector.x;
					_reader.Member("y")& vector.y;
					_reader.Member("z")& vector.z;
					PropEntry.second.emplace<glm::vec3>()=vector;
				}
				else if (0 == std::strcmp(test.data(), "ScripRef"))
				{

					script_ref script;
					_reader.Member("ScriptName")& script.Modulename;
					PropEntry.second.emplace<script_ref>() = script;
					Eos::ScriptSystem::InitScriptEntity(ID);
					////Serializing the Data from the scripts
					if (ScriptSystem::ModuleExists(script.Modulename))
					{
						size_t fields;
						_reader.Member("StoredFields");
						_reader.StartArray(&fields);
						for (size_t i = 0; i < fields; ++i)
						{
							std::string varname;
							std::string nametype;
							int type;
							FieldType fieldtype = FieldType::NONE;
							_reader.StartObject();
							_reader.Member("name")& varname;
							_reader.Member("typename")& nametype;
							_reader.Member("type")& type;
							
							EntityInstanceData& entity = ScriptSystem::GetEntityInstanceData(ID);
			
							ScriptModuleField& moduleFieldMap = entity.m_modulefieldmap;
							std::unordered_map<std::string, PublicField>& publicFields = moduleFieldMap[script.Modulename];
							
							if (publicFields.find(varname) == publicFields.end())
							{
								PublicField pf = { varname, nametype, fieldtype };
								publicFields.emplace(varname, std::move(pf));
							}

							switch (type)
							{
							case 1:
							{
								bool data;                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
								_reader.Member("data")& data;
								publicFields.at(varname).SetStoredValue(data);
								break;
							}
							case 2:
							{
								int data;
								_reader.Member("data")& data;
								publicFields.at(varname).SetStoredValue(data);
								break;
							}
							case 3:
							{
								float data;
								_reader.Member("data")& data;
								publicFields.at(varname).SetStoredValue(data);
								break;
							}
							case 4:
							{
								unsigned data;
								_reader.Member("data")& data;
								publicFields.at(varname).SetStoredValue(data);
								break;
							}
							case 5:
							{
								std::string string;
								_reader.Member("data")& string;
								publicFields.at(varname).SetStoredValue(string);
								break;
							}
							case 7:
							{
								glm::vec3 vector;
								_reader.Member("x")& vector.x;
								_reader.Member("y")& vector.y;
								_reader.Member("z")& vector.z;
								publicFields.at(varname).SetStoredValue(vector);
								break;
							}
							/*case 7:
								serializer.ReadBool(name.data(), field.SetStoredValue<glm::vec4>(), _reader);
								break;*/
							default: break;
							}

							_reader.EndObject();
						}
						_reader.EndArray();
					}
				}
				else if (0 == std::strcmp(test.data(), "MeshType"))
				{
					_reader.Member(PropEntry.first.c_str())& (PropEntry.second.emplace<int>());
				}
				else if (0 == std::strcmp(test.data(), "ShaderType"))
				{
					_reader.Member(PropEntry.first.c_str())& (PropEntry.second.emplace<int>());
				}
				else if (0 == std::strcmp(test.data(), "A_State"))
				{
					auto animComp = std::dynamic_pointer_cast<Animated_Mesh_Component>(temp);
					size_t size = 0;
					_reader.Member("States");
					_reader.StartArray(&size);
					for (size_t i = 0; i < size; ++i)
					{
						A_State state;
						int mode;
						_reader.StartObject();
						_reader.Member("StateName")& state.stateName;
						_reader.Member("Mode")& mode;
						_reader.Member("animTrackName")& state.animTrackName;
						_reader.EndObject();
						state.mode = static_cast<AnimationWrapMode>(mode);

						animComp->list_animations.push_back(state);
					}
					_reader.EndArray();
				}
				_reader.EndObject();
				property::set(*temp, PropEntry.first.c_str(), PropEntry.second);
				
			}
			_reader.EndArray();
		}
		_reader.EndObject();

		temp->Init();
	}
	_reader.EndArray();
	return true;
}

bool ComponentManager::LoadPreFab(EntityID id, JsonReader& _reader)
{
	size_t num = 0;
	_reader.Member("Components");
	_reader.StartArray(&num);

	SerializeManager serializer;
	for (size_t i = 0; i < num; ++i)
	{
		int componentName;
		_reader.StartObject();
		_reader.Member("ComponentType")& componentName;
		auto temp = AddComponent(id, static_cast<COMPONENTNAME>(componentName));

		if (_reader.Good())
		{
			size_t numofprops;
			property::entry PropEntry;
			_reader.Member("Properties");
			_reader.StartArray(&numofprops);

			for (size_t j = 0; j < numofprops; ++j)
			{
				_reader.StartObject();

				_reader.Member("name")& PropEntry.first;

				std::string test;
				_reader.Member("type")& test;

				if (0 == std::strcmp(test.data(), "string"))
				{
					_reader.Member(PropEntry.first.c_str())& (PropEntry.second.emplace<std::string>());
				}
				else if (0 == std::strcmp(test.data(), "Uint"))
				{
					_reader.Member(PropEntry.first.c_str())& (PropEntry.second.emplace<int>());
				}
				else if (0 == std::strcmp(test.data(), "f32"))
				{
					_reader.Member(PropEntry.first.c_str())& (PropEntry.second.emplace<float>());
				}
				else if (0 == std::strcmp(test.data(), "bool"))
				{
					_reader.Member(PropEntry.first.c_str())& (PropEntry.second.emplace<bool>());
				}
				else if (0 == std::strcmp(test.data(), "vector3"))
				{
					glm::vec3 vector;
					_reader.Member("x")& vector.x;
					_reader.Member("y")& vector.y;
					_reader.Member("z")& vector.z;
					PropEntry.second.emplace<glm::vec3>() = vector;
				}
				else if (0 == std::strcmp(test.data(), "ScripRef"))
				{
					
				}
				else if (0 == std::strcmp(test.data(), "MeshType"))
				{
					_reader.Member(PropEntry.first.c_str())& (PropEntry.second.emplace<int>());
				}
				else if (0 == std::strcmp(test.data(), "ShaderType"))
				{
					_reader.Member(PropEntry.first.c_str())& (PropEntry.second.emplace<int>());
				}
				_reader.EndObject();
				property::set(*temp, PropEntry.first.c_str(), PropEntry.second);

			}
			_reader.EndArray();
		}
		_reader.EndObject();

		temp->Init();
	}
	_reader.EndArray();
	return true;
}

bool ComponentManager::Save(const std::string& DataToBeSave, JsonWriter& _writer, EntityID ID)
{
	UNREFERENCED_PARAMETER(DataToBeSave);
	std::vector<std::shared_ptr<IComponent>> listofcom = ListofComponentsInEntity(ID);

	//_writer.Member("ComponentCount")& static_cast<int>(listofcom.size());

	_writer.Member("Components");
	_writer.StartArray();

	SerializeManager serializer;

	
	for ( auto& E : listofcom )
	{

		_writer.StartObject();
		int ComponentID = static_cast<int>(E->componentType);
		_writer.Member("ComponentType")& ComponentID;

		_writer.Member("Properties");
		_writer.StartArray();


		property::SerializeEnum(*E, [&](std::string_view PropertyName, property::data&& Data, const property::table&, std::size_t, property::flags::type Flags)
		{
			UNREFERENCED_PARAMETER(Flags);
			std::visit([&](auto&& Value)
			{

				using T = std::decay_t<decltype(Value)>;

				if constexpr (std::is_same_v<T, int>)
				{
					serializer.WriteInt(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, float>)
				{
					serializer.WriteFloat(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, uint32_t>)
				{
					serializer.WriteUInt(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, bool>)
				{
					serializer.WriteBool(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, string_t>)
				{
					serializer.WriteString(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, oobb>)
				{
					
				}
				else if constexpr (std::is_same_v<T, glm::vec2>)
				{
					serializer.WriteVector(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, glm::vec3>)
				{
					serializer.WriteVector(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, script_ref>)
				{
					script_ref script = Value;

					auto comp = std::dynamic_pointer_cast<Script_Component>(E);
					std::string scriptname = comp->GetModuleName();
					_writer.StartObject();
					_writer.Member("name")& PropertyName.data();
					_writer.Member("type") & "ScripRef";
					_writer.Member("ScriptName")& scriptname;
					////Serializing the Data from the scripts
					EntityInstanceData& data = ScriptSystem::GetEntityInstanceData(ID);
					const ScriptModuleField& moduleFieldMap = data.m_modulefieldmap;

					if (moduleFieldMap.find(scriptname) != moduleFieldMap.end())
					{
						const auto& fields = moduleFieldMap.at(scriptname);
						_writer.Member("StoredFields");
						_writer.StartArray();
						for (const auto& [name, field] : fields)
						{
							
							int fieldType = static_cast<int>(field.m_Type);
						
							
							switch (field.m_Type)
							{
							case FieldType::BOOL:
							{
								bool storedbool = field.GetStoredValue<bool>();
								_writer.StartObject();
								_writer.Member("name")& name.data();
								_writer.Member("typename")& field.m_Typename.data();
								_writer.Member("type")& fieldType;
								_writer.Member("data")& storedbool;
								_writer.EndObject();
								break;
							}
							case FieldType::INT:
							{
								int storedInt = field.GetStoredValue<int>();
								_writer.StartObject();
								_writer.Member("name")& name.data();
								_writer.Member("typename")& field.m_Typename.data();
								_writer.Member("type")& fieldType;
								_writer.Member("data")& storedInt;
								_writer.EndObject();
								break;
							}
							case FieldType::FLOAT:
							{
								float storedFloat = field.GetStoredValue<float>();
								_writer.StartObject();
								_writer.Member("name")& name.data();
								_writer.Member("typename")& field.m_Typename.data();
								_writer.Member("type")& fieldType;
								_writer.Member("data")& storedFloat;
								_writer.EndObject();
								break;
							}
							case FieldType::UINT:
							{
								unsigned storedunsigned = field.GetStoredValue<unsigned>();
								_writer.StartObject();
								_writer.Member("name")& name.data();
								_writer.Member("typename")& field.m_Typename.data();
								_writer.Member("type")& fieldType;
								_writer.Member("data")& storedunsigned;
								_writer.EndObject();
								break;
							}
							case FieldType::STRING:
							{
								std::string storedstring = field.GetStoredValue<std::string>();
								_writer.StartObject();
								_writer.Member("name")& name.data();
								_writer.Member("typename")& field.m_Typename.data();
								_writer.Member("type")& fieldType;
								_writer.Member("data")& storedstring;
								_writer.EndObject();
								break;
							}
							case FieldType::VEC2:
								//serializer.WriteVector(name.data(), field.GetStoredValue<glm::vec2>(), _writer);
								break;
							case FieldType::VEC3:
							{
								glm::vec3 data = field.GetStoredValue<glm::vec3>();
								_writer.StartObject();
								_writer.Member("name")& name.data();
								_writer.Member("typename")& field.m_Typename.data();
								_writer.Member("type")& fieldType;
								_writer.Member("x")& data.x;
								_writer.Member("y")& data.y;
								_writer.Member("z")& data.z;
								_writer.EndObject();
								break;
							}
							case FieldType::VEC4:
								//serializer.WriteVector(name.data(), field.GetStoredValue<glm::vec4>(), _writer);
								break;
							default: break;
							}
						}
						_writer.EndArray();
					}

					_writer.EndObject();
				}
				else if constexpr (std::is_same_v<T, MeshType>)
				{
					serializer.WriteMeshType(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, ShaderType>)
				{
					serializer.WriteShaderType(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, glm::mat4>)
				{

				}
				else if constexpr (std::is_same_v<T, A_State>)
				{
					auto animComp = std::dynamic_pointer_cast<Animated_Mesh_Component>(E);
					_writer.StartObject();
					_writer.Member("name")& PropertyName.data();
					_writer.Member("type") & "A_State";
					_writer.Member("States");
					_writer.StartArray();
					
					for (auto i : animComp->list_animations)
					{
						int mode = static_cast<int>(i.mode);
						_writer.StartObject();
						_writer.Member("StateName")& i.stateName;
						_writer.Member("Mode")& mode;
						_writer.Member("animTrackName")& i.animTrackName;
						_writer.EndObject();
					}
					_writer.EndArray();
					_writer.EndObject();
				}
				else static_assert(always_false<T>::value, "No Such Property Value!");

			}
			, Data);
		
		});
		_writer.EndArray();

		_writer.EndObject();
	}


	_writer.EndArray();


	return true;
}

bool ComponentManager::SavePreFab(EntityID id, JsonWriter& _writer)
{
	PE_CORE_INFO("ComponentManager saving Prefab...");
	std::vector<std::shared_ptr<IComponent>> listofcom = ListofComponentsInEntity(id);

	_writer.Member("Components");
	_writer.StartArray();

	SerializeManager serializer;


	for (auto& E : listofcom)
	{

		_writer.StartObject();
		int ComponentID = static_cast<int>(E->componentType);
		_writer.Member("ComponentType")& ComponentID;

		_writer.Member("Properties");
		_writer.StartArray();


		property::SerializeEnum(*E, [&](std::string_view PropertyName, property::data&& Data, const property::table&, std::size_t, property::flags::type Flags)
		{
			UNREFERENCED_PARAMETER(Flags);
			std::visit([&](auto&& Value)
			{

				using T = std::decay_t<decltype(Value)>;

				if constexpr (std::is_same_v<T, int>)
				{
					serializer.WriteInt(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, float>)
				{
					serializer.WriteFloat(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, uint32_t>)
				{
					serializer.WriteUInt(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, bool>)
				{
					serializer.WriteBool(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, string_t>)
				{
					serializer.WriteString(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, oobb>)
				{

				}
				else if constexpr (std::is_same_v<T, glm::vec2>)
				{
					serializer.WriteVector(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, glm::vec3>)
				{
					serializer.WriteVector(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, script_ref>)
				{

					serializer.WriteScript(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, MeshType>)
				{
					serializer.WriteMeshType(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, ShaderType>)
				{
					serializer.WriteShaderType(PropertyName.data(), Value, _writer);
				}
				else if constexpr (std::is_same_v<T, A_State>)
				{

				}
				else static_assert(always_false<T>::value, "No Such Property Value!");

			}
			, Data);

		});
		_writer.EndArray();

		_writer.EndObject();
	}


	_writer.EndArray();


	return true;
}


std::shared_ptr<IComponent> ComponentManager::GetComponent(EntityID entityID, COMPONENTNAME compName)
{
	for (auto itr = allComponents.begin(); itr != allComponents.end(); itr++)
	{
		if (itr->first == entityID)
		{
			if (itr->second->componentType == compName)
			{
				return itr->second;
			}
		}
	}
	return nullptr;
}

//This isnt efficient at all, will change the way to get it
std::vector<std::shared_ptr<IComponent>> ComponentManager::ListofComponentsInEntity(EntityID entityID)
{
	std::vector<std::shared_ptr<IComponent>> vector;
	for (auto itr = allComponents.begin(); itr != allComponents.end(); itr++)
	{
		if (itr->first == entityID)
		{
			vector.push_back(itr->second);
		}
	}

	return vector;
}

bool ComponentManager::EntityHaveComponent(EntityID entityID, COMPONENTNAME compName)
{
	for (auto itr = allComponents.begin(); itr != allComponents.end(); itr++)
		if (itr->first == entityID)
		{
			if (itr->second->componentType == compName)
			{
				return true;
			}
		}
	return false;
}

void ComponentManager::ListAllEntitiesOfComponent(EntityID entityID)
{
	PE_CORE_INFO("Components in EntityID {} :", entityID._id);
	//std::cout << "Components in EntityID " << entityID << " :" << std::endl;
	for (auto itr = allComponents.begin(); itr != allComponents.end(); itr++)
		if (itr->first == entityID)
			PE_CORE_INFO(COMPNAMEtoString(itr->second->componentType));
			//std::cout << COMPNAMEtoString(itr->second->componentType) << std::endl;
}

const std::multimap<EntityID, std::shared_ptr<IComponent>>& ComponentManager::GetAllComponentsMap()
{
	return allComponents;
}

bool ComponentManager::EntityHaveTransform(EntityID entityID)
{
	return transforms.contains(entityID);
}

std::shared_ptr<Transform_Component> ComponentManager::GetTransformComponent(EntityID entityID)
{
	if (EntityHaveTransform(entityID))
		return transforms[entityID];
	else
		return nullptr;
}

std::map<EntityID, std::shared_ptr<Transform_Component>>& ComponentManager::GetTransformMap()
{
	return transforms;
}

void ComponentManager::updatePrevTransform()
{
	for (auto itr = transforms.begin(); itr != transforms.end(); ++itr) 
	{
		glm::vec3 pos = itr->second->Translation;
		glm::vec3 rot = itr->second->Rotation;
		glm::vec3 scale = itr->second->Scale;
		prevTransforms.insert(std::make_pair(itr->first, std::make_tuple(pos, rot, scale)));
	}
}

void ComponentManager::calculateTransformOffset()
{
	for (auto itr = prevTransforms.begin(); itr != prevTransforms.end(); ++itr) 
	{
		if (transforms.contains(itr->first)) 
		{
			glm::vec3 currPos = transforms[itr->first]->Translation;
			glm::vec3 currRot = transforms[itr->first]->Rotation;
			glm::vec3 currScale = transforms[itr->first]->Scale;
			glm::vec3 prevPos = std::get<0>(itr->second);
			glm::vec3 prevRot = std::get<1>(itr->second);
			glm::vec3 prevScale = std::get<2>(itr->second);
			transformOffset.insert(std::make_pair(itr->first, std::make_tuple((currPos-prevPos), (currRot-prevRot), (currScale-prevScale))));
		}
	}
	prevTransforms.clear();
}

void ComponentManager::parentChildUpdate()
{
	for (auto itr = transformOffset.begin(); itr != transformOffset.end(); ++itr) 
	{
		std::vector<EntityID> temp = GET_ENTITY_MANAGER.findAllChild(itr->first, true);
		for (int i = 0; i < temp.size(); ++i) 
		{
			if (transforms.contains(temp[i]))
			{
				transforms[temp[i]]->Translation += std::get<0>(itr->second);
				transforms[temp[i]]->Rotation += std::get<1>(itr->second);
				transforms[temp[i]]->Scale += std::get<2>(itr->second);
			}
		}
	}
	transformOffset.clear();
}
bool ComponentManager::Clone(EntityID newentity, EntityID oldentity)
{

	std::vector<std::shared_ptr<IComponent>> listofcom = ListofComponentsInEntity(oldentity);

	

	for (auto& C : listofcom)
	{

		property::entry PropEntry;


		int ComponentID = static_cast<int>(C->componentType);
		auto temp = AddComponent(newentity, static_cast<COMPONENTNAME>(ComponentID));

		property::SerializeEnum(*C, [&](std::string_view PropertyName, property::data&& Data, const property::table&, std::size_t, property::flags::type Flags)
		{
			UNREFERENCED_PARAMETER(Flags);
			std::visit([&](auto&& Value)
			{

				using T = std::decay_t<decltype(Value)>;

				if constexpr (std::is_same_v<T, int>)
				{
					PropEntry.first = PropertyName;
					PropEntry.second = Value;
				}
				else if constexpr (std::is_same_v<T, float>)
				{
					PropEntry.first = PropertyName;
					PropEntry.second = Value;
				}
				else if constexpr (std::is_same_v<T, uint32_t>)
				{
					PropEntry.first = PropertyName;
					PropEntry.second = Value;
				}
				else if constexpr (std::is_same_v<T, bool>)
				{
					PropEntry.first = PropertyName;
					PropEntry.second = Value;
				}
				else if constexpr (std::is_same_v<T, string_t>)
				{
					PropEntry.first = PropertyName;
					PropEntry.second = Value;
				}
				else if constexpr (std::is_same_v<T, oobb>)
				{
					PropEntry.first = PropertyName;
					PropEntry.second = Value;
				}
				else if constexpr (std::is_same_v <T, glm::vec2>)
				{
					PropEntry.first = PropertyName;
					PropEntry.second = Value;
				}
				else if constexpr (std::is_same_v<T, glm::vec3>)
				{
					PropEntry.first = PropertyName;
					PropEntry.second = Value;
				}
				else if constexpr (std::is_same_v<T, script_ref>)
				{
					PropEntry.first = PropertyName;
					PropEntry.second = Value;
				}
				else if constexpr (std::is_same_v<T, MeshType>)
				{
					PropEntry.first = PropertyName;
					PropEntry.second = Value;
				}
				else if constexpr (std::is_same_v<T, ShaderType>)
				{
					PropEntry.first = PropertyName;
					PropEntry.second = Value;
				}
				else if constexpr (std::is_same_v<T, glm::mat4>)
				{
					PropEntry.first = PropertyName;
					PropEntry.second = Value;
				}
				else if constexpr (std::is_same_v<T, A_State>)
				{

				}
				else static_assert(always_false<T>::value, "No Such Property Value!");

				property::set(*temp, PropEntry.first.c_str(), PropEntry.second);
			}
			, Data);

			temp->Init();

		});

		
	}

	return true;
}



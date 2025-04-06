#include <Src/pch.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <imgui.h> //For the interface to see the scripts *DEBUGGING PURPOSE*
#include "ScriptingSystem.hpp"
#include "ScriptRegistry.hpp"
#include "../Src/Engine/Core/Manager/EntityManager.h"
#include "../Src/Engine/Core/Manager/ComponentManager.h"
#include <mono/metadata/reflection.h>


namespace Eos
{
	// Images holds the the data inside the dlls
	MonoImage* sAppAssemblyImage = nullptr;
	MonoImage* sCoreAssemblyImage = nullptr;

	std::map<EntityID, std::shared_ptr<Script_Component>> ScriptSystem::scripts;
	//For the engine interface so to compile C# script 
	//Mainly to enable hot reloading

	struct ComponentC
	{
		MonoClass* EntityClass;
	};

	struct ScriptEngineData
	{
		MonoDomain* Domain = nullptr;
		MonoAssembly* CoreAssembly = nullptr;
		//This is for the scripting part
		MonoAssembly* AppAssembly = nullptr;
		EntityInstanceMap EntityInstanceMap;
		//The string in here refers to the class name in c#
		std::unordered_map<std::string, CSClass> EntityClassMap;
	};

	static ScriptEngineData sData;
	static ComponentC  sComponent;


	struct CSClass
	{
		std::string FullName;
		std::string ClassName;
		std::string NamespaceName;
		
		MonoClass* Class = nullptr;
		MonoClass* CompClass = nullptr;
		MonoMethod* Constructor = nullptr;
		MonoMethod* InitMethod = nullptr;
		MonoMethod* OnStartMethod = nullptr;
		MonoMethod* OnUpdateMethod = nullptr;
		MonoMethod* OnFixedUpdateMethod = nullptr;

		MonoMethod* OnCollisionBeginMethod = nullptr;
		MonoMethod* OnCollisionEndMethod = nullptr;
		MonoMethod* OnTriggerBeginMethod = nullptr;
		MonoMethod* OnTriggerStayMethod = nullptr;
		MonoMethod* OnTriggerEndMethod = nullptr;

		MonoClass* EntityClass = nullptr;


		//The image that is passed in is sAppAssemblyImage
		void InitClassMethods(MonoImage* image)
		{


			CompClass = mono_class_from_name(image, "EosScriptCore", "MonoComponent");
			std::string Namespace = "EosScriptCore.Scripts.";
			//The right side is the scripting side c# function
			InitMethod = mono_class_get_method_from_name(CompClass, "Init", -1);
			OnStartMethod = Scripting::GetMethod(image, Namespace + ClassName + ":OnStart()");
			OnUpdateMethod = Scripting::GetMethod(image, Namespace + ClassName + ":OnUpdate()");
			OnFixedUpdateMethod = Scripting::GetMethod(image, Namespace + ClassName + ":OnFixedUpdate()");


			OnCollisionBeginMethod = Scripting::GetMethod(image, Namespace + ClassName + ":OnCollisionBegin(ulong)");
			OnCollisionEndMethod = Scripting::GetMethod(image, Namespace + ClassName + ":OnCollisionEnd(ulong)");
			OnTriggerBeginMethod = Scripting::GetMethod(image, Namespace + ClassName + ":OnTriggerBegin(ulong)");
			OnTriggerStayMethod = Scripting::GetMethod(image, Namespace + ClassName + ":OnTriggerStay(ulong)");
			OnTriggerEndMethod = Scripting::GetMethod(image, Namespace + ClassName + ":OnTriggerEnd(ulong)");
		}

	};


	MonoClass* ScriptSystem::GetEntityClass()
	{
		return sComponent.EntityClass;
	}

	void ScriptSystem::InitComponentsClass()
	{
		sComponent.EntityClass = mono_class_from_name(sCoreAssemblyImage, "EosScriptCore", "Entity");
		EosAssert(sComponent.EntityClass, "Failed to generate Gameobject Class");
	}


	void ScriptSystem::Init()
	{
#ifndef _DEBUG
		const char* corePath = "EosScriptCore\\EosScriptCore\\bin\\Release\\netcoreapp3.1\\EosScriptCore.dll";
#else
		const char* corePath = "EosScriptCore\\EosScriptCore\\bin\\Debug\\netcoreapp3.1\\EosScriptCore.dll";
#endif

		mono_set_dirs("EosScriptCore\\Mono\\lib", "EosScriptCore\\Mono\\etc\\mono");
		mono_jit_init("Eos");

		//Creating the main domain
		sData.Domain = Scripting::CreateDomain((char*)"Eos Runtime");
		//Exception


		if (!sData.Domain)
		{
			PE_CORE_ERROR("Core Domain is not found");
		}

		LoadAssemblies(corePath);

	}
	void ScriptSystem::Update()
	{

#ifndef _DEBUG
		//ZoneScopedN("Scripting System")
#endif	

			

		if (CoreManager::GetInstance().getEngineState() == ENGINESTATE::PLAY)
		{
			OnRuntimeUpdate(static_cast<float>(Eos::FrameRateManager::GetInstance().delta()));
			OnRuntimeFixedUpdate(static_cast<float>(Eos::FixedTimer::GetFixedDeltaTime()));
		}

		if (CoreManager::GetInstance().getEngineState() == ENGINESTATE::STOP)
		{
			OnRuntimeStop();
		}




#ifndef _DEBUG
		//FrameMark
#endif

	}
	void ScriptSystem::Exit()
	{

		for (auto& ent : sData.EntityInstanceMap)
		{
			uint32_t handle = ent.second.m_instance.Handle;
			mono_gchandle_free(handle);
		}

		mono_jit_cleanup(sData.Domain);

		sData.EntityInstanceMap.clear();
	}

	void ScriptSystem::ReloadAssembly(const std::string& path)
	{
		OnRuntimeStop(); //to stop updating another scripts;

		LoadAssemblies(path);

		//This if statement doesnt apply well here as compared to Electro
		//Reason being is that there will also be a sceneID in Electro where i dont have SceneID
		// Therefore, the map will have sth due to the sceneID
		// My map is literally empty at start
		if (sData.EntityInstanceMap.empty())
		{
			for (auto& entity : scripts)
			{
				EosAssert(scripts.find(entity.first) != scripts.end(), "Invalid entity ID or entity doesn't exist in scene!");
				InitScriptEntity(entity.first);
			}
		}


	}

	void ScriptSystem::OnScriptComponentDestroyed(EntityID EntityID)
	{
		EosAssert(sData.EntityInstanceMap.find(EntityID._id) != sData.EntityInstanceMap.end(), "Entity Doesnt Exist");
		sData.EntityInstanceMap.erase(EntityID._id);
	}

	EntityInstanceMap& ScriptSystem::GetInstanceMap()
	{
		return sData.EntityInstanceMap;
	}

	void ScriptSystem::LoadAssemblies(const std::string& assemblyPath)
	{
		MonoDomain* domain = nullptr;
		bool cleanup = false;


		//The app path wont be the current path (the path will recompile again and send through the api)
		//const std::string AppPath = "EosScriptCore\\EosScriptCore\\bin\\Debug\\netcoreapp3.1\\";

		if (sData.Domain)
		{
			domain = Scripting::CreateAppDomain((char*)"Eos Runtime");
			Scripting::SetDomain(domain);
			cleanup = true;
		}

		const std::string corePath = FileManager::GetParentPath(assemblyPath) + "\\EosScriptCore.dll";


		sData.CoreAssembly = Scripting::LoadAssembly(corePath.c_str());
		sCoreAssemblyImage = Scripting::GetAssemblyImage(sData.CoreAssembly);

		if (!sCoreAssemblyImage)
		{
			EosAssert(sCoreAssemblyImage, "Core Assembly is not found");
		}


		sData.AppAssembly = Scripting::LoadAssembly(assemblyPath.c_str());
		sAppAssemblyImage = Scripting::GetAssemblyImage(sData.AppAssembly);

		if (!sAppAssemblyImage)
		{
			EosAssert(sAppAssemblyImage, "App Assembly is not found");
		}


		InitComponentsClass();
		ScriptRegistry::RegisterAll();


		if (cleanup)
		{
			mono_domain_unload(sData.Domain);
			sData.Domain = domain;
		}

	}

	bool ScriptSystem::IsEntityExist(EntityID entt)
	{
		if (sData.EntityInstanceMap.find(entt._id) != sData.EntityInstanceMap.end())
			return false;
		else
			return true;
	}

	MonoDomain* ScriptSystem::GetDomain()
	{
		return sData.Domain;
	}

	bool ScriptSystem::IsEntityModuleValid(EntityID EntityID)
	{
		return scripts.contains(EntityID) && ModuleExists(scripts.find(EntityID)->second->GetModuleName());
	}

	bool ScriptSystem::ModuleExists(const std::string& moduleName)
	{

		std::string classname;

		if (moduleName.find('.') != std::string::npos)
		{
			classname = moduleName.substr(0, moduleName.find_last_of('.'));
		}
		else
			classname = moduleName;

		MonoClass* monoClass = mono_class_from_name(sAppAssemblyImage, "EosScriptCore.Scripts", classname.c_str());

		return monoClass != nullptr;

	}

	static FieldType GetFieldType(MonoType* monoType)
	{
		int type = mono_type_get_type(monoType);
		switch (type)
		{
		case MONO_TYPE_BOOLEAN:  return FieldType::BOOL;
		case MONO_TYPE_R4: return FieldType::FLOAT;
		case MONO_TYPE_I4: return FieldType::INT;
		case MONO_TYPE_U4: return FieldType::UINT;
		case MONO_TYPE_STRING: return FieldType::STRING;
		case MONO_TYPE_VALUETYPE:
		{
			char* name = mono_type_get_name(monoType);
			if (strcmp(name, "EosScriptCore.Vector2") == 0) return FieldType::VEC2;
			if (strcmp(name, "EosScriptCore.Vector3") == 0) return FieldType::VEC3;
			if (strcmp(name, "EosScriptCore.Vector4") == 0) return FieldType::VEC4;
		}
		}
		return FieldType::NONE;
	}


	//When the scriptcomponent is added to an Entity
	void ScriptSystem::InitScriptEntity(EntityID EntityID)
	{
		const std::string& moduleName = scripts.find(EntityID)->second->GetModuleName();

		if (moduleName == "Null")
			return;
		if (!ModuleExists(moduleName))
			return;

		CSClass& scriptClass = sData.EntityClassMap[moduleName];
		scriptClass.FullName = moduleName;


		if (moduleName.find('.') != std::string::npos) {

			scriptClass.ClassName = moduleName.substr(0, moduleName.find_last_of('.'));
		}
		else
			scriptClass.ClassName = moduleName;

		scriptClass.Class = GetClass(sAppAssemblyImage, scriptClass);
		scriptClass.InitClassMethods(sAppAssemblyImage);

		EntityInstanceData& entityintData = sData.EntityInstanceMap[EntityID._id];
		EntityInstance& entityInt = entityintData.m_instance;
		entityInt.ScriptClass = &scriptClass;

		ScriptModuleField& modulefieldmap = entityintData.m_modulefieldmap;
		auto& fieldmap = modulefieldmap[moduleName];


		//Save the old fields
		std::unordered_map<std::string, PublicField> oldfields;
		oldfields.reserve(fieldmap.size());
		for (auto& [fieldName, field] : fieldmap)
			oldfields.emplace(fieldName, std::move(field));

		fieldmap.clear();

		{
			MonoClassField* iter;
			void* ptr = nullptr;
			//This while loop will retrieve all fields in the c# scripts
			while ((iter = mono_class_get_fields(scriptClass.Class, &ptr)) != nullptr)
			{
				const char* name = mono_field_get_name(iter);
				uint32_t flags = mono_field_get_flags(iter);

				//private variables can leave it be inside the script
				if ((flags & MONO_FIELD_ATTR_PUBLIC) == 0)
					continue;

				MonoType* fieldType = mono_field_get_type(iter);
				//Converting C# variables type into C++ types
				FieldType fieldtype = GetFieldType(fieldType);

				//MonoCustomAttrInfo* attr = mono_custom_attrs_from_field(scriptClass.Class, iter);

				//UNREFERENCED_PARAMETER(attr);

				char* typeName = mono_type_get_name(fieldType);

				if (oldfields.find(name) != oldfields.end())
					fieldmap.emplace(name, std::move(oldfields.at(name)));
				else
				{
					PublicField field = { name, typeName, fieldtype };
					field.m_EntityInstance = &entityInt;
					field.m_MonoClassField = iter;
					fieldmap.emplace(name, std::move(field));
				}

			}
		}


	}
	//If ScriptComponent is removed
	void ScriptSystem::ShutDownScriptEntity(EntityID EntityID, const std::string& modulename)
	{
		EntityInstanceData& EID = GetEntityInstanceData(EntityID);
		ScriptModuleField& modulefieldMap = EID.m_modulefieldmap;
		if (modulefieldMap.find(modulename) != modulefieldMap.end())
			modulefieldMap.erase(modulename);
	}

	void ScriptSystem::OnStart(EntityID entityID)
	{
		EntityInstance& entityinstance = GetEntityInstanceData(entityID).m_instance;

		if (entityinstance.ScriptClass->OnStartMethod)
			Scripting::CallMethod(entityinstance.GetInstance(), entityinstance.ScriptClass->OnStartMethod);

	}

	void ScriptSystem::OnUpdate(EntityID entityID, float engineDelta)
	{
		EntityInstance& entityinstance = GetEntityInstanceData(entityID).m_instance;
		if (GET_ENTITY_MANAGER.getActive(entityID))
		{
			if (entityinstance.ScriptClass->OnUpdateMethod)
			{
				void* args[] = { &engineDelta };
				Scripting::CallMethod(entityinstance.GetInstance(), entityinstance.ScriptClass->OnUpdateMethod, args);
			}
		}

	}

	//Test//

	void ScriptSystem::OnFixedUpdate(EntityID entityID, float fixedDeltaTime)
	{
		EntityInstance& entityinstance = GetEntityInstanceData(entityID).m_instance;
		if (GET_ENTITY_MANAGER.getActive(entityID))
		{
			if (entityinstance.ScriptClass->OnFixedUpdateMethod)
			{
				void* args[] = { &fixedDeltaTime };
				Scripting::CallMethod(entityinstance.GetInstance(), entityinstance.ScriptClass->OnFixedUpdateMethod, args);
			}
		}
	}

	void ScriptSystem::OnColiisonBegin(EntityID entityID, EntityID otherID)
	{
		EntityInstance& entityInstance = GetEntityInstanceData(entityID).m_instance;
		if (entityInstance.ScriptClass->OnCollisionBeginMethod)
		{
			uint64_t value = otherID._id;
			void* args[] = { &value };
			Scripting::CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->OnCollisionBeginMethod, args);
		}

	}

	void ScriptSystem::OnCollisionEnd(EntityID entityID, EntityID otherID)
	{
		EntityInstance& entityInstance = GetEntityInstanceData(entityID).m_instance;
		if (entityInstance.ScriptClass->OnCollisionEndMethod)
		{
			uint64_t value = otherID._id;
			void* args[] = { &value };
			Scripting::CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->OnCollisionEndMethod, args);
		}
	}

	void ScriptSystem::OnTriggerBegin(uint32_t id, uint32_t id2)
	{
		UNREFERENCED_PARAMETER(id);
		EntityID entity{ id2, 0 };
		EntityInstance& entityInstance = GetEntityInstanceData(entity).m_instance;
		if (GET_ENTITY_MANAGER.getActive(entity))
		{
			if (entityInstance.ScriptClass->OnTriggerBeginMethod)
			{
				uint32_t value = entity._id;
				void* args[] = { &value };
				Scripting::CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->OnTriggerBeginMethod, args);
			}
		}
	}

	void ScriptSystem::OnTriggerStay(uint32_t id)
	{
		EntityID entity{ id, 0 };
		EntityInstance& entityInstance = GetEntityInstanceData(entity).m_instance;
		if (GET_ENTITY_MANAGER.getActive(entity))
		{
			if (entityInstance.ScriptClass->OnTriggerStayMethod)
			{
				uint64_t value = entity._id;
				void* args[] = { &value };
				Scripting::CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->OnTriggerStayMethod, args);
			}
		}
	}

	void ScriptSystem::OnTriggerEnd(uint32_t id)
	{
		EntityID entity{ id, 0 };
		EntityInstance& entityInstance = GetEntityInstanceData(entity).m_instance;
		if (GET_ENTITY_MANAGER.getActive(entity))
		{
			if (entityInstance.ScriptClass->OnTriggerEndMethod)
			{
				uint64_t value = entity._id;
				void* args[] = { &value };
				Scripting::CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->OnTriggerEndMethod, args);
			}
		}
	}


	//This will instantiate the entites that have the script attached to it
	void ScriptSystem::InstantiateEntityClass(EntityID entityID)
	{
		auto component = scripts.find(entityID)->second;
		std::string ModuleName = component->GetModuleName();

		EntityInstanceData& instanceData = GetEntityInstanceData(entityID);
		EntityInstance& entityInstance = instanceData.m_instance;
		EosAssert(entityInstance.ScriptClass, "Invalid ScriptClass");

		//Instantiate the class throught function calls
		entityInstance.Handle = InstantiateClass(*entityInstance.ScriptClass);
		//entityInstance.Handle = CreateCSComponent(entityInstance.ScriptClass->Class, entityInstance.ScriptClass->InitMethod, entityID);
		
		void* param[] = { &entityID };
		Scripting::CallMethod(entityInstance.GetInstance(), entityInstance.ScriptClass->InitMethod, param);
		 
		// Set all public fields to appropriate values [Runtime]
		ScriptModuleField& moduleFieldMap = instanceData.m_modulefieldmap;
		if (moduleFieldMap.find(ModuleName) != moduleFieldMap.end())
		{
			auto& pf = moduleFieldMap.at(ModuleName);
			for (auto& [name, field] : pf)
			{
				field.CopyStoredValueToRunTime();
			}

		}

		//mono_gchandle_free(entityInstance.Handle);
		//Calls the constructor in c
		

		//entityInstance.Handle = CreateCSComponent(entityInstance.ScriptClass->Class, entityInstance.ScriptClass->InitMethod, entityID);

		//Calls the OnStart Method in C# 
		OnStart(entityID);

	}

	EntityInstanceData& ScriptSystem::GetEntityInstanceData(EntityID EntityID)
	{
		EosAssert(sData.EntityInstanceMap.find(EntityID._id) != sData.EntityInstanceMap.end(), "Is your script Initialized after inserting the script?");
		auto& entityID = sData.EntityInstanceMap.at(EntityID._id);
		return entityID;
	}

	std::map<EntityID, std::shared_ptr<Script_Component>> ScriptSystem::GetScripts()
	{
		return scripts;
	}

	void ScriptSystem::OnRuntimeUpdate(float ts)
	{
		for (auto& entity : scripts)
		{
			if (ModuleExists(entity.second->GetModuleName()) && entity.second->Active)
				OnUpdate(entity.first, ts);
			else
				PE_CORE_WARN("C# Entity Is Not Updating")

		}
	}

	void ScriptSystem::OnRuntimeFixedUpdate(float ts)
	{
		for (auto& entity : scripts)
		{
			std::shared_ptr<Script_Component> scriptComponent = entity.second;
			if (ModuleExists(scriptComponent->GetModuleName()) && scriptComponent->Active)
				OnFixedUpdate(entity.first, ts);
			else
				PE_CORE_WARN("C# Entity Is Not Updating")
		}
	}

	void ScriptSystem::OnRuntimeStart()
	{
		for (auto& entity : scripts)
		{
			auto& scriptComponent = *entity.second;
			if (ModuleExists(scriptComponent.GetModuleName()) && scriptComponent.Active)
				InstantiateEntityClass(entity.first);
			else
				PE_CORE_WARN("C# Entity Cannot be instantiated.")
		}
	}


	//Figuring out the stop functionality of the scripts
	void ScriptSystem::OnRuntimeStop()
	{
		for (auto& entity : scripts)
		{
			if (ModuleExists(entity.second->GetModuleName()))
				OnScriptComponentDestroyed(entity.first);
			else
				PE_CORE_WARN("C# Entity Cannot be instantiated.")
		}
	}


	void ScriptSystem::AddingNewComponent(EntityID entityID, COMPONENTNAME compName, std::shared_ptr<IComponent> compPtr)
	{
		if (compName == COMPONENTNAME::Comp_Script_Component)
		{
			if (scripts.find(entityID) == scripts.end())
			{
				auto script = std::dynamic_pointer_cast<Script_Component>(compPtr);
				script->ScripRef.m_EntityID = entityID;
				scripts.insert({ entityID, script });
			}
		}
	}
	void ScriptSystem::RemovingComponent(EntityID entityID, COMPONENTNAME compName)
	{
		if (compName == COMPONENTNAME::Comp_Script_Component)
		{
			scripts.erase(entityID);
		}
	}
	void ScriptSystem::ClearAllComponents()
	{
		scripts.clear();

	}

	uint32_t ScriptSystem::CreateCSComponent(MonoClass* cs, MonoMethod* Init, EntityID entity)
	{
		MonoObject* obj = mono_object_new(sData.Domain, cs);
		if (!obj)
		{
			EosAssert(obj, "Failed to create a C# Component from C++ side");
		}

		mono_runtime_object_init(obj);
		if (!obj)
		{
			EosAssert(obj, "Failed to create a C# Component from C++ side");
		}

		const uint32_t handle = mono_gchandle_new(obj, false);


		void* args = &entity;
		mono_runtime_invoke(Init, obj, &args, NULL);

		return handle;

	}

	MonoClass* ScriptSystem::GetClass(MonoImage* image, const CSClass& scriptClass)
	{

		const char* name = scriptClass.ClassName.c_str();
		MonoClass* monoclass = mono_class_from_name(image, "EosScriptCore.Scripts", name);
		if (!monoclass)
			PE_CORE_ERROR("Cannot find class in C#");
		return monoclass;
	}

	//In charge of creating the class for the entity
	uint32_t ScriptSystem::InstantiateClass(CSClass& scriptClass)
	{
		MonoObject* instance = mono_object_new(sData.Domain, scriptClass.Class);
		if (!instance)
			PE_CORE_ERROR("Cannot instantiate C# class!");

		mono_runtime_object_init(instance);
		const uint32_t handle = mono_gchandle_new(instance, false);
		return handle;
	}

	//void ScriptSystem::FreeInstance()
	//{
	//	for (auto ent : sData.EntityInstanceMap)
	//		mono_gchandle_free(ent.second.m_instance.Handle);
	//}

	MonoObject* EntityInstance::GetInstance() const
	{

		if (!Handle)
			PE_CORE_ERROR("Entity has not been instantiated!");

		MonoObject* object = mono_gchandle_get_target(Handle);


		if (!object)
			PE_CORE_ERROR("Entity has not been instantiated!");

		return object;
	}




}
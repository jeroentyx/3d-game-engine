#pragma once
#include <Tools/Singleton.h>
#include <Src/Engine/Core/Interfaces/ISystem.h>
#include "../Src/Engine/Components/Script/ScriptComponent.h"
#include "MonoUtils.hpp"
#include "Fields.hpp"
#include "ScriptData.h"
#include <string>
#include <mono/jit/jit.h>
#include "tracy\Tracy.hpp"

namespace Eos
{

	class ScriptSystem : public ISystem
	{
		

	public:


		static bool IsEntityExist(EntityID);

		static MonoDomain* GetDomain();
		//static void SetSceneContext(const Ref<Scene>& scene);
		//static Ref<Scene> GetSceneContext();
		
		static bool IsEntityModuleValid(EntityID EntityID);
		static bool ModuleExists(const std::string& moduleName);
		
		static void InitScriptEntity(EntityID);
		static void ShutDownScriptEntity(EntityID, const std::string& modulename);
	

		//Get EntityClass
		static MonoClass* GetEntityClass();
		static void InitComponentsClass();

		//C# methods
		static void OnStart(EntityID);
		static void OnUpdate(EntityID, float);
		static void OnFixedUpdate(EntityID, float);
		static void OnColiisonBegin(EntityID, EntityID);
		static void OnCollisionEnd(EntityID, EntityID);
		static void OnTriggerBegin(uint32_t, uint32_t);
		static void OnTriggerStay(uint32_t);
		static void OnTriggerEnd(uint32_t);
		
		static void InstantiateEntityClass(EntityID entityID);
		//static void OnSceneDestruct(int Scene); Will confirm if we need a scene clas
		static void ReloadAssembly(const std::string& path);
		static void OnScriptComponentDestroyed(EntityID EntityID);
		static EntityInstanceMap& GetInstanceMap();

		//Loading Purpose
		static void LoadAssemblies(const std::string& assemblyPath);

		static EntityInstanceData& GetEntityInstanceData(EntityID EntityID);
		static std::map<EntityID, std::shared_ptr<Script_Component>> GetScripts();
		
		//Scripting Functionality
		
		static void OnRuntimeUpdate(float dt);
		static void OnRuntimeFixedUpdate(float ts);
		static void OnRuntimeStart();

		//Stop the script
		static void OnRuntimeStop();



		//Inherited values
		virtual void Init();
		virtual void Update();
		virtual void Exit();
		virtual void AddingNewComponent(EntityID entityID, COMPONENTNAME compName, std::shared_ptr<IComponent> compPtr);
		virtual void RemovingComponent(EntityID entityID, COMPONENTNAME compName);
		virtual void ClearAllComponents();

	
	
	private:
		//static MonoClass* GetClass(MonoImage* img, const CSClass& scriptClass);
		//static uint32_t InstantiateClass(CSClass& scriptClass);
		//std::map<int, std::shared_ptr<Script_Component>> scripts;
		static std::map<EntityID, std::shared_ptr<Script_Component>> scripts;
		
		//For getting methods
		MonoImage* m_image = nullptr;
		MonoImage* m_typeImage = nullptr;

		static uint32_t CreateCSComponent(MonoClass* cs, MonoMethod* Init ,EntityID Id);
		static MonoClass* GetClass(MonoImage* image, const CSClass& scriptClass);
		static uint32_t InstantiateClass(CSClass& scriptClass);

		

	};

}


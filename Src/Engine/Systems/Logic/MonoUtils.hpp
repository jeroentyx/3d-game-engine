#pragma once
#include "ScriptingSystem.hpp"
#include <Mono/metadata/object.h>


namespace Eos::Scripting
{
	MonoDomain* CreateDomain(char* Name);
	MonoDomain* CreateAppDomain(char* Name);
	void SetDomain(MonoDomain* domain);
	MonoAssembly* LoadAssembly(const char* path);
	MonoImage* GetAssemblyImage(MonoAssembly* assmebly);

	//Function Utility
	MonoMethod* GetMethod(MonoImage* img, const std::string& methodName);
	MonoObject* CallMethod(MonoObject* object, MonoMethod* method, void** params = nullptr);
	

	//String
	MonoString* ConvertCppStringToMonoString(MonoDomain* domain, const  std::string& str);
	std::string ConvertMonoStringToCppString(MonoString* message);
	char*		ConvertMonoObjectToCppChar(MonoObject* obj);



	//CppContainersToC#
	template<typename containerT>
	MonoArray* ConvertCppContainersToMonoArray(const containerT& obj)
	{
		MonoArray* array;
		MonoClass* arrayclass = ScriptSystem::GetEntityClass();
		array = mono_array_new(ScriptSystem::GetDomain(), arrayclass, obj.size());

		std::size_t counter = 0;
		for (auto& i : obj)
		{
			mono_array_set(array, EntityWrapper, counter, i);

			++counter;
		}


		return array;
	}
}
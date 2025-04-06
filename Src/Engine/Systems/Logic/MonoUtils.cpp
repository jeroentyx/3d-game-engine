#include "../Src/pch.h"
#include "MonoUtils.hpp"
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/jit/jit.h>

MonoDomain* Eos::Scripting::CreateDomain(char* Name)
{
	MonoDomain* domain = mono_domain_create_appdomain(Name, nullptr);
	EosAssert(domain, "MonoDomain creation FAILURE!");
	return domain;
}

MonoDomain* Eos::Scripting::CreateAppDomain(char* Name)
{
	MonoDomain* appdomain = mono_domain_create_appdomain((char*)Name, NULL);
	return appdomain;
}

void Eos::Scripting::SetDomain(MonoDomain* domain)
{
	mono_domain_set(domain, false);
}

MonoAssembly* Eos::Scripting::LoadAssembly(const char* path)
{
	auto buffer = FileManager::ReadBinaryFile(path);

	MonoImageOpenStatus status;
	MonoImage* image = mono_image_open_from_data_full(buffer.data(), static_cast<uint32_t>(buffer.size()), 1, &status, 0);
	if (status != MONO_IMAGE_OK)
	{
		//Logging
		return nullptr;
	}

	MonoAssembly* assembly = mono_assembly_load_from_full(image, path, &status, 0);

	//Free the image
	mono_image_close(image);
	image = nullptr;

	if (!assembly) {}
		//Logging Cannot load Assembly

	return assembly;

}

MonoImage* Eos::Scripting::GetAssemblyImage(MonoAssembly* assembly)
{
	MonoImage* image = mono_assembly_get_image(assembly);
	if (!image) {}
	//Logging Cannot find image from assembly

	return image;
}

MonoMethod* Eos::Scripting::GetMethod(MonoImage* img, const std::string& methodName)
{

	MonoMethodDesc* description = mono_method_desc_new(methodName.c_str(), NULL);

	if (!description) 
		PE_CORE_WARN("Description not Found");

	MonoMethod* method = mono_method_desc_search_in_image(description, img);

	if (!method) 
		PE_CORE_WARN("Method Not Found {}", methodName);
	
	return method;
}

MonoObject* Eos::Scripting::CallMethod(MonoObject* object, MonoMethod* method, void** params)
{
	MonoObject* pException = nullptr;
	MonoObject* result = mono_runtime_invoke(method, object, params, nullptr);
	EosAssert(!pException, "Cannot invoke C# method");

	return result;
}


MonoString* Eos::Scripting::ConvertCppStringToMonoString(MonoDomain* domain, const std::string& str)
{
	return mono_string_new(domain, str.c_str());
}

std::string Eos::Scripting::ConvertMonoStringToCppString(MonoString* message)
{
	char* ptr = mono_string_to_utf8(message);
	std::string s{ ptr };
	mono_free(ptr);
	return s;
}

char* Eos::Scripting::ConvertMonoObjectToCppChar(MonoObject* obj)
{
	if (obj == NULL)
	{
		char* a = nullptr;
		return a;
	}
	else
	{
		MonoString* a = mono_object_to_string(obj, NULL);
		std::string b = ConvertMonoStringToCppString(a);
		char* s = _strdup(b.c_str());
		return s;
	}
}

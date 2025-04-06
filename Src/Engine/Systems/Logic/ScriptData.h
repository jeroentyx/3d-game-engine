#pragma once
#include <mono/jit/jit.h>
#include <unordered_map>
#include "Src/Engine/Systems/Logic/Fields.hpp"

namespace Eos
{
	struct CSClass;

	struct EntityWrapper
	{
		uint32_t _id;
		uint32_t _generations;
	};

	struct QuaternionWrapper
	{
		float x;
		float y;
		float z;
		float w;
	};

	struct Vector3Wrapper
	{
		float x;
		float y;
		float z;
	};

	struct EntityInstance
	{
		CSClass* ScriptClass = nullptr;
		uint32_t Handle = 0;
		MonoObject* GetInstance() const;
	};

	// Mapped as-> ModuleName --- (FieldName - PublicFieldData)
	using ScriptModuleField = std::unordered_map<std::string, std::unordered_map<std::string, PublicField>>;

	struct EntityInstanceData
	{
		EntityInstance m_instance;
		ScriptModuleField m_modulefieldmap;
	};

	using EntityInstanceMap = std::unordered_map<uint32_t, EntityInstanceData>;
}
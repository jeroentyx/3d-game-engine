#pragma once
#include "Fields.hpp"
#include "../Src/pch.h"	
#include "../Src/Engine/Systems/Logic/ScriptingSystem.hpp"	
#include <Mono/metadata/object.h>

namespace Eos
{
	static uint32_t GetFieldType(FieldType type)
	{
		switch (type)
		{
		case FieldType::BOOL:  return 1;
		case FieldType::FLOAT: return 4;
		case FieldType::INT:   return 4;
		case FieldType::UINT:  return 4;
		case FieldType::STRING:return 8;
		case FieldType::VEC2:  return 4 * 2;
		case FieldType::VEC3:  return 4 * 3;
		case FieldType::VEC4:  return 4 * 4;
		}
		return 0;
	}

	PublicField::PublicField(PublicField&& other) noexcept
	{
		m_name = std::move(other.m_name);
		m_Typename = std::move(other.m_Typename);
		//m_name = other.m_name;
		//m_Typename = other.m_Typename;
		m_Type = other.m_Type;

		m_EntityInstance = other.m_EntityInstance;
		m_MonoClassField = other.m_MonoClassField;
		m_storedValueBuffer = other.m_storedValueBuffer;

		other.m_EntityInstance = nullptr;
		other.m_MonoClassField = nullptr;
		other.m_storedValueBuffer = nullptr;
	}

	PublicField::PublicField(const std::string& name, const std::string& typeName, FieldType type) :
		m_name(name), m_Typename(typeName), m_Type(type)
	{
		//Allocate	memory	for the given type
		m_storedValueBuffer = AllocateBuffer(type);
	}
	PublicField::~PublicField()
	{
		delete[] m_storedValueBuffer;
	}
	void PublicField::CopyStoredValueToRunTime()
	{
		switch (m_Type)
		{
		case FieldType::BOOL:
			mono_field_set_value(m_EntityInstance->GetInstance(), m_MonoClassField, m_storedValueBuffer);
			break;
		case FieldType::INT:
			mono_field_set_value(m_EntityInstance->GetInstance(), m_MonoClassField, m_storedValueBuffer);
			break;
		case FieldType::FLOAT:
			mono_field_set_value(m_EntityInstance->GetInstance(), m_MonoClassField, m_storedValueBuffer);
			break;
		case FieldType::UINT:
			mono_field_set_value(m_EntityInstance->GetInstance(), m_MonoClassField, m_storedValueBuffer);
			break;
		case FieldType::VEC3:
			mono_field_set_value(m_EntityInstance->GetInstance(), m_MonoClassField, m_storedValueBuffer);
			break;

		}
	}
	bool PublicField::IsRunTimeAvaliable() const
	{
		return m_EntityInstance->Handle != 0;
	}

	void PublicField::SetStoredValueRaw(void* src)
	{
		memcpy(m_storedValueBuffer, src, GetFieldType(m_Type));
	}

	void PublicField::SetRuntimeValueRaw(void* src)
	{
		mono_field_set_value(m_EntityInstance->GetInstance(), m_MonoClassField, src);
	}

	void* PublicField::GetRuntimeValueRaw()
	{
		std::byte* outValue = nullptr;
		mono_field_get_value(m_EntityInstance->GetInstance(), m_MonoClassField, outValue);
		return outValue;
	}

	uint8_t* PublicField::AllocateBuffer(FieldType type)
	{
		uint32_t size = GetFieldType(type);
		uint8_t *buffer = new uint8_t[size];
		//std::memset(buffer, 0, size);
		memset(buffer, 0, size);
		return buffer;
	}

	void PublicField::SetStoredValueInternal(void *value) const
	{
		memcpy(m_storedValueBuffer, value, GetFieldType(m_Type));
	}

	void PublicField::GetStoreValueInternal(void* outValue) const
	{
		//memcpy(outValue, m_storedValueBuffer, GetFieldType(m_Type));
		memcpy(outValue, m_storedValueBuffer, GetFieldType(m_Type));
	}

	void PublicField::SetRunTimeValueInternal(void* value) const
	{
		mono_field_set_value(m_EntityInstance->GetInstance(), m_MonoClassField, value);
	}

	void PublicField::GetRunTimeValueInternal(void* outValue) const
	{
		mono_field_get_value(m_EntityInstance->GetInstance(), m_MonoClassField, outValue);
	}

}

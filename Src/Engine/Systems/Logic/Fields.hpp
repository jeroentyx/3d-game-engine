#pragma once
#include "../Src/pch.h"
#include "../Tools/Singleton.h"

extern "C"
{
	typedef struct _MonoClassField MonoClassField;
}

namespace Eos
{
	struct EntityInstance;

	enum class FieldType
	{
		NONE = 0,
		BOOL,
		INT,
		FLOAT,
		UINT,
		STRING,
		VEC2,
		VEC3,
		VEC4
	};

	class PublicField
	{
	public:
		std::string m_name;
		std::string m_Typename;
		FieldType m_Type; //Member 
		MonoClassField* m_MonoClassField;
		PublicField() = default;

		PublicField(const std::string& name, const std::string& typeName, FieldType type);
		PublicField(const PublicField&) = delete;
		PublicField(PublicField&& other) noexcept;
		~PublicField();

		void CopyStoredValueToRunTime();
		bool IsRunTimeAvaliable()const;

		void SetStoredValueRaw(void* src);
		void* GetStoredValueRaw() { return m_storedValueBuffer; }


		template<typename T>
		T GetStoredValue() const
		{
			T value;
			GetStoreValueInternal(&value);
			return value;
		}

		template<typename T>
		void SetStoredValue(T value) const
		{
			SetStoredValueInternal(&value);
		}

		template<typename T>
		T GetRuntimeValue() const
		{
			T value;
			GetRunTimeValueInternal(&value);
			return value;
		}

		template<typename T>
		void SetRuntimeValue(T value) const
		{
			SetRunTimeValueInternal(&value);
		}

		void SetRuntimeValueRaw(void* src);
		void* GetRuntimeValueRaw();

	private:

		uint8_t* AllocateBuffer(FieldType type);
		void SetStoredValueInternal(void* value) const;
		void GetStoreValueInternal(void* outValue) const;
		void SetRunTimeValueInternal(void* value) const;
		void GetRunTimeValueInternal(void* outValue) const;

	private:
		EntityInstance* m_EntityInstance;
		
		uint8_t* m_storedValueBuffer;

		friend class ScriptSystem;
	};
}
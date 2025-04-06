#pragma once

#include <string>

namespace Eos
{
	template <typename T>
	class HashEntry
	{
		std::string _key;
		T _value;

	public:
		HashEntry(std::string key, T value) : 
			_key{ key },
			_value{ value }
		{
		}

		~HashEntry()
		{
		}

		std::string getKey()
		{
			return _key;
		}

		T getValue()
		{
			return _value;
		}
	};
}
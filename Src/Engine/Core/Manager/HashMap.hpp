#pragma once

#include "HashEntry.hpp"

template <typename T>
class HashEntry;

namespace Eos
{
	template <typename T>
	class HashMap
	{
		const int TABLE_SIZE = 256;
		HashEntry<T>** table;			// Pointer to pointers

	public:
		HashMap()
		{
			table = new HashEntry<T> * [TABLE_SIZE];
			for (int i = 0; i < TABLE_SIZE; ++i)
				table[i] = nullptr;
		}

		~HashMap()
		{
			for (int i = 0; i < TABLE_SIZE; ++i)
				if (table[i] != nullptr)
					delete table[i];

			delete[] table;
		}

		void put(std::string key, T value)
		{
			// Look through each of the slots to find a empty slot
			for (int i = 0; i < TABLE_SIZE; ++i)
				if (table[i] == nullptr)
					table[i] = new HashEntry(key, value);
		}

		T get(std::string key)
		{
			// Look through each of the slots to find a slot with a matching key value
			for (int i = 0; i < TABLE_SIZE; ++i)
				if (table[i]->getKey() == key)
					return table[i]->getValue();

			// nothing found
			return nullptr;
		}
	};
}
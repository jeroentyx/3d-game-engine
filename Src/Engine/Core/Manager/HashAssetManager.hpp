#pragma once
#include "HashMap.hpp"

namespace Eos
{
	enum class ResourceType
	{
		None = 0,
		Fonts,
		Animations
	};

	class Hash_AssetManager
	{
		HashMap<bool>* m_fonts;
		HashMap<bool>* m_animations;

	public:
		Hash_AssetManager();
		Hash_AssetManager(const Hash_AssetManager&) = default;
		Hash_AssetManager& operator=(const Hash_AssetManager&) = default;
		~Hash_AssetManager();

		template <typename T>
		T* findResource(ResourceType type, std::string name);

		template <typename T>
		void addResource(ResourceType type, std::string name, T value);
	};
}
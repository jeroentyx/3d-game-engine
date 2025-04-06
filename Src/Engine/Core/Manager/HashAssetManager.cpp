#include "HashAssetManager.hpp"

namespace Eos
{
	Hash_AssetManager::Hash_AssetManager()
	{
		m_fonts = new HashMap<bool>();
		m_animations = new HashMap<bool>();
	}

	Hash_AssetManager::~Hash_AssetManager()
	{
		delete m_fonts;
		delete m_animations;
	}

	template<typename T>
	T* Hash_AssetManager::findResource(ResourceType type, std::string name)
	{
		//return nullptr;
		switch (type)
		{
		case ResourceType::None:
		default: return nullptr;

		case ResourceType::Fonts:
			return (T*)m_fonts->get(name);
		}
	}

	template<typename T>
	void Hash_AssetManager::addResource(ResourceType type, std::string name, T value)
	{
		switch (type)
		{
		case ResourceType::None:
		default: break;

		case ResourceType::Fonts:
			m_fonts->put(name, value);
		}
	}
}
#pragma once
#include "Tools\Singleton.h"
#include <vector>
#include <string>

namespace Eos
{
	struct EntityManagerStorage : public Singleton<EntityManagerStorage>
	{
		using entityMap = std::vector<std::pair<int, std::string>>;
		entityMap ScenePool;
	};
}
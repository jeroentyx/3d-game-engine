#pragma once

#include "IImGuiWindow.h"
#include "..\pch.h"
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include "Src\Engine\Core\Manager\EntityManager.h"
#include "Src\Engine\Core\Manager\EntitiesIDManager.h"
#include "Src\Engine\Core\Manager\HistoryManager.h"

namespace Eos
{
	class SceneHierachy : public IImGuiWindow
	{
		using SceneHierachyMap = std::vector<std::pair<int, std::string>>;
	public:
		SceneHierachy();
		void LoadAllEntitiesInHierachy();
		virtual void Update() override;
		SceneHierachyMap sceneHierachypool;
		EntityID m_SelectedID = NULLENTITY;

		//Function by Ben for Parent Child Hierachy
		void makeTree(const std::map<EntityID, EntityID> &map, EntityID); //Recursive Function
		bool haveChild(const std::map<EntityID, EntityID>& map, EntityID);
	};
}
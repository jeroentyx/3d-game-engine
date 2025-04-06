#pragma once

#include "../../ComponentSystemID.h"
#include <memory>
#include"Src/Engine/Core/Manager/EntityID.h"

class IComponent;

class ISystem
{
public:
	ISystem() {}
	virtual ~ISystem() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;

	//Only used if System manages any components
	virtual void AddingNewComponent(EntityID entityID, COMPONENTNAME compName, std::shared_ptr<IComponent> compPtr) = 0;
	virtual void RemovingComponent(EntityID entityID, COMPONENTNAME compName) = 0;
	virtual void ClearAllComponents() = 0;
};
#pragma once
#include "../../Core/Interfaces/ISystem.h"
#include "../../Core/Interfaces/IComponent.h"
#include "../../ComponentSystemID.h"
#include "../Src/Engine/Core/Manager/ComponentManager.h"
#include "../Src/Engine/Systems/Graphics/Graphics_System.h"
#include <map>
#include "tracy\Tracy.hpp"

enum class UI_TYPE 
{
	BUTTON,
	OBJECT
};

class UI_Component : public IComponent
{
	friend class UI_System;
	virtual void Init() override;
public:
	UI_TYPE type = UI_TYPE::BUTTON;
	glm::vec2 pos = { glm::vec2{0.0f} };
};

class UI_System : public ISystem
{
	//Map to keep track of Audio Components
	std::map<EntityID, std::shared_ptr<UI_Component>> uiComps;
	std::map<EntityID, std::shared_ptr<UI_Sprite2D_Component>> sprites;

	glm::vec3 cam = glm::vec3{ 0.0f };

public:

	//Overloaded stuff from ISystem
	void Init();
	void Update();
	void Exit();
	void AddingNewComponent(EntityID entityID, COMPONENTNAME compName, std::shared_ptr<IComponent> compPtr);
	void RemovingComponent(EntityID entityID, COMPONENTNAME compName);
	void ClearAllComponents();


	// Billboard
	std::shared_ptr<Texture2D> billboard;
	Eos::Shader bill_shader;
	std::shared_ptr<VertexArray> m_testVA;
	bool isFixedSize = false;				// Hard code value to control if fixed shape or not inside shader
};
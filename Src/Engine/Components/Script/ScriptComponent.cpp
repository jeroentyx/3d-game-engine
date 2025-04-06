#include "ScriptComponent.h"
#include "Src/Engine/Frameworks/Serialization/SerializeManager.h"
#include "Src/pch.h"
#include "Src/Engine/Systems/Logic/ScriptingSystem.hpp"



void Script_Component::Init()
{
	/*ScripRef.Modulename = GetModuleName();
	Eos::ScriptSystem::InitScriptEntity(ScripRef.m_EntityID);*/
}

std::string Script_Component::GetModuleName() const
{
	return ModuleName;
}

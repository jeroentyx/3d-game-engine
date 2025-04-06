#pragma once
#include "Src/Engine/Core/Interfaces/IComponent.h"
#include "Src/Engine/Systems/Logic/Fields.hpp"
#include "Src/Engine/Systems/Logic/ScriptData.h"
#include <string>


using namespace Eos;

class Script_Component : public IComponent
{
public:

	std::string				ModuleName;
	bool					Active			= true;
	//EntityInstanceData		instance;
	script_ref				ScripRef;


	void Init() override;

	std::string GetModuleName() const;
	property_vtable()
};
property_begin(Script_Component)
{ property_var(ModuleName)
, property_var(Active)
, property_var(ScripRef)
}property_vend_h(Script_Component)
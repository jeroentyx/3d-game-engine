#pragma once

#include "../../../Engine/ComponentSystemID.h"
#include "../Src/Engine/Frameworks/Serialization/ISerializable.h"



//Virtual base class
class IComponent : property::base
{
public:
	COMPONENTNAME componentType = COMPONENTNAME::Comp_None;

	virtual void Init() = 0;


	virtual ~IComponent() = default;

	property_vtable()
};
property_begin(IComponent)
{
}
property_vend_h(IComponent)
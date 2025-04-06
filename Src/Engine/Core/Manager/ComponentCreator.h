#pragma once
#include "../../ComponentSystemID.h"
#include <memory>
class IComponent;

class ComponentCreator
{
public:
	ComponentCreator(COMPONENTNAME typeId)
		:TypeId(typeId)
	{
	}

	COMPONENTNAME TypeId;
	///Create the component
	virtual std::shared_ptr<IComponent> Create() = 0;
};

///Templated helper class used to make concrete component creators.
template<typename type>
class ComponentCreatorType : public ComponentCreator
{
public:
	ComponentCreatorType(COMPONENTNAME typeId)
		:ComponentCreator(typeId)
	{
	}

	virtual std::shared_ptr<IComponent> Create()
	{
		//return new type();
		return std::make_shared<type>();
	}

	/*ComponentCreatorType(ComponentCreatorType&& source)
		:ComponentCreator(source.TypeId)
	{
	}

	ComponentCreatorType& operator=(ComponentCreatorType&& source) 
	{
		std::swap(TypeId, source.TypeId);
		return *this;
	}*/

};

//Register component macro
//#define RegisterComponent(type) FACTORY->AddComponentCreator( COMPONENTNAME::Comp_##type, new ComponentCreatorType<type>( COMPONENTNAME::Comp_##type ) );
#define RegisterComponent(type) FACTORY->AddComponentCreator( COMPONENTNAME::Comp_##type, std::make_shared<ComponentCreatorType<type>>( COMPONENTNAME::Comp_##type ) );

;
#pragma once
//#include "LIONant/include/PropertyConfig.h"
#include "LIONant/include/Properties.h"
#include "../Src/Engine/Core/Manager/EntityID.h"
//#include "Src/pch.h"
#include "Reader.h"
#include "Writer.h"

class ISerializable : property::base
{

public:

	ISerializable() = default;

	//Reading from file to initialize all entities, componenets and systems
	virtual bool Load(const std::string& DataToBeRead, JsonReader& _reader, EntityID ID) = 0;

	//Saving the File into the json string then into the file
	virtual bool Save(const std::string& DataToBeSave, JsonWriter& _writer, EntityID ID) = 0;

	virtual ~ISerializable() = default;

	property_vtable()
};
property_begin(ISerializable)
{
}
property_vend_h(ISerializable)
































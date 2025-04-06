#pragma once
#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>
#include <string>
#include <iostream>


class JsonWriter
{

	void* m_Writer; //Json Writer
	void* m_Stream; //Stream Buffer, holds the data
	bool  m_Error;  // Whether an error has occurred.

public:
	//Constructor
	JsonWriter();
	//Destructor
	~JsonWriter();

	//Copy Constructor
	JsonWriter(const JsonWriter&);
	//Copy Assignment
	JsonWriter& operator=(const JsonWriter&);

	//Returns the serialized JSON string.
	const char* GetString() const;

	//Member Function
	operator bool() const { return true; }

	bool Good() const;

	//Member Functions for writing into an empty object
	JsonWriter& StartObject();
	JsonWriter& Member(const char* name);
	bool HasMember(const char* name) const;
	JsonWriter& EndObject();

	//Member Functions for writing into an array
	JsonWriter& StartArray();
	JsonWriter& EndArray();

	//Checking for data to be written in
	JsonWriter& operator&(bool& b);
	JsonWriter& operator&(unsigned& u);
	JsonWriter& operator&(int& i);
	JsonWriter& operator&(double& d);
	JsonWriter& operator&(float& f);
	JsonWriter& operator&(std::string& s);
	JsonWriter& operator&(const char* c);
	JsonWriter& SetNull();

	//This will be for the vector serialization
/*
*	//2d
	template<typename T = float>
	JsonReader& operator&(//Vector);

	//3d
	template<

	//glm3

	//glm2

	//glm4

*/
};
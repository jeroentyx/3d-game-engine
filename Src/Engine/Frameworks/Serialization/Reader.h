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
#include <stack>


class JsonReader
{

	void Next();

	// PIMPL
	void* m_Document;			   // DOM result of parsing.
	void* m_Stack;                 // Stack for iterating the DOM
	bool  m_Error;                 // Whether an error has occurred.

public:

	//Constructor that takes in the a string from the json format
	JsonReader();
	JsonReader(const char* json);
	JsonReader(const JsonReader&);
	JsonReader& operator=(const JsonReader&);

	//Destructor
	~JsonReader();

	//This will be archive concept where it can be either a reader or writer 
	//It will be the same concept for writer

	bool CheckForError(const char* json);


	operator bool() const
	{
		return !m_Error;
	}
	//Member Function for reading an object
	JsonReader& StartObject();
	JsonReader& Member(const char* name);
	bool HasMember(const char* name)const;
	JsonReader& EndObject();
	bool Good() const;
	

	JsonReader& IsObject();
	JsonReader& GetName(std::string&);
	//Member function for reading an array
	JsonReader& StartArray(size_t* size = 0);
	JsonReader& EndArray();

	//Checking for Data in the array or object
	JsonReader& operator&(bool& b);
	JsonReader& operator&(int& i);
	JsonReader& operator&(std::string& string);
	JsonReader& operator&(double& db);
	JsonReader& operator&(unsigned& b);
	JsonReader& operator&(float& f);


	//This will be for the vector deserialization
	/*
	*	//2d
		template<typename T = float>
		JsonReader& operator&(//Vector);

		//3d
		template<typename T = float>
		JsonReader& operator&(//Vector);

		//glm3

		//glm2

		//glm4

	*/
	JsonReader& SetNull();

};
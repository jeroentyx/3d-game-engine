#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include "ISerializable.h"


//Would require the component settings for collision, transform coordinates
//Graphics Settings


class SerializeManager : public ISerializable
{
public:

	SerializeManager();
	~SerializeManager() = default;

	//Reading from file to initialize all entities, componenets and systems
	virtual bool Load(const std::string& data, JsonReader& _reader, EntityID ID) override;

	//Saving the File into the json string then into the file
	virtual bool Save(const std::string& data, JsonWriter& _writer,EntityID ID) override;



	//Reading
	static void ReadBool(const char* _name, bool& b, JsonReader& _reader);
	static void ReadInt(const char* _name, int& i, JsonReader& _reader);
	static void ReadString(const char* _name, std::string& s, JsonReader& _reader);
	static void ReadDouble(const char* _name, double& db, JsonReader& _reader);
	static void ReadUInt(const char* _name, unsigned& us, JsonReader& _reader);
	static void ReadFloat(const char* _name, float& f, JsonReader& _reader);
	static void ReadScript(const char* name, script_ref& scr, JsonReader& _reader);
	static void ReadMeshType(const char* name, MeshType& scr, JsonReader& _reader);
	static void ReadShaderType(const char* name, ShaderType& scr, JsonReader& _reader);
	
	
	template<typename T = float>
	static void ReadVector(const char* _name, glm::vec2& _coord, JsonReader& _reader)
	{
		if (_reader.Good())
		{

			if (_name != nullptr)
			{
				if (_reader.HasMember(_name))
				{
					_reader.Member(_name);
					_reader.StartObject();
				}
			}

			if (typeid(T) == typeid(double) || typeid(T) == typeid(float))
			{
				ReadFloat("x", (_coord.x), _reader);
				ReadFloat("y", (_coord.y), _reader);
			}

			if (_name != nullptr)
			{
				_reader.EndObject();
			}

		}
	}



	template<typename T = float>
	static void ReadVector(const char* _name, glm::vec<3, T, glm::defaultp>& _coord, JsonReader& _reader)
	{
		if (_reader.Good())
		{
		
			if (_name != nullptr)
			{
				if (_reader.HasMember(_name))
				{
					_reader.Member(_name);
					_reader.StartObject();
				}
			}

			{
				if (typeid(T) == typeid(double) || typeid(T) == typeid(float))
				{
					ReadFloat("x", _coord.x, _reader);
					ReadFloat("y", _coord.y, _reader);
					ReadFloat("z", _coord.z, _reader);
				}
			}

			if (_name != nullptr)
			{
				_reader.EndObject();
			}

		}
	}

	template<typename T = float>
	static void ReadVector(const char* _name, glm::vec<4, T, glm::defaultp>& _coord, JsonReader& _reader)
	{
		if (_reader.Good())
		{

			if (_name != nullptr)
			{
				if (_reader.HasMember(_name))
				{
					_reader.Member(_name);
					_reader.StartObject();
				}
			}

			if (typeid(T) == typeid(double) || typeid(T) == typeid(float))
			{
				ReadFloat("x", _coord.x, _reader);
				ReadFloat("y", _coord.y, _reader);
				ReadFloat("z", _coord.z, _reader);
				ReadFloat("w", _coord.w, _reader);
			}

			if (_name != nullptr)
			{
				_reader.EndObject();
			}

		}
	}

	


	//Writing
	static void WriteBool	(const char* _name, bool& b, JsonWriter& _writer);
	static void WriteInt	(const char* _name, int& i, JsonWriter& _writer);
	static void WriteString	(const char* _name, std::string& s, JsonWriter& _writer);
	static void WriteDouble	(const char* _name, double& db, JsonWriter& _writer);
	static void WriteUInt	(const char* _name, unsigned& us, JsonWriter& _writer);
	static void WriteFloat	(const char* _name, float& f, JsonWriter& _writer);
	static void WriteScript(const char* name, script_ref& scr, JsonWriter& _writer);
	static void WriteMeshType(const char* name, MeshType& scr, JsonWriter& _writer);
	static void WriteShaderType(const char* name, ShaderType& scr, JsonWriter& _writer);


	static void WriteVector(const char* _name, glm::vec2& _coord, JsonWriter& _writer)
	{
		if (_writer.Good())
		{
			_writer.StartObject();
			_writer.Member("name")& _name;
			_writer.Member("type")& "vector2";
			_writer.Member("x")& _coord.x;
			_writer.Member("y")& _coord.y;
			_writer.EndObject();
		}
	}

	static void WriteVector(const char* _name, glm::vec3& _coord, JsonWriter& _writer)
	{
		if (_writer.Good())
		{
			_writer.StartObject();

			_writer.Member("name")& _name;
		
			{
				static std::string String("vector3");
				_writer.Member("type")& String;
				_writer.Member("x")& _coord.x;
				_writer.Member("y")& _coord.y;
				_writer.Member("z")& _coord.z;
			}
			_writer.EndObject();
		}
	}

	static void WriteVector(const char* _name, glm::vec4& _coord, JsonWriter& _writer)
	{
		if (_writer.Good())
		{
			_writer.StartObject();

			_writer.Member("name")& _name;

			{
				static std::string String("vector4");
				_writer.Member("type")& String;
				_writer.Member("x")& _coord.x;
				_writer.Member("y")& _coord.y;
				_writer.Member("z")& _coord.z;
				_writer.Member("w")& _coord.w;
			}

			_writer.EndObject();
		}
	}


private:

};







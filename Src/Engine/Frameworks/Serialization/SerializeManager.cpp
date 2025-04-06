#include "SerializeManager.h"

SerializeManager::SerializeManager()
{
}

bool SerializeManager::Load(const std::string& data, JsonReader& _reader, EntityID test)
{
	(void)data;
	(void)_reader;
	(void)test;
	return false;
}

bool SerializeManager::Save(const std::string& data, JsonWriter& _writer, EntityID test)
{
	(void)data;
	(void)_writer;
	(void)test;
	return false;
}


void SerializeManager::ReadBool(const char* _name, bool& b, JsonReader& _reader)
{
	if (_reader.Good())
	{
		_reader.Member(_name)& b;
	}
}

void SerializeManager::ReadInt(const char* _name, int& i, JsonReader& _reader)
{
	if (_reader.Good())
	{
		_reader.Member(_name)& i;
	}
}

void SerializeManager::ReadString(const char* _name, std::string& s, JsonReader& _reader)
{
	if (_reader.Good())
	{
		_reader.Member(_name)& s;
	}
}

void SerializeManager::ReadDouble(const char* _name, double& db, JsonReader& _reader)
{
	if (_reader.Good())
	{
		_reader.Member(_name)& db;
	}
}

void SerializeManager::ReadUInt(const char* _name, unsigned& us, JsonReader& _reader)
{
	if (_reader.Good())
	{
		_reader.Member(_name)& us;
	}
}

void SerializeManager::ReadFloat(const char* _name, float& f, JsonReader& _reader)
{
	if (_reader.Good())
	{
		_reader.Member(_name)& f;
	}
}

void SerializeManager::ReadScript(const char* name, script_ref& scr, JsonReader& _reader)
{
	(void)name;
	(void)scr;
	(void)_reader;
}

void SerializeManager::ReadMeshType(const char* name, MeshType& scr, JsonReader& _reader)
{
	if (_reader.Good())
	{
		int meshtype = 0;
		_reader.Member(name)& meshtype;
		scr = static_cast<MeshType>(meshtype);
	}
}

void SerializeManager::ReadShaderType(const char* name, ShaderType& scr, JsonReader& _reader)
{
	if (_reader.Good())
	{
		int shadertype = 0;
		_reader.Member(name)& shadertype;
		scr = static_cast<ShaderType>(shadertype);
	}
}

void SerializeManager::WriteBool(const char* _name, bool& b, JsonWriter& _writer)
{
	_writer.StartObject();
	_writer.Member("name")& _name;
	_writer.Member("type") & "bool";
	_writer.Member(_name)& b;
	_writer.EndObject();
}

void SerializeManager::WriteInt(const char* _name, int& i, JsonWriter& _writer)
{
	_writer.StartObject();
	_writer.Member("name")& _name;
	_writer.Member("type") & "int";
	_writer.Member(_name)& i;
	_writer.EndObject();
}

void SerializeManager::WriteString(const char* _name, std::string& s, JsonWriter& _writer)
{
	_writer.StartObject();
	_writer.Member("name")& _name;
	_writer.Member("type") & "string";
	_writer.Member(_name)& s;
	_writer.EndObject();
}

void SerializeManager::WriteDouble(const char* _name, double& db, JsonWriter& _writer)
{
	_writer.StartObject();
	_writer.Member("name")& _name;
	_writer.Member("type") & "f64";
	_writer.Member(_name)& db;
	_writer.EndObject();
}

void SerializeManager::WriteUInt(const char* _name, unsigned& us, JsonWriter& _writer)
{
	_writer.StartObject();
	_writer.Member("name")& _name;
	_writer.Member("type") & "Uint";
	_writer.Member(_name)& us;
	_writer.EndObject();
}

void SerializeManager::WriteFloat(const char* _name, float& f, JsonWriter& _writer )
{
	_writer.StartObject();
	_writer.Member("name")& _name;
	_writer.Member("type") & "f32";
	_writer.Member(_name)& f;
	_writer.EndObject();
}

void SerializeManager::WriteScript(const char* _name, script_ref& f, JsonWriter& _writer)
{
	_writer.StartObject();
	_writer.Member("name")& _name;
	_writer.Member("type") & "ScripRef";
	_writer.Member(_name)& f.m_EntityID._id;
	_writer.Member("ScriptName")& f.Modulename;
	_writer.EndObject();
}

void SerializeManager::WriteMeshType(const char* _name, MeshType& scr, JsonWriter& _writer)
{
	int test = static_cast<int>(scr);
	_writer.StartObject();
	_writer.Member("name")& _name;
	_writer.Member("type") & "MeshType";
	_writer.Member(_name)& test;
	_writer.EndObject();
}

void SerializeManager::WriteShaderType(const char* _name, ShaderType& scr, JsonWriter& _writer)
{
	int test = static_cast<int>(scr);
	_writer.StartObject();
	_writer.Member("name")& _name;
	_writer.Member("type") & "ShaderType";
	_writer.Member(_name)& test;
	_writer.EndObject();
}




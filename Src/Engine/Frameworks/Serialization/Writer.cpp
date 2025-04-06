#include "Writer.h"


/////////////////////////////////////////////////////////////////////////
//JsonWriter
/////////////////////////////////////////////////////////////////////////

//Defining WRITER and STREAM 
#define WRITER reinterpret_cast<rapidjson::PrettyWriter<rapidjson::StringBuffer>*>(m_Writer)
#define STREAM reinterpret_cast<rapidjson::StringBuffer*>(m_Stream)

//This would allocate memory to m_stream and m_Writer for the API in rapidjson
JsonWriter::JsonWriter() :m_Writer(), m_Stream(), m_Error(false)
{
	m_Stream = new rapidjson::StringBuffer;
	m_Writer = new rapidjson::PrettyWriter<rapidjson::StringBuffer>(*STREAM);
}

JsonWriter::~JsonWriter()
{
	delete WRITER;
	delete STREAM;
}

JsonWriter::JsonWriter(const JsonWriter& original) :m_Stream{ nullptr }, m_Writer{ nullptr }, m_Error(false)
{
	if (this != &original)
	{
		m_Stream = original.m_Stream;
		m_Writer = original.m_Writer;
	}
}

JsonWriter& JsonWriter::operator=(const JsonWriter& rhs)
{
	if (this != &rhs)
	{
		m_Stream = rhs.m_Stream;
		m_Writer = rhs.m_Writer;
	}

	return *this;
}

//It will use the member function in PrettyWriter to return the string
const char* JsonWriter::GetString() const
{
	return STREAM->GetString();
}

bool JsonWriter::Good() const
{
	return !m_Error;
}

//Informs the Writer that are you are going to write stuff into the object
JsonWriter& JsonWriter::StartObject()
{
	WRITER->StartObject();
	return *this;
}

//Informs the Writer that you are ending the recording of the object
JsonWriter& JsonWriter::EndObject()
{
	WRITER->EndObject();
	return *this;
}

//Adds in the member with the const char * name which is the keyword
JsonWriter& JsonWriter::Member(const char* name)
{
	WRITER->String(name, static_cast<rapidjson::SizeType>(strlen(name)));
	return *this;
}

//Only for JsonReader
bool JsonWriter::HasMember(const char*) const
{
	assert(false);
	return *this;
}

//Start of an array (not sure why need size_t * int there)
JsonWriter& JsonWriter::StartArray()
{
	WRITER->StartArray();
	return *this;
}

//End of an array
JsonWriter& JsonWriter::EndArray()
{
	WRITER->EndArray();
	return *this;
}

//Writes in a bool
JsonWriter& JsonWriter::operator&(bool& b)
{
	WRITER->Bool(b);
	return *this;
}
//Writes in a unsigned
JsonWriter& JsonWriter::operator&(unsigned& u)
{
	WRITER->Uint(u);
	return *this;
}
//Writes in Int
JsonWriter& JsonWriter::operator&(int& i)
{
	WRITER->Int(i);
	return *this;
}
//Writes in double
JsonWriter& JsonWriter::operator&(double& d)
{
	WRITER->Double(d);
	return *this;
}
JsonWriter& JsonWriter::operator&(float& f)
{
	WRITER->Double(f);
	return *this;
}
//Writes in String
JsonWriter& JsonWriter::operator&(std::string& s)
{
	WRITER->String(s.c_str(), static_cast<rapidjson::SizeType>(s.size()));
	return *this;
}
JsonWriter& JsonWriter::operator&(const char* c)
{
	WRITER->String(c);
	return *this;
}
JsonWriter& JsonWriter::SetNull()
{
	WRITER->Null();
	return *this;
}

#undef STREAM
#undef WRITER
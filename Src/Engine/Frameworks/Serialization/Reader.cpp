#include "Reader.h"


struct JsonReaderStackItem
{
	enum State {
		BeforeStart, // An Object or Array is in the stack but not called by startobject or startarray function
		Started,     // An Object or Array is called StartObject/StartArray
		Closed       // An array is closed after reading all elements, but before EndArray()
	};

	//Declaration of Variables
	const rapidjson::Value* m_value;
	State m_state;
	rapidjson::SizeType m_index;  //This is to hold the array size (typedef for unsigned)

	//Constructor for JsonReaderStackItem
	JsonReaderStackItem(const rapidjson::Value* value, State state) :m_value(value), m_state(state), m_index() {}
};


//Giving a naming for the stack that allows LIFO context
typedef std::stack <JsonReaderStackItem> JsonReaderStack;


//Making DOCUMENT a pointer of rapidjson::Document with reference to m_Document
#define DOCUMENT reinterpret_cast <rapidjson::Document *> (m_Document)
//Making STACK a pointer to the struct JsonReaderStack with reference to m_Stack
#define STACK reinterpret_cast<JsonReaderStack *> (m_Stack)
//Defining TOP as a reference to STL stack::top that returns a reference to the top element
#define TOP (STACK->top())
//Dereferencing of the top element and set it to CURRENT
#define CURRENT (*TOP.m_value)


JsonReader::JsonReader(const JsonReader& copy) :m_Document{ nullptr }, m_Error{ false }, m_Stack{ nullptr }
{
	if (this != &copy)
	{
		m_Document = copy.m_Document;
		m_Error = copy.m_Error;
		m_Stack = copy.m_Stack;
	}
}

//JsonReader (Deep Copy)
JsonReader& JsonReader::operator=(const JsonReader& rhs)
{
	if (this != &rhs)
	{
		m_Document = rhs.m_Document;
		m_Stack = rhs.m_Stack;
		m_Error = rhs.m_Error;
	}

	return *this;
}


JsonReader::JsonReader() : m_Document(nullptr), m_Stack(nullptr), m_Error(false)
{
	m_Document = new rapidjson::Document;
}

JsonReader::JsonReader(const char* json) : m_Document(), m_Stack(), m_Error(false)
{
	DOCUMENT->Parse(json);
	//Checks if the parsing of the object have any errors
	if (DOCUMENT->HasParseError())
	{
		std::cout << DOCUMENT->GetParseError() << std::endl;
		m_Error = true;
	}
	//If it reaches here, it means it is an array to be read
	else
	{
		m_Stack = new JsonReaderStack;
		//Needs some understanding on this
		STACK->push(JsonReaderStackItem(DOCUMENT, JsonReaderStackItem::BeforeStart));

	}
}

JsonReader::~JsonReader()
{
	delete DOCUMENT;
	delete STACK;
}

bool JsonReader::CheckForError(const char* json)
{
	DOCUMENT->Parse(json);
	//Checks if the parsing of the object have any errors
	if (DOCUMENT->HasParseError())
	{
		std::cout << DOCUMENT->GetParseError() << std::endl;
		m_Error = true;
		return false;
	}
	//If it reaches here, it means it is an array to be read
	else
	{
		m_Stack = new JsonReaderStack;
		//Needs some understanding on this
		STACK->push(JsonReaderStackItem(DOCUMENT, JsonReaderStackItem::BeforeStart));
		return true;
	}
}


//Archive concept
//Start of an Object
JsonReader& JsonReader::StartObject()
{
	if (!m_Error)
	{
		//RapidJson has a function to check if is an object
		//Check if is an object and if the state of the object is before the start
		if (CURRENT.IsObject() && TOP.m_state == JsonReaderStackItem::BeforeStart)
			//Set it to the next state that is started
			TOP.m_state = JsonReaderStackItem::Started;
		else
			m_Error = true;
	}

	return *this;
}

//End of the Object
JsonReader& JsonReader::EndObject()
{
	if (!m_Error)
	{
		if (CURRENT.IsObject() && TOP.m_state == JsonReaderStackItem::Started)
			Next();
		else
			m_Error = true;
	}
	return *this;
}

bool JsonReader::Good() const
{
	return !m_Error;
}





/// After calling StartObject(), assign a member with a name
JsonReader& JsonReader::Member(const char* name)
{
	if (!m_Error)
	{
		if (CURRENT.IsObject() && TOP.m_state == JsonReaderStackItem::Started)
		{
			//Using rapidjson member functions
			rapidjson::Value::ConstMemberIterator memberIter = CURRENT.FindMember(name);
			if (memberIter != CURRENT.MemberEnd())
				STACK->push(JsonReaderStackItem(&memberIter->value, JsonReaderStackItem::BeforeStart));
			else
				m_Error = true;
		}
		else
			m_Error = true;
	}
	return *this;
}

/// After calling StartObject(), check if a member presents
bool JsonReader::HasMember(const char* name) const
{
	if (!m_Error)
	{
		if (CURRENT.IsObject() && TOP.m_state == JsonReaderStackItem::Started)
			//Rapidjson function
			return CURRENT.HasMember(name);
	}
	return false;
}


JsonReader& JsonReader::IsObject()
{
	if (!m_Error)
	{
		if (CURRENT.IsObject())
		{
			Next();
		}
		else
		{
			m_Error = true;
		}
	}
	return *this;
}


JsonReader& JsonReader::GetName(std::string& temp)
{
	
	if (!m_Error)
	{
		if (CURRENT.IsObject())
		{
			//Using rapidjson member functions
			rapidjson::Value::ConstMemberIterator memberIter = CURRENT.MemberBegin();
			if (memberIter != CURRENT.MemberEnd())
				temp = memberIter->name.GetString();
			else
				m_Error = true;
		}
		else
			m_Error = true;
	}
	return *this;
}

JsonReader& JsonReader::StartArray(size_t* size)
{
	if (!m_Error)
	{
		//Checks if it is an array and the before it starts
		if (CURRENT.IsArray() && TOP.m_state == JsonReaderStackItem::BeforeStart)
		{
			TOP.m_state = JsonReaderStackItem::Started;
			if (size)
				//Assigns the size from the JsonReaderStackItem constructor into a pointer for size
				*size = CURRENT.Size();

			//If the current stack is not empty
			if (!CURRENT.Empty())
			{
				//Retrieving the value from the array 
				const rapidjson::Value* value = &CURRENT[TOP.m_index];
				STACK->push(JsonReaderStackItem(value, JsonReaderStackItem::BeforeStart));
			}
			else
				TOP.m_state = JsonReaderStackItem::Closed;
		}
		else
			m_Error = true;
	}
	return *this;
}

JsonReader& JsonReader::EndArray()
{
	if (!m_Error)
	{
		if (CURRENT.IsArray() && TOP.m_state == JsonReaderStackItem::Closed)
			Next();
		else
			m_Error = true;
	}
	return *this;
}

JsonReader& JsonReader::operator&(bool& b)
{
	if (!m_Error)
	{
		if (CURRENT.IsBool())
		{
			b = CURRENT.GetBool();
			Next();
		}
		else m_Error = true;
	}
	return *this;
}

JsonReader& JsonReader::operator&(int& i)
{
	if (!m_Error)
	{
		if (CURRENT.IsInt())
		{
			i = CURRENT.GetInt();
			Next();
		}
	}
	return *this;
}

JsonReader& JsonReader::operator&(std::string& string)
{
	if (!m_Error)
	{
		if (CURRENT.IsString())
		{

			const char* temp = nullptr;
			temp = CURRENT.GetString();
			string = temp;
			Next();
		}
		else
			m_Error = true;
	}
	return *this;
}

JsonReader& JsonReader::operator&(double& db)
{
	if (!m_Error)
	{
		if (CURRENT.IsDouble())
		{
			db = CURRENT.GetDouble();
			Next();
		}
		else
			m_Error = true;
	}
	return *this;
}

JsonReader& JsonReader::operator&(unsigned& b)
{
	if (!m_Error)
	{
		if (CURRENT.IsUint())
		{
			b = CURRENT.GetUint();
			Next();
		}
		else
			m_Error = true;
	}
	return *this;
}

JsonReader& JsonReader::operator&(float& f)
{
	if (!m_Error)
	{
		if (CURRENT.IsFloat())
		{
			f = CURRENT.GetFloat();
			Next();
		}
		else
			m_Error = true;
	}
	return *this;
}

JsonReader& JsonReader::SetNull()
{
	//This function will be for JsonWriter only.
	m_Error = true;
	return *this;
}

//this function will move the next member of an object or next element of an array
void JsonReader::Next()
{
	if (!m_Error)
	{
		//This is an exeception throw

		(!STACK->empty());
		STACK->pop();


		if (!STACK->empty() && CURRENT.IsArray())
		{
			if (TOP.m_state == JsonReaderStackItem::Started)
			{
				if (TOP.m_index < CURRENT.Size() - 1)
				{
					const rapidjson::Value* value = &CURRENT[++TOP.m_index];
					STACK->push(JsonReaderStackItem(value, JsonReaderStackItem::BeforeStart));
				}
				else
					TOP.m_state = JsonReaderStackItem::Closed;
			}
			else
				m_Error = true;
		}
	}
}

#undef DOCUMENT
#undef STACK
#undef TOP
#undef CURRENT
#pragma once
//#include "Src/pch.h"

#define NULLENTITY EntityID(UINT32_MAX, UINT32_MAX)

struct EntityID
{
	uint32_t _id;
	uint32_t _generations;

	//Default Constructor
	EntityID() : _id(0), _generations(0) {}

	//Constructor
	EntityID(uint32_t id, uint32_t gens) : _id(id), _generations(gens) {}

	//Copy Constructor
	EntityID(const EntityID& ent)
	{
		_id = ent._id;
		_generations = ent._generations;
	}

	//Assignment Constructor
	void operator=(const EntityID& ent)
	{
		_id = ent._id;
		_generations = ent._generations;
	}

	//bool operator()(const EntityID& x, const EntityID& y) const
	//{
	//	return ((x._id < y._id) || (x._generations < y._generations));
	//}

	bool const operator==(const EntityID& ent) const
	{
		return (_id == ent._id && _generations == ent._generations);
	}
	bool const operator<(const EntityID& ent) const
	{
		return (_id < ent._id || _generations < ent._generations);
	}

};

struct CompareEntityID 
{
	bool operator()(const EntityID& x, const EntityID& y) const
	{
		return ((x._id < y._id) || (x._generations < y._generations));
	}
};

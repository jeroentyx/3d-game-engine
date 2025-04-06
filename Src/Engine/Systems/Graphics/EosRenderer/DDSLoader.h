#pragma once

#include "DDSLoaderpch.h"
#include "Src\pch.h"

using namespace tinyddsloader;


//
//	A compiler to reduce the memory consumption of 1.4GB
//

namespace compiler
{
	class DDSLoader
	{
	public:

		//
		//	internal format is how we want Opengl to store
		//	type and foramt is what our data looks like
		static void Load();

		std::vector<DDSFile> ListOfDDS;
	};
}
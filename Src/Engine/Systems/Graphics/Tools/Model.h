#pragma

#include <string>
#include "Src\pch.h"

namespace Eos
{
	class Model
	{
	public:
		Model() = default;
		Model(const std::string& filePath);
		Model();

		~Model() = default;

	};
}
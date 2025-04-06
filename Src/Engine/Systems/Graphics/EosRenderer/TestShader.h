#pragma once

#if !defined(_CRT_SECURE_NO_WARNINGS)
#	define _CRT_SECURE_NO_WARNINGS 1
#endif // _CRT_SECURE_NO_WARNINGS

#include "Src\pch.h"
#include <sstream>
#include <fstream>

namespace Test
{
	class Shader
	{
	public:
		uint32_t id;
		Shader();
		Shader(const std::string& vertexp,
			const std::string& fragp);

		void Init(std::string const& vert, std::string const& frag);
		void Bind();

		void unBind();

		void SetInt(const std::string& name, int value);

		void SetVec3(const std::string& name, const glm::vec3& value);
		void SetBool(const std::string& name, bool value);

		void SetMat4(const std::string& name, const glm::mat4& value);

		void SetFloat(const std::string& name, float value);

	};
}
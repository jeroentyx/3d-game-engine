#pragma once

#include <malloc.h>
#include <string.h>
#include <string>

#include <sstream>
#include <fstream>
#include <glew.h>
#include <iostream>
#include <glm\gtc\type_ptr.hpp>
#include <assert.h>
#include <unordered_map>
#include <array>

namespace Eos
{
	class Shader //shader file for both vert, frag
	{

	public:
		uint32_t id;
		std::string name;

		Shader();
		~Shader();

		Shader(const std::string& filepath);

		Shader(const std::string& name,
			const std::string& vertexSrc,
			const std::string fragmentSrc);

		Shader(const char* const source,
			GLenum type);

		void Init(const std::string& vertexp, const std::string& fragp);

		static std::shared_ptr<Shader>											Create(
			const std::string& filepath);

		static std::shared_ptr<Shader>											Create(
			const std::string& name,
			const std::string& vertexSrc,
			const std::string& fragmentSrc);

		void																	Bind();
		void																	unBind();


		void										 SetInt(const std::string& name, int value);
		void										 SetIntArray(const std::string& name, int* values, GLuint count);
		void										 SetVec2(const std::string& name, const glm::vec2& value);
		void										 SetVec3(const std::string& name, const glm::vec3& value);
		void										 SetVec4(const std::string& name, const glm::vec4& value);
		void										 SetBool(const std::string& name, bool value);
		void										 SetMat4(const std::string& name, const glm::mat4& value, unsigned int count = 1);
		void										 SetFloat(const std::string& name, float value);

	private:
		void										Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		std::string									ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string>		PreProcess(const std::string& source);
	};

}
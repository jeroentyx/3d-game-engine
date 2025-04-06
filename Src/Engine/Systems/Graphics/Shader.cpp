#include "Shader.h"
#include "Src\pch.h"

namespace Eos
{

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment") return GL_FRAGMENT_SHADER;
		return 0;
	}

	Shader::Shader() :
		id(0) {};

	Shader::~Shader()
	{
		glDeleteProgram(id);
	}

	Shader::Shader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		//filepath example: ../shaders/basic.glsl
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		name = filepath.substr(lastSlash, count);
	}

	Shader::Shader(const std::string& name, const std::string& vertexSrc, const std::string fragmentSrc)
	{
		UNREFERENCED_PARAMETER(name);
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER_ARB] = fragmentSrc;
		Compile(sources);
	}

	std::string Shader::ReadFile(const std::string& filepath)
	{
		std::string vertexCode;
		std::ifstream vShaderFile(filepath);
		vShaderFile.exceptions(std::ifstream::badbit);

		try
		{
			vShaderFile.open(filepath);
			
			//spdlog::critical("File opening failed!");

			if (!vShaderFile.is_open())
			{
				PE_CORE_CRITICAL("File opening failed!");
			}

			std::stringstream vShaderStream;
			vShaderStream << vShaderFile.rdbuf();

			vShaderFile.close();

			vertexCode = vShaderStream.str();
		}
		catch (...)
		{
			std::cout << "unknown exception caught" << std::endl;
		}
		return vertexCode;
	}

	std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		//PE_CORE_CRITICAL("unknown exception caught");
		//spdlog::critical("unknown exception caught");
		//std::cout << "unknown exception caught" << std::endl;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			// If above line fails, then its a syntax error

			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			/*shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));*/
			shaderSources[ShaderTypeFromString(type)] =
				(pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				// Use the infoLog as you see fit.

				// In this simple program, we'll just leave
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}


		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto _id : glShaderIDs)
				glDeleteShader(_id);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		for (auto _id : glShaderIDs)
			glDetachShader(program, _id);

		id = program;
	}

	void Shader::Init(const std::string& vertexp,
		const std::string& fragp)
	{
		std::string vertexCode, fragmentCode;
		std::ifstream vShaderFile(vertexp);
		std::ifstream fShaderFile(fragp);
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);

		try
		{
			vShaderFile.open(vertexp);
			fShaderFile.open(fragp);

			if (!vShaderFile.is_open() && !fShaderFile.is_open())
			{
				std::cout << "File opening failed !" << std::endl;
			}

			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();


			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "file not read" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		uint32_t vertex, fragment;
		GLint success = 0;
		GLchar* infoLog = (GLchar*)malloc(sizeof(GLchar) * 512);

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);		// check compile success
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			std::cout << "vertex compile failed" << infoLog << std::endl;
		}
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);	// check compile success
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
			std::cout << "fragment compile failed" << infoLog << std::endl;
			PE_CORE_WARN("File opening failed !");
			//std::cout << "File opening failed !" << std::endl;
		}

		this->id = glCreateProgram();
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 512, nullptr, infoLog);
			std::cout << "program linking failed" << infoLog << std::endl;
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		free(infoLog);
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
	{
		return std::shared_ptr<Shader>(new Shader(filepath));
		//PE_CORE_WARN("file not read");
		//std::cout << "file not read" << std::endl;
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		return std::shared_ptr<Shader>(new Shader(name, vertexSrc, fragmentSrc));
	}


	void Shader::Bind()
	{
		glUseProgram(id);
	}

	void Shader::unBind()
	{
		glUseProgram(0);
	}

	void Shader::SetInt(const std::string& _name, int value)
	{
		glUniform1i(glGetUniformLocation(id, _name.c_str()), value);
	}

	void Shader::SetIntArray(const std::string& _name, int* values, GLuint count)
	{
		GLint location = glGetUniformLocation(id, _name.c_str());
		glUniform1iv(location, count, values);
	}

	void Shader::SetVec2(const std::string& _name, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(id, _name.c_str()), 1, &value[0]);
	}

	void Shader::SetVec3(const std::string& _name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(id, _name.c_str()), 1, &value[0]);
	}

	void Shader::SetVec4(const std::string& _name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(id, _name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void Shader::SetBool(const std::string& _name, bool value)
	{
		glUniform1i(glGetUniformLocation(id, _name.c_str()), (int)value);
	}

	void Shader::SetMat4(const std::string& _name, const glm::mat4& value, unsigned int count)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, _name.c_str()), count, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::SetFloat(const std::string& _name, float value)
	{
		glUniform1f(glGetUniformLocation(id, _name.c_str()), value);
	}
}
#include "TestShader.h"

namespace Test
{
	//Hardcoded to test to see if it can work
	Shader::Shader() :
		id(0)
	{

	}

	Shader::Shader(const std::string& vertexp, const std::string& fragp) :
		id(0)
	{

		std::string vertexCode, fragmentCode;
		//std::ifstream vShaderFile(std::string("./shaders/" + vertexp));
		//std::ifstream fShaderFile(std::string("./shaders/" + fragp));

		std::ifstream vShaderFile(std::string("./shaders/mesh.vert"));
		std::ifstream fShaderFile(std::string("./shaders/mesh.frag"));
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

	void Shader::Init(std::string const& vert,std::string const& frag)
	{
		std::string vertexCode, fragmentCode;

		std::ifstream vShaderFile(std::string("./shaders/" + vert));
		std::ifstream fShaderFile(std::string("./shaders/" + frag));
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);

		try
		{
			vShaderFile.open(vert);
			fShaderFile.open(frag);

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
	void Shader::Bind()
	{
		glUseProgram(id);
	}
	void Shader::unBind()
	{
		glUseProgram(0);
	}
	void Shader::SetInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}
	void Shader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);

	}
	void Shader::SetBool(const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);

	}
	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
	void Shader::SetFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}

}
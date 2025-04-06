#include "TestSkyBox.h"

namespace Test
{
	void GfxSkyBox::Init(const std::string& vert, const std::string frag)
	{
		Test::Shader skyboxShader(vert, frag);
		id = skyboxShader.id;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		texture = loadCubemap(faces);;
		glUseProgram(id);
		SetInt("skybox", 0);
	}

	GfxSkyBox::~GfxSkyBox()
	{
		glDeleteProgram(id);
	}

	void GfxSkyBox::Draw(glm::mat4 view, glm::mat4 proj)
	{
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		glUseProgram(id);

		SetMat4("view", view);
		SetMat4("projection", proj);
		// skybox cube
		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
		glUseProgram(0);
	}

	void GfxSkyBox::SetInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}

	void GfxSkyBox::SetMat4(const std::string& name, glm::mat4 value)
	{
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
}
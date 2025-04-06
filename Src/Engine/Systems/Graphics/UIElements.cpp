#include "UIElements.h"

using std::cerr;
using std::endl;

OpenGuiTexture::OpenGuiTexture()
{
	CreateVAOLayout();
}

OpenGuiTexture::~OpenGuiTexture()
{
	Cleanup();
}

void OpenGuiTexture::CreateVAOLayout()
{
	//load shader
	ShaderProgram.Init("shaders/texture.vs", "shaders/texture.fs");

	if (ShaderProgram.id == 0) {
		cerr << "Error creating a texture" << endl;
	}

	//taken from openGL
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	//set up vao
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Loading and create a texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	std::string assetsPath = "Assets/Textures/";
	unsigned char* data = stbi_load("Assets/Textures/default.png", &width, &height, &nrChannels, 0);

	if (data)
	{

		//Load image data to the textureID? then free it because binaries loaded?
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cerr << "failed to load texture " << std::endl;
	}

	stbi_image_free(data);

}

void OpenGuiTexture::UpdateTransform(const glm::mat4& LocalToWorld )
{
	ShaderProgram.Bind();
	unsigned int transform_loc = glGetUniformLocation(ShaderProgram.id, "LocalToWorld");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(LocalToWorld));
}

void OpenGuiTexture::UpdateOpacity(const float& opacity)
{
	ShaderProgram.Bind();
	unsigned int opacity_loc = glGetUniformLocation(ShaderProgram.id, "Opacity");
	glUniform1f(opacity_loc, opacity);
}

void OpenGuiTexture::UpdateDepth(const float d_value)
{
	ShaderProgram.Bind();
	unsigned int depth_value = glGetUniformLocation(ShaderProgram.id, "Depth");
	glUniform1f(depth_value, d_value);
}

void OpenGuiTexture::UpdateFilePath(const std::string& filePath)
{
	//only do the function if the file path has been changed
	if (CheckForDirtyPath(filePath))
	{
		m_CurrFilePath = filePath; // just assign new path
		CreateTexture2D(); //Create the new texture handler with the new path
	}
}

void OpenGuiTexture::CreateTexture2D()
{
	//Loading and create a texture
	GLuint newTextureID;
	glGenTextures(1, &newTextureID);
	glBindTexture(GL_TEXTURE_2D, newTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	std::string assetsPath = "Assets/Textures/";
	assetsPath.append(m_CurrFilePath);
	unsigned char* data = stbi_load(assetsPath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{

		//Load image data to the textureID? then free it because binaries loaded?
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cerr << "failed to load texture " << std::endl;
	}

	stbi_image_free(data);
	textureID = newTextureID; // assign new TextureID
}

bool OpenGuiTexture::CheckForDirtyPath(const std::string& newPath)
{
	//Check if it is not equal, if not equal return true;
	if (m_CurrFilePath.compare(newPath) != 0)
	{
		return true;
	}
	return false;
}

void OpenGuiTexture::UpdateColor(const glm::vec3& color)
{
	//Pass the in_color to shader side
	glUniform3fv(glGetUniformLocation(ShaderProgram.id, "in_Color"), 1, &color[0]);
}

void OpenGuiTexture::UpdateGamma(const bool _gamma)
{
	ShaderProgram.Bind();
	unsigned int depth_value = glGetUniformLocation(ShaderProgram.id, "gamma");
	glUniform1i(depth_value, _gamma);
}

void OpenGuiTexture::Render()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	ShaderProgram.Bind();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void OpenGuiTexture::Cleanup()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

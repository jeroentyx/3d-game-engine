/*******************************************************************************
filename    FontControl.h
author(s)   Teoh Yan-jie Edwin, t.yanjieedwin@digipen.edu, 100% Code Contribution

date        September 23, 2020
brief

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#pragma once

#include "../Shader.h"

#include <ft2build.h>
#include <map>
#include <string>

#include <gl/glew.h>

#include FT_FREETYPE_H


class Font
{
	struct Character
	{
		unsigned int TextureID = 0;
		glm::ivec2 Size = glm::ivec2{};
		glm::ivec2 Bearing = glm::ivec2{};
		unsigned int Advance = 0;

		glm::vec2 startUV = glm::vec2(0, 0);;
		glm::vec2 endUV = glm::vec2(0, 0);
	};

	struct FontVertex
	{
		glm::vec2 pos;
		glm::vec2 uv;

		FontVertex(float x, float y, float u, float v)
		{
			pos = glm::vec2(x, y);
			uv = glm::vec2(u, v);
		}
	};

	std::map<char, Character> Characters;

	FT_Library _library;
	FT_Face _face;			// Font loaded as face
	glm::mat4 _projection;

	//glm::vec2 _renderPos;
	//glm::vec2 _drawingPos{ 20.0f, Window::height - 20.0f };
	//glm::vec2 _drawingPos{ 0.0f, 0.0f };
	float _interval;
	float _scale;
	glm::vec3 _color;


	std::shared_ptr<Eos::Shader> m_shader;

	GLuint texture;
	unsigned int _vao, _vbo;
public:
	// Create font with its shader from the shader container
	Font();
	~Font();

	// Init from a .ttf font file
	void InitStd(const char* filepath, const glm::mat4& viewprojMatrix);

	void InitAtlas(const char* filepath, const glm::mat4& viewprojMatrix);

	// Render a text on the screen
	void RenderWithAtlas(const std::string text, const glm::vec2 pos, float scale);
	void RenderWithStd(const std::string text, const glm::vec2 renderPos, float scale = 40.0f);

	// Set colour of the text; Default is black
	void SetColor(glm::vec3 color);

	void SetProjection(const glm::mat4& proj);

	// Free allocated memory for the font face and library
	void Exit();
};



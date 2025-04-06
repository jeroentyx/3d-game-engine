/*******************************************************************************
filename    FontControl.cpp
author(s)   Teoh Yan-jie Edwin, t.yanjieedwin@digipen.edu, 100% Code Contribution

date        September 23, 2020
brief

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "Font.h"
#include "Src/pch.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>



Font::Font() :
	_library{ 0 },
	_face{ 0 },
	_projection{ glm::mat4(1.0f) },
	_interval{ 20.0f },
	_scale{ 0.5f },
	_color{ 1.0f,1.0f,1.0f },
	_vao{ 0 },
	_vbo{ 0 },
	texture{ 0 }
{

}

Font::~Font()
{

}

// Change to take in a const char*
void Font::InitStd(const char* filepath, const glm::mat4& viewprojMatrix)
{
	// Init Freetype lib
	FT_Error error = FT_Init_FreeType(&_library);
	assert(("Error : Initialization of freetype failed", error == 0));

	// Load font into the face
	//"../fonts/07558_CenturyGothic.ttf"
	error = FT_New_Face(_library, filepath, 0, &_face);
	assert(("Error: failed to load font file", error == 0));

	m_shader = Eos::Shader::Create("shaders/freetype.glsl");

	// Define pixel font size to extract from the face; width and height
	// Width to 0 allows for face to use height to calculate width
	FT_Set_Pixel_Sizes(_face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; ++c)
	{
		if (FT_Load_Char(_face, c, FT_LOAD_RENDER))
		{
			PE_CORE_WARN("Error: Freetype failed to load glyph");
			//std::cout << "Error: Freetype failed to load glyph" << std::endl;
			continue;
		}

		unsigned int text;
		glGenTextures(1, &text);
		glBindTexture(GL_TEXTURE_2D, text);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _face->glyph->bitmap.width,
			_face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
			_face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			text,
			glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
			glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
			static_cast<unsigned int>(_face->glyph->advance.x)
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}

	// (0,0) at the bottom-left corner
	_projection = viewprojMatrix;

	// Generate buffers for VAO and VBO
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	// Bind VAO and VBO for the fonts
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Font::InitAtlas(const char* filepath, const glm::mat4& viewprojMatrix)
{
	// Init Freetype lib
	FT_Error error = FT_Init_FreeType(&_library);
	assert(("Error : Initialization of freetype failed", error == 0));

	// Load font into the face
	//"../fonts/07558_CenturyGothic.ttf"
	error = FT_New_Face(_library, filepath, 0, &_face);
	assert(("Error: failed to load font file", error == 0));

	m_shader = Eos::Shader::Create("shaders/freetype.glsl");

	// Define pixel font size to extract from the face; width and height
	// Width to 0 allows for face to use height to calculate width
	FT_Set_Pixel_Sizes(_face, 0, 48);
	
	// First loop: Calculate textureWidth and textureHeight
	GLuint textureWidth = 0, textureHeight = 0;
	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(_face, c, FT_LOAD_RENDER))
		{
			PE_CORE_WARN("Error: Freetype failed to load glyph");
			//std::cout << "Error: Freetype failed to load glyph" << std::endl;
			continue;
		}

		// in this sample we are building one row texture
		textureWidth += _face->glyph->bitmap.width;
		textureHeight = std::max(textureHeight, _face->glyph->bitmap.rows);

		// Store character for later use
		Character character;
		character.TextureID = texture;
		character.Size = glm::ivec2{ _face->glyph->bitmap.width, _face->glyph->bitmap.rows };
		character.Bearing = glm::ivec2{ _face->glyph->bitmap_left, _face->glyph->bitmap_top };
		character.Advance = _face->glyph->advance.x;

		Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	// Building one texture for all glyhs
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		textureWidth,
		textureHeight,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		NULL
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Second loop to build one atlas texture map for the whole font
	int UJumps = 0, VJumps = 0;
	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(_face, c, FT_LOAD_RENDER))
		{
			PE_CORE_CRITICAL("Cannot load Glyph");
			std::cout << "Cannot load Glyph" << std::endl;
			continue;
		}

		// startUV
		float uS = (float)(UJumps) / (float)(textureWidth);
		float vS = (float)(VJumps) / (float)(textureHeight);

		//endUV
		float uE = (float)(UJumps + (int)_face->glyph->bitmap.width);
		uE = uE / (float)(textureWidth);
		float vE = (float)(VJumps + _face->glyph->bitmap.rows);
		vE = vE / (float)(textureHeight);

		// CharacterGlyph structure has now 2 additional members: vec2 startUV, vec2 endUV
		Characters[c].startUV = glm::vec2(uS, vS);
		Characters[c].endUV = glm::vec2(uE, vE);

		if (_face->glyph->bitmap.width != 0)
		{
			glTexSubImage2D(
				GL_TEXTURE_2D,
				0,
				UJumps,
				VJumps,
				_face->glyph->bitmap.width,
				_face->glyph->bitmap.rows,
				GL_RED,
				GL_UNSIGNED_BYTE,
				_face->glyph->bitmap.buffer
			);
		}

		// Offset along u
		UJumps += _face->glyph->bitmap.width;
	}

	_projection = viewprojMatrix;

	glBindTexture(GL_TEXTURE_2D, 0);

	// Generate buffers for VAO and VBO
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
}

void Font::RenderWithAtlas(const std::string text, const glm::vec2 pos, float scale)
{
	scale /= 40;
	_scale = scale;
	m_shader->Bind();

	m_shader->SetVec3("textColor", _color);
	m_shader->SetMat4("u_ViewProjection", _projection);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(_vao);

	glm::vec2 _drawingPos{ pos };
	std::vector<FontVertex> vArray;
	for (std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = _drawingPos.x + ch.Bearing.x * scale;
		float ypos = _drawingPos.y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		// update VBO for each character
		vArray.push_back(FontVertex(xpos, ypos + h,		ch.startUV.x, ch.startUV.y));
		vArray.push_back(FontVertex(xpos, ypos,			ch.startUV.x, ch.endUV.y));
		vArray.push_back(FontVertex(xpos + w, ypos,		ch.endUV.x, ch.endUV.y));
		vArray.push_back(FontVertex(xpos, ypos + h,		ch.startUV.x, ch.startUV.y));
		vArray.push_back(FontVertex(xpos + w, ypos,		ch.endUV.x, ch.endUV.y));
		vArray.push_back(FontVertex(xpos + w, ypos + h, ch.endUV.x, ch.startUV.y));
		
		_drawingPos.x += (ch.Advance >> 6) * scale; 
	}

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(FontVertex) * vArray.size(), vArray.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FontVertex), 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vArray.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_shader->unBind();
}


void Font::RenderWithStd(const std::string text, const glm::vec2 renderPos, float scale)
{
	scale /= 100;
	_scale = scale;

	m_shader->Bind();

	/*glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	//glUniform3f(glGetUniformLocation(_shdrRef.GetHandle(), "textColor"), _color.x, _color.y, _color.z);
	//glUniformMatrix4fv(glGetUniformLocation(_shdrRef.GetHandle(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	m_shader->SetVec3("textColor", _color);
	m_shader->SetMat4("u_ViewProjection", _projection);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(_vao);

	// iterate through all characters
	std::string::const_iterator c;
	glm::vec2 _drawingPos{ renderPos };

	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = _drawingPos.x + ch.Bearing.x * scale;
		float ypos = _drawingPos.y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		// VBO 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// render
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// advance cursors for next glyph (note that advance is number of 1/64 pixels)
		_drawingPos.x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	m_shader->unBind();
}

void Font::SetColor(glm::vec3 color)
{
	_color = color;
}

void Font::SetProjection(const glm::mat4& proj)
{
	_projection = proj;
}

void Font::Exit()
{
	Characters.clear();
	FT_Done_Face(_face);
	FT_Done_FreeType(_library);
}

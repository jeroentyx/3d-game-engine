#pragma once

#include "Src\Engine\Core\Interfaces\IComponent.h"

#include "Src\Engine\Systems\Graphics\Compiler\FontCompiler.h"

class VertexArray;

class Font_Component : public IComponent
{
	// Private variables

	// Private helper functions
	void LoadFontFile(const std::string& filepath);

public:
	Font_Component();

	virtual void Init() override;

	int testVariable;
	std::shared_ptr<VertexArray> _vao;

	std::shared_ptr<Eos::Shader> fontshader;
	FontCompiler compiler;

	// From compiler
	// TODO: remove and let asset manager manage these data
	FontCompiler::CodePoints codepoints;
	FontCompiler::Advances advances;
	FontCompiler::UVs uvs;
	FontCompiler::Quads quads;
	FontCompiler::Verticals verticals;
	FontCompiler::Kernings kernings;

	void GetFontCharUVBounds(uint32_t unicodeChar, float& out_l, float& out_r, float& out_b, float& out_t);
	void GetFontCharQuadBounds(uint32_t unicodeChar, float& out_l, float& out_r, float& out_b, float& out_t, uint32_t prevChar = 0);
	double GetFontCharAdvance(uint32_t unicodeChar);

	property_vtable()
};

property_begin(Font_Component)
{
	property_var(testVariable)
}property_vend_h(Font_Component)
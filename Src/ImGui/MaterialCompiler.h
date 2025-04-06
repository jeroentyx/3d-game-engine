#pragma once

#include "..\pch.h"
#include "IImGuiWindow.h"

namespace Eos
{
	class MaterialCompiler : public IImGuiWindow
	{
	public:
		MaterialCompiler();
		~MaterialCompiler() = default;

		// Stored values
		bool blendable, backfaceCulling, frontFaceCulling, DepthTest;
		int materialFlag = false, shaderFlag = false;

		float diffuseMeasurement[4] = { 0 };
		float colorDiffuse[3] = { 0 };

		float specularXYZ[3] = { 0 };
		float specularRGB[3] = { 0 };

		float textOffset[2] = { 0 };

		float textureTiling[2] = { 0 };

		bool enableNormal = false;

		bool enableXYZWDiffuse = true;
		bool enableRGBDiffuse = false;
		bool enableXYZSpecular = true;
		bool enableRGBSpecular = false;

		virtual void Update() override;
	};
}

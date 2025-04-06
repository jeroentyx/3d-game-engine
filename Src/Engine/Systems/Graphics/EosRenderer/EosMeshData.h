#pragma once

#include <vector>
#include "Src\MeshCompiler\VertexData.h"

class EosMeshData
{
public:
	EosMeshData(const char* meshFile);
	MeshBlock block_;
	MeshData meshData_;
	std::vector<RenderData> primitives_;
};
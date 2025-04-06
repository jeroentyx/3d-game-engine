#pragma once

#include <stdint.h>

#include "glm\glm.hpp"
#include "MeshUtilities.h"
#include <vector>

#include <stdint.h>
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "assimp\cimport.h"


constexpr const uint32_t maxLods = 8;
constexpr const uint32_t maxStreams = 8;

/*
* Helper functions needed for Mesh Compiler
*/
struct Mesh final
{
	uint32_t														lodCount = 1;
	uint32_t														streamCount = 0;
	uint32_t														indexOffset = 0;
	uint32_t														vertexOffset = 0;
	uint32_t														vertexCount = 0;
	uint32_t														lodOffset[maxLods];

	inline uint32_t getLODIndicesCount(uint32_t lod) const { return lodOffset[lod + 1] - lodOffset[lod]; }

	uint32_t streamOffset[maxStreams] = { 0 };
	uint32_t streamElementSize[maxStreams] = { 0 };

};

struct MeshBlock
{
	uint32_t														ParityBit;
	uint32_t														meshCount;
	uint32_t														dataBlockStartOffset;
	uint32_t														indexDataSize;
	uint32_t														vertexDataSize;
};

struct RenderData
{
	uint32_t														meshIndex;
	uint32_t														materialIndex;
	uint32_t														LOD;
	uint32_t														indexOffset;
	uint32_t														vertexOffset;
	uint32_t														transformIndex;
};

struct MeshData
{
	std::vector<uint32_t>											m_indexData;
	std::vector<float>												m_vertexData;
	std::vector<Mesh>												m_meshes;
};


MeshBlock							LoadMeshData(const char* file, MeshData& out);

void								SaveMeshData(const char* file, const MeshData& m);

void								RecalculateBoundingBoxes(MeshData& m);

MeshBlock							MergeMeshData(MeshData& m, const std::vector<MeshData*> md);




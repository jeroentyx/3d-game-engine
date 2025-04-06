#include <iostream>
#include "MeshHelpers.h"
#include "VertexData.h"

#include <stdint.h>
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "assimp\cimport.h"

#include "meshoptimizer.h"

MeshData g_meshData;

uint32_t g_indexOffset = 0;
uint32_t g_vertexOffset = 0;

constexpr uint32_t g_numElementsToStore = 3 + 3 + 2;

float g_meshScale = 0.01f;
bool g_calculateLODs = false;


void processLods(std::vector<uint32_t>& indices, std::vector<float>& vertices,
	std::vector<std::vector<uint32_t>>& outlods)
{
	size_t verticesCountIn = vertices.size() / 2;
	size_t targetIndicesCount = indices.size();
	uint8_t LOD = 1;

	std::cout << "LOD : " << indices.size() << "indices" << std::endl;

	outlods.push_back(indices);

	while (targetIndicesCount > 1024 && LOD < 8)
	{
		targetIndicesCount = indices.size() / 2;

		bool sloppy = false;

		size_t numOptIndices = meshopt_simplify(
			indices.data(),
			indices.data(), (uint32_t)indices.size(),
			vertices.data(), verticesCountIn,
			sizeof(float) * 3,
			targetIndicesCount, 0.02f);

		if (static_cast<size_t>(numOptIndices * 1.1f) > indices.size())
		{
			if (LOD > 1)
			{
				numOptIndices = meshopt_simplifySloppy(
					indices.data(),
					indices.data(), indices.size(),
					vertices.data(), verticesCountIn,
					sizeof(float) * 3,
					targetIndicesCount, 0.02f);
				sloppy = true;
				if (numOptIndices == indices.size())break;
			}
			else
			{
				break;
			}
		}
		indices.resize(numOptIndices);

		meshopt_optimizeVertexCache(indices.data(), indices.data(), indices.size(), verticesCountIn);

		std::cout << "LOD" << static_cast<int>(LOD) << " indices" << (int)(numOptIndices) << std::endl;

		LOD++;

		outlods.push_back(indices);
	
	}
}

Mesh convertAIMesh(const aiMesh* m)
{
	const bool hasTexCoords = m->HasTextureCoords(0);
	const uint32_t streamElementSize = static_cast<uint32_t>(g_numElementsToStore * sizeof(float));

	// Original data for LOD calculation
	std::vector<float> srcVertices;
	std::vector<uint32_t> srcIndices;

	std::vector<std::vector<uint32_t>> outLods;

	auto& vertices = g_meshData.m_vertexData;

	for (size_t i = 0; i != m->mNumVertices; i++)
	{
		const aiVector3D v = m->mVertices[i];
		const aiVector3D n = m->mNormals[i];
		const aiVector3D t = hasTexCoords ? m->mTextureCoords[0][i] : aiVector3D();

		if (g_calculateLODs)
		{
			srcVertices.push_back(v.x);
			srcVertices.push_back(v.y);
			srcVertices.push_back(v.z);
		}

		vertices.push_back(v.x * g_meshScale);
		vertices.push_back(v.y * g_meshScale);
		vertices.push_back(v.z * g_meshScale);

		vertices.push_back(t.x);
		vertices.push_back(1.0f - t.y);

		vertices.push_back(n.x);
		vertices.push_back(n.y);
		vertices.push_back(n.z);
	}

	Mesh result = {
		.streamCount = 1,
		.indexOffset = g_indexOffset,
		.vertexOffset = g_vertexOffset,
		.vertexCount = m->mNumVertices,
		.streamOffset = { g_vertexOffset * streamElementSize },
		.streamElementSize = { streamElementSize }
	};

	for (size_t i = 0; i != m->mNumFaces; i++)
	{
		if (m->mFaces[i].mNumIndices != 3)
			continue;
		for (unsigned j = 0; j != m->mFaces[i].mNumIndices; j++)
			srcIndices.push_back(m->mFaces[i].mIndices[j]);
	}

	if (!g_calculateLODs)
		outLods.push_back(srcIndices);
	else
		processLods(srcIndices, srcVertices, outLods);

	printf("Calculated LOD count: %u \n", (unsigned)outLods.size());

	uint32_t numIndices = 0;
	for (size_t l = 0; l < outLods.size(); l++)
	{
		for (size_t i = 0; i < outLods[l].size(); i++)
			g_meshData.m_indexData.push_back(outLods[l][i]);

		result.lodOffset[l] = numIndices;
		numIndices += (int)outLods[l].size();
	}

	result.lodOffset[outLods.size()] = numIndices;
	result.lodCount = (uint32_t)outLods.size();

	g_indexOffset += numIndices;
	g_vertexOffset += m->mNumVertices;

	return result;

}

void LoadFile(const char* fileName)
{
	const uint32_t compilerFlags = 0 |
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_LimitBoneWeights |
		aiProcess_SplitLargeMeshes |
		aiProcess_ImproveCacheLocality |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_FindDegenerates |
		aiProcess_FindInvalidData |
		aiProcess_GenUVCoords;

	const aiScene* scene = aiImportFile(fileName, compilerFlags);

	if (!scene || !scene->HasMeshes())
	{
		std::cout << "Unable to load " << fileName << std::endl;
		exit(255);
	}

	g_meshData.m_meshes.reserve(scene->mNumMeshes);

	for (unsigned int i = 0; i != scene->mNumMeshes; i++)
	{
		std::cout << "Converting meshes " << i + 1 << " " << scene->mNumMeshes << std::endl;
		fflush(stdout);
		g_meshData.m_meshes.push_back(convertAIMesh(scene->mMeshes[i]));
	}
}

int main()
{
	std::cout << "Eos Mesh Compiler started" << std::endl;
	LoadFile("cube/cube.obj");

	std::vector<RenderData> grid;

	g_vertexOffset = 0;
	for (auto i = 0; i < g_meshData.m_meshes.size(); i++)
	{
		grid.push_back(RenderData{
			.meshIndex = (uint32_t)i,
			.materialIndex = 0,
			.LOD = 0,
			.indexOffset = g_meshData.m_meshes[i].indexOffset,
			.vertexOffset = g_vertexOffset,
			.transformIndex = 0
			});
		g_vertexOffset += g_meshData.m_meshes[i].vertexCount;
	}

	SaveMeshData("cube/cube.eo", g_meshData);

	FILE* f = fopen("cube/cube.eo.drawdata", "wb");
	fwrite(grid.data(), grid.size(), sizeof(RenderData), f);
	fclose(f);
	return 0;
}
#pragma once

#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// Assimp just for loading in for first time
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

class AnimationCompiler
{
public:
	struct Vertex
	{
		glm::vec3 position = glm::vec3{ 0.0f };
		glm::vec3 normal = glm::vec3{ 0.0f };
		glm::vec2 uv = glm::vec2{ 0.0f };
		glm::vec4 boneIDs = glm::vec4(0.0f);
		glm::vec4 boneWeights = glm::vec4(0.0f);
	};

	struct BoneTransformTrack
	{
		std::vector<float> positionTimeStamps;
		std::vector<float> rotationTimeStamps;
		std::vector<float> scaleTimeStamps;

		std::vector<glm::vec3> positions;
		std::vector<glm::quat> rotations;
		std::vector<glm::vec3> scales;
	};

public:
	struct Bone
	{
		int id = 0;
		std::string name = "";
		glm::mat4 offset = glm::mat4(1.0f);
		std::vector<Bone> children;
	};

	struct SkeletonInfo
	{
		Bone skeleton;
		unsigned int boneCount;
		std::unordered_map<std::string, std::pair<int, glm::mat4>> boneInfo;
	};

	struct AnimationTrack
	{
		float duration = 0.0f;
		float ticksPerSecond = 1.0f;
		std::unordered_map<std::string, BoneTransformTrack> boneTransforms;
	};

	struct MeshInfo
	{
		glm::mat4 globalInvTransform;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int vao = 0;
	};

	struct Ex_Info
	{
		glm::mat4 globalInvTransform;	//
		unsigned int boneCount;			//
		Bone skeleton;					//
		AnimationTrack animtionClip;	//
		std::unordered_map<std::string, std::pair<int, glm::mat4>> boneInfo;

		std::vector<Vertex> outputVerts;
		std::vector<unsigned int> outputIdxs;
	};

private:
	static void save_skeleton(aiNode* node, std::ofstream& out);
	static void save_mesh(aiMesh* mesh, std::ofstream& out);
	static void save_animation(aiAnimation* anim, std::ofstream& out);

	static bool read_skeleton(std::ifstream& readfile, Bone& boneOut, AnimationCompiler::SkeletonInfo& info);
	static bool read_mesh(std::ifstream& readfile, AnimationCompiler::MeshInfo& info, AnimationCompiler::SkeletonInfo& skeleton);
	static bool read_animation(std::ifstream& readFile, AnimationCompiler::AnimationTrack& info);

public:
	static void readFBX_to_ownFormat(std::string const& path, 
		bool saveMesh = true, bool saveSkeleton = true, bool saveAnimation = true, bool saveScene = false);
	//static Ex_Info readOwnFormat(std::string const& path);
	static bool readOwnFormat(const std::string& meshpath,
		const std::string& skeletonpath,
		const std::string& animationpath);

	// Read a eosmesh file and saves it into 
	static void read_mesh_skeleton_from_file(const std::string& meshfile, const std::string& skeletionfile);
	static void read_animation_from_file(const std::string& file);
};
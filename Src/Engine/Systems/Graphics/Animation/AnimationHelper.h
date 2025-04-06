#pragma once

#include <vector>
#include <unordered_map>


#include <glm/gtc/quaternion.hpp>	//quat_cast
#include <glm/gtx/quaternion.hpp>	//toMat4 toMat3

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Src/Engine/Systems/Graphics/Shader.h"
#include "src/Engine/Systems/Graphics/Compiler/AnimationCompiler.h"


struct Vertex
{
	glm::vec3 position = glm::vec3{ 0.0f };
	glm::vec3 normal = glm::vec3{ 0.0f };
	glm::vec2 uv = glm::vec2{ 0.0f };
	glm::vec4 boneIDs = glm::vec4(0.0f);
	glm::vec4 boneWeights = glm::vec4(0.0f);
};

// structure to hold bone tree (skeleton)
struct Bone
{
	int id = 0;
	std::string name = "";
	glm::mat4 offset = glm::mat4(1.0f);
	std::vector<Bone> children;
};

// Animation track
struct BoneTransformTrack
{
	std::vector<float> positionTimeStamps;
	std::vector<float> rotationTimeStamps;
	std::vector<float> scaleTimeStamps;

	std::vector<glm::vec3> positions;
	std::vector<glm::quat> rotations;
	std::vector<glm::vec3> scales;
};

struct Animation
{
	float duration = 0.0f;
	float ticksPerSecond = 1.0f;
	std::unordered_map<std::string, BoneTransformTrack> boneTransforms;
};


bool readSkeleton(Bone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable);

void loadModel(const aiScene* scene, aiMesh* mesh, std::vector<Vertex>& verticesOutput, std::vector<unsigned int>& indicesOutput, Bone& skeletonOutput, unsigned int& nBoneCount);
void loadAnimation(const aiScene* scene, Animation& animation);

unsigned int createVertexArray(std::vector<Vertex>& vertices, std::vector<unsigned int> indices);
unsigned int createTexture(std::string filepath);

std::pair<unsigned int, float> getTimeFraction(std::vector<float>& times, float& dt);
void getPose(AnimationCompiler::AnimationTrack& animation,
	AnimationCompiler::Bone& skeletion, float& dt,
	std::vector<glm::mat4>& output,
	glm::mat4& parentTransform,
	glm::mat4& globalInverseTransform);


//struct BodyRig
//{
//	std::string name;
//	std::vector<Vertex> BodyVertices;
//	std::vector<unsigned int> BodyIndices;
//	Bone BodyBone;
//};

class TestAnimation
{
public:
	std::shared_ptr<Eos::Shader> m_shader;

	//glm::mat4 projectionMatrix{ 1.0f };
	//glm::mat4 viewMatrix{ 1.0f };
	//glm::mat4 viewProjectionMatrix{ 1.0f };

	Animation animation;
	Bone skeleton;
	unsigned int boneCount = 0;
	//std::vector<BodyRig> SurgeNoColor;

	std::vector<Vertex> vertices = {};
	std::vector<unsigned int> indices = {};

	glm::mat4 globalInverseTransform{ 1.0f };
	glm::mat4 identity{ 1.0f };
	unsigned int diffuseTexture = 0;

	unsigned int viewProjectionMatrixLocation;
	unsigned int modelMatrixLocation;
	unsigned int boneMatricesLocation;
	unsigned int textureLocation;

	std::vector<glm::mat4> currentPose;
	unsigned int vao = 0;

public:
	TestAnimation();
	~TestAnimation();

	void Init(std::string const& path);
	void End();
};
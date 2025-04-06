#include "src/Engine/Systems/Graphics/Compiler/AnimationCompiler.h"

std::pair<unsigned int, float> getTimeFraction(std::vector<float>& times, float& dt);

void getPose(AnimationCompiler::AnimationTrack& animation,
	AnimationCompiler::Bone& skeletion, float& dt,
	std::vector<glm::mat4>& output,
	glm::mat4& parentTransform,
	glm::mat4& globalInverseTransform);

unsigned int createTexturefromfile(std::string filepath);
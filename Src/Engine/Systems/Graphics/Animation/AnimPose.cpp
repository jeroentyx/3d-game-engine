#include "AnimPose.h"

#include <stb_image/stb_image.h>
#include <Src/pch.h>

std::pair<unsigned int, float> getTimeFraction(std::vector<float>& times, float& dt)
{
	unsigned int segment = 0;
	while (dt > times[segment])
	{
		segment++;
	}
	float start = times[segment - 1];
	float end = times[segment];
	float frac = (dt - start) / (end - start);
	return { segment, frac };
}


void ViewSkeleton(AnimationCompiler::Bone skeleton)
{
	PE_CORE_DEBUG("{}", skeleton.name);
	for (AnimationCompiler::Bone bone : skeleton.children)
	{
		ViewSkeleton(bone);
	}
}

void getPose(AnimationCompiler::AnimationTrack& animation, AnimationCompiler::Bone& skeletion, float& dt, std::vector<glm::mat4>& output, glm::mat4& parentTransform, glm::mat4& globalInverseTransform)
{
	//ViewSkeleton(skeletion);

	// Reset animation if the dt is more than the duration
	if (dt >= (animation.duration))
		dt = 0.1f;

	AnimationCompiler::BoneTransformTrack& btt = animation.boneTransforms[skeletion.name];
	dt = fmod(dt, animation.duration);
	std::pair<unsigned int, float> fp;

	//calculate interpolated position
	fp = getTimeFraction(btt.positionTimeStamps, dt);
	glm::vec3 position1 = btt.positions[fp.first - 1];
	glm::vec3 position2 = btt.positions[fp.first];
	glm::vec3 position = glm::mix(position1, position2, fp.second);

	//calculate interpolated rotation
	fp = getTimeFraction(btt.rotationTimeStamps, dt);
	glm::quat rotation1 = btt.rotations[fp.first - 1];
	glm::quat rotation2 = btt.rotations[fp.first];
	glm::quat rotation = glm::slerp(rotation1, rotation2, fp.second);

	//calculate interpolated scale
	fp = getTimeFraction(btt.scaleTimeStamps, dt);
	glm::vec3 scale1 = btt.scales[fp.first - 1];
	glm::vec3 scale2 = btt.scales[fp.first];
	glm::vec3 scale = glm::mix(scale1, scale2, fp.second);

	glm::mat4 positionMat = glm::mat4(1.0),
		scaleMat = glm::mat4(1.0);

	// calculate localTransform
	positionMat = glm::translate(positionMat, position);
	glm::mat4 rotationMat = glm::toMat4(rotation);
	scaleMat = glm::scale(scaleMat, scale);
	glm::mat4 localTransform = positionMat * rotationMat * scaleMat;


	glm::mat4 globalTransform = parentTransform * localTransform;

	output[skeletion.id] = globalInverseTransform * globalTransform * skeletion.offset;

	//update values for children bones
	for (AnimationCompiler::Bone& child : skeletion.children)
		if (child.name != "")
			getPose(animation, child, dt, output, globalTransform, globalInverseTransform);
}

unsigned int createTexturefromfile(std::string filepath)
{
	unsigned int textureId = 0;
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 4);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, str.c_str());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}
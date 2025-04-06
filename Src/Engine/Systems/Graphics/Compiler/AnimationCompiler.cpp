#include "AnimationCompiler.h"
#include "Src/Engine/Core/Manager/AssetManager.h"

#include <Src/pch.h>

static inline glm::mat4 mat4_cast(const aiMatrix4x4& m) { return glm::transpose(glm::make_mat4(&m.a1)); }

// Function to save mat4 to an external file stream
void save_MAT_4(glm::mat4& matrix, std::ofstream& out)
{
	glm::vec4 t0 = matrix[0];
	glm::vec4 t1 = matrix[1];
	glm::vec4 t2 = matrix[2];
	glm::vec4 t3 = matrix[3];

	out << t0[0] << " " << t0[1] << " " << t0[2] << " " << t0[3] << "\n";
	out << t1[0] << " " << t1[1] << " " << t1[2] << " " << t1[3] << "\n";
	out << t2[0] << " " << t2[1] << " " << t2[2] << " " << t2[3] << "\n";
	out << t3[0] << " " << t3[1] << " " << t3[2] << " " << t3[3] << "\n";
}

// Function to read mat4 from a filestream
glm::mat4 read_MAT_4(std::ifstream& file)
{
	glm::vec4 t0;
	glm::vec4 t1;
	glm::vec4 t2;
	glm::vec4 t3;

	file >> t0.x >> t0.y >> t0.z >> t0.w;
	file >> t1.x >> t1.y >> t1.z >> t1.w;
	file >> t2.x >> t2.y >> t2.z >> t2.w;
	file >> t3.x >> t3.y >> t3.z >> t3.w;

	glm::mat4 offsetMat{ t0, t1, t2, t3 };
	return offsetMat;
}

// Create VAO for animated meshes helper function
unsigned int createVertexArray(std::vector<AnimationCompiler::Vertex>& vertices, std::vector<unsigned int> indices)
{
	unsigned int vao = 0;
	unsigned int vbo = 0;
	unsigned int ebo = 0;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(AnimationCompiler::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimationCompiler::Vertex), (GLvoid*)offsetof(AnimationCompiler::Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AnimationCompiler::Vertex), (GLvoid*)offsetof(AnimationCompiler::Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AnimationCompiler::Vertex), (GLvoid*)offsetof(AnimationCompiler::Vertex, uv));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(AnimationCompiler::Vertex), (GLvoid*)offsetof(AnimationCompiler::Vertex, boneIDs));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(AnimationCompiler::Vertex), (GLvoid*)offsetof(AnimationCompiler::Vertex, boneWeights));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
	return vao;
}

// Function to save the skeleton into a output file stream
void AnimationCompiler::save_skeleton(aiNode* node, std::ofstream& out)
{
	//std::cout << node->mName.C_Str() << " " << node->mNumChildren << std::endl;
	std::string str{ node->mName.C_Str() };
	if (!str.compare("Surge split"))
		out << "Surge" << " " << node->mNumChildren << "\n";
	else
		out << node->mName.C_Str() << " " << node->mNumChildren << "\n";
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		AnimationCompiler::save_skeleton(node->mChildren[i], out);
	}
}

// Function to save a mesh to a output file stream
void AnimationCompiler::save_mesh(aiMesh* mesh, std::ofstream& out)
{
	out << mesh[0].mNumVertices << "\n";
	for (unsigned int i = 0; i < mesh[0].mNumVertices; i++)
	{
		// Save vertices
		out << mesh->mVertices[i].x << " " << mesh->mVertices[i].y << " " << mesh->mVertices[i].z << "\n";

		// Save normals
		out << mesh->mNormals[i].x << " " << mesh->mNormals[i].y << " " << mesh->mNormals[i].z << "\n";

		// Save UV
		out << mesh->mTextureCoords[0][i].x << " " << mesh->mTextureCoords[0][i].y << "\n";

		// Save Bone ID

		// Save Bone weight
	}

	// Loading bones and weights information
	out << mesh->mNumBones << "\n";
	for (unsigned int i = 0; i < mesh->mNumBones; ++i)
	{
		// Access each bone, name of bone and the bone matrix
		aiBone* bone = mesh->mBones[i];
		out << bone->mName.C_Str() << "\n";
		glm::mat4 offsetMatrix = mat4_cast(bone->mOffsetMatrix);
		save_MAT_4(offsetMatrix, out);

		out << bone->mNumWeights << "\n";
		for (unsigned int j = 0; j < bone->mNumWeights; ++j)
		{
			out << bone->mWeights[j].mVertexId << " " << bone->mWeights[j].mWeight << "\n";
		}
	}

	// Loading indices
	out << mesh->mNumFaces << "\n";
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		// For each face
		aiFace& face = mesh->mFaces[i];

		out << face.mNumIndices << "\n";
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			out << face.mIndices[j] << "\n";
	}
}

// Function to save animation into a output file stream
void AnimationCompiler::save_animation(aiAnimation* anim, std::ofstream& out)
{
	out << anim->mTicksPerSecond << "\n";
	out << anim->mDuration << "\n";
	out << anim->mNumChannels << "\n";
	for (unsigned int i = 0; i < anim->mNumChannels; ++i)
	{
		// Save number of keys within each channel for position, rotation and scale
		aiNodeAnim* channel = anim->mChannels[i];

		// Accessing time stamps and values at those time stamps
		out << channel->mNumPositionKeys << "\n";
		for (unsigned int j = 0; j < anim->mChannels[i]->mNumPositionKeys; ++j)
		{
			out << channel->mPositionKeys[j].mTime << " ";
			out << channel->mPositionKeys[j].mValue.x << " "
				<< channel->mPositionKeys[j].mValue.y << " "
				<< channel->mPositionKeys[j].mValue.z << "\n";
		}

		out << channel->mNumRotationKeys << "\n";
		for (unsigned int j = 0; j < anim->mChannels[i]->mNumRotationKeys; ++j)
		{
			out << channel->mRotationKeys[j].mTime << " ";
			out << channel->mRotationKeys[j].mValue.w << " "
				<< channel->mRotationKeys[j].mValue.x << " "
				<< channel->mRotationKeys[j].mValue.y << " "
				<< channel->mRotationKeys[j].mValue.z << "\n";
		}

		out << channel->mNumScalingKeys << "\n";
		for (unsigned int j = 0; j < anim->mChannels[i]->mNumScalingKeys; ++j)
		{
			out << channel->mScalingKeys[j].mTime << " ";
			out << channel->mScalingKeys[j].mValue.x << " "
				<< channel->mScalingKeys[j].mValue.y << " "
				<< channel->mScalingKeys[j].mValue.z << "\n";
		}

		out << channel->mNodeName.C_Str() << std::endl;
	}
}




bool AnimationCompiler::read_skeleton(std::ifstream& file, AnimationCompiler::Bone& boneOutput, AnimationCompiler::SkeletonInfo& info)
{
	std::string nodeName;
	unsigned int numChildren;

	file >> nodeName >> numChildren;
	//std::cout << nodeName << numChildren << std::endl;

	//Loop all the RootNode,Armature,Root Toroso,Head
	if (info.boneInfo.find(nodeName) != info.boneInfo.end())
	{
		boneOutput.name = nodeName;
		boneOutput.id = info.boneInfo[boneOutput.name].first;
		boneOutput.offset = info.boneInfo[boneOutput.name].second;

		for (unsigned int i = 0; i < numChildren; ++i)
		{
			AnimationCompiler::Bone child;
			AnimationCompiler::read_skeleton(file, child, info);
			boneOutput.children.push_back(child);
			
		}
		return true;
	}
	else
	{
		for (unsigned int i = 0; i < numChildren; ++i)
		{
			if (AnimationCompiler::read_skeleton(file, boneOutput, info))
			{
				return true;
			}

		}
	}

	//If it comes here, reading of file failed.
	return false;
}


bool AnimationCompiler::read_mesh(std::ifstream& readfile, AnimationCompiler::MeshInfo& mesh, AnimationCompiler::SkeletonInfo& skeleton)
{
	mesh.globalInvTransform = read_MAT_4(readfile);

	unsigned int numVertices;
	readfile >> numVertices;
	for (unsigned int i = 0; i < numVertices; ++i)
	{
		Vertex vert;

		// Read position, normals, UV and initialize boneID and boneWeights
		readfile >> vert.position.x >> vert.position.y >> vert.position.z;
		readfile >> vert.normal.x >> vert.normal.y >> vert.normal.z;
		readfile >> vert.uv.x >> vert.uv.y;
		vert.boneIDs = glm::ivec4(0);
		vert.boneWeights = glm::vec4(0.0f);

		mesh.vertices.push_back(vert);
	}

	std::vector<unsigned int> boneCounts;
	boneCounts.resize(mesh.vertices.size(), 0);

	// Looping through each bone
	readfile >> skeleton.boneCount;
	for (unsigned int i = 0; i < skeleton.boneCount; ++i)
	{
		std::string boneName;
		glm::mat4 offsetMatrix;
		readfile >> boneName;
		offsetMatrix = read_MAT_4(readfile);
		skeleton.boneInfo[boneName] = { i, offsetMatrix };


		unsigned numWeights;
		readfile >> numWeights;
		for (unsigned int j = 0; j < numWeights; ++j)
		{
			unsigned int id;
			float weight;
			readfile >> id >> weight;
			boneCounts[id]++;

			switch (boneCounts[id])
			{
			case 1:
				mesh.vertices[id].boneIDs.x = static_cast<float>(i);
				mesh.vertices[id].boneWeights.x = weight;
				break;
			case 2:
				mesh.vertices[id].boneIDs.y = static_cast<float>(i);
				mesh.vertices[id].boneWeights.y = weight;
				break;
			case 3:
				mesh.vertices[id].boneIDs.z = static_cast<float>(i);
				mesh.vertices[id].boneWeights.z = weight;
				break;
			case 4:
				mesh.vertices[id].boneIDs.w = static_cast<float>(i);
				mesh.vertices[id].boneWeights.w = weight;
				break;
			default:
				PE_CORE_WARN("Unable to allocate bone to vertex");
				break;
			}
		}
	}

	// Normalize all weights
	for (Vertex vertex : mesh.vertices)
	{
		glm::vec4& boneWeights = vertex.boneWeights;
		float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
		if (totalWeight > 0.0f)
		{
			vertex.boneWeights = glm::vec4(
				boneWeights.x / totalWeight,
				boneWeights.y / totalWeight,
				boneWeights.z / totalWeight,
				boneWeights.w / totalWeight);
		}
	}

	// Loading indices
	unsigned int numFaces;
	readfile >> numFaces;
	for (unsigned int i = 0; i < numFaces; ++i)
	{
		unsigned int numIndices;
		readfile >> numIndices;
		for (unsigned int j = 0; j < numIndices; ++j)
		{
			unsigned int index;
			readfile >> index;
			mesh.indices.push_back(index);
		}
	}

	// Create VAO
	mesh.vao = createVertexArray(mesh.vertices, mesh.indices);

	return true;
}

bool AnimationCompiler::read_animation(std::ifstream& readfile, AnimationCompiler::AnimationTrack& info)
{
	double ticksPerSecond;
	readfile >> ticksPerSecond;
	if (ticksPerSecond != 0.0f)
		info.ticksPerSecond = (float)ticksPerSecond;
	else
		info.ticksPerSecond = 1;

	readfile >> info.duration;
	info.boneTransforms = {};

	unsigned int channels;
	readfile >> channels;
	for (unsigned int i = 0; i < channels; ++i)
	{
		BoneTransformTrack track;
		unsigned int keys;
		double time;
		float x, y, z;

		readfile >> keys;
		for (unsigned int j = 0; j < keys; ++j)
		{
			readfile >> time >> x >> y >> z;
			track.positionTimeStamps.push_back((float)time);
			track.positions.push_back(glm::vec3(x, y, z));
		}

		readfile >> keys;
		for (unsigned int j = 0; j < keys; ++j)
		{
			float w;
			readfile >> time >> w >> x >> y >> z;
			track.rotationTimeStamps.push_back((float)time);
			track.rotations.push_back(glm::quat(w, x, y, z));
		}

		readfile >> keys;
		for (unsigned int j = 0; j < keys; ++j)
		{
			readfile >> time >> x >> y >> z;
			track.scaleTimeStamps.push_back((float)time);
			track.scales.push_back(glm::vec3(x, y, z));
		}

		std::string nodeName;
		readfile >> nodeName;
		info.boneTransforms[nodeName] = track;
	}

	return true;
}



// Converts a FBX file into a specified format
void AnimationCompiler::readFBX_to_ownFormat(std::string const& path, 
	bool saveMesh, bool saveSkeleton, bool saveAnimation, bool saveScene)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string msg{ "ERROR::ASSIMP:: " };
		msg += importer.GetErrorString();
		PE_CORE_ERROR(msg.c_str());
		return;
	}
	
	if (std::filesystem::create_directories("Assets\\Animations\\Imports\\"))
		PE_CORE_TRACE("Returned true");

	// open a custom file
	std::string baseFileName = "Assets\\Animations\\Imports\\" + Eos::FileManager::GetFileNameWithoutExtension(path);
	//std::string outputFileName = baseFileName + ".eosanim";


	//std::ofstream outFile;
	//outFile.open(outputFileName, std::ios::out | std::ios::trunc);
	//if (!outFile.is_open())
	//	return;

	// scene related information
	if (saveScene)
	{
		std::string sceneFilename = baseFileName + ".eosscene";
		std::ofstream sceneInfoOut;
		sceneInfoOut.open(sceneFilename, std::ios::out | std::ios::trunc);
		if (sceneInfoOut.is_open())
		{
			sceneInfoOut << "HasAnimations " << (scene->HasAnimations() ? "1" : "0") << "\n";
			sceneInfoOut << "HasCameras " << (scene->HasCameras() ? "1" : "0") << "\n";
			sceneInfoOut << "HasLights " << (scene->HasLights() ? "1" : "0") << "\n";
			sceneInfoOut << "HasMaterials " << (scene->HasMaterials() ? "1" : "0") << "\n";
			sceneInfoOut << "HasMeshes " << (scene->HasMeshes() ? "1" : "0") << "\n";
			sceneInfoOut << "HasTextures " << (scene->HasTextures() ? "1" : "0") << "\n";

			sceneInfoOut << scene->mNumAnimations << "\n";
			sceneInfoOut << scene->mNumMaterials << "\n";
			sceneInfoOut << scene->mNumMeshes << "\n";
			sceneInfoOut << scene->mNumTextures << "\n";

			// Close file
			sceneInfoOut.close();
		}
		else
		{
			// Not open
			PE_CORE_ERROR("Scene File Saving Failed");
		}
	}

	// Mesh related information
	if (saveMesh)
	{
		// If animations only have 1 mesh, then we can do scene->mMeshes[0]
		// If scene have multiple meshes then scene->mMeshes[mesh number]
		aiMesh* mesh = scene->mMeshes[0];

		std::string meshFilename = baseFileName + ".eosmesh";
		std::ofstream meshOut;
		meshOut.open(meshFilename, std::ios::out | std::ios::trunc);
		if (meshOut.is_open())
		{
			// save global inverse
			glm::mat4 globalInvTransform = glm::inverse(mat4_cast(scene->mRootNode->mTransformation));
			save_MAT_4(globalInvTransform, meshOut);

			// save mesh
			save_mesh(mesh, meshOut);
			meshOut.close();
		}
		else
		{
			PE_CORE_ERROR("Mesh saving failed");
		}
	}

	// Skeleton
	if (saveSkeleton)
	{
		std::string skeletonFilename = baseFileName + ".eosskeleton";
		std::ofstream skeletonOut;
		skeletonOut.open(skeletonFilename, std::ios::out | std::ios::trunc);
		if (skeletonOut.is_open())
		{
			save_skeleton(scene->mRootNode, skeletonOut);
			skeletonOut.close();
		}
		else
		{
			PE_CORE_ERROR("Saving skeleton failed");
		}
	}

	// Animation related information
	if (saveAnimation)
	{
		// Animation data
		std::string animFileName = baseFileName + ".eosanimation";
		std::ofstream animOut;
		animOut.open(animFileName, std::ios::out | std::ios::trunc);
		if (animOut.is_open())
		{
			save_animation(scene->mAnimations[0], animOut);
			animOut.close();
		}
		else
		{
			PE_CORE_ERROR("Animation saving failed");
		}
	}
}

/*
AnimationCompiler::Ex_Info AnimationCompiler::readOwnFormat(std::string const& path)
{
	//std::vector<Vertex> outputVertices;
	//std::vector<unsigned int> outputIndices;
	Ex_Info extractedInfo;

	std::ifstream readFile;
	readFile.open(path, std::ifstream::in);

	if (!readFile.is_open())
		PE_CORE_ERROR(path + " unable to open");

	// Load global inverse transform
	extractedInfo.globalInvTransform = read_MAT_4(readFile);

	// Get mesh related information
	unsigned int numVertices;
	readFile >> numVertices;
	for (unsigned int i = 0; i < numVertices; ++i)
	{
		Vertex vert;

		// Read position
		readFile >> vert.position.x >> vert.position.y >> vert.position.z;
		
		// Read normals
		readFile >> vert.normal.x >> vert.normal.y >> vert.normal.z;

		// Read UV
		readFile >> vert.uv.x >> vert.uv.y;

		// Read Bone ID
		vert.boneIDs = glm::ivec4(0);

		// Read Bone weight
		vert.boneWeights = glm::vec4(0.0f);

		extractedInfo.outputVerts.push_back(vert);
	}

	std::unordered_map<std::string, std::pair<int, glm::mat4>> boneInfo;
	std::vector<unsigned int> boneCounts;
	boneCounts.resize(extractedInfo.outputVerts.size(), 0);

	// Looping through each bone
	readFile >> extractedInfo.boneCount;
	for (unsigned int i = 0; i < extractedInfo.boneCount; ++i)
	{
		std::string boneName;
		glm::mat4 offsetMatrix;
		readFile >> boneName;
		offsetMatrix = read_MAT_4(readFile);
		boneInfo[boneName] = { i, offsetMatrix };


		unsigned numWeights;
		readFile >> numWeights;
		for (unsigned int j = 0; j < numWeights; ++j)
		{
			unsigned int id;
			float weight;
			readFile >> id >> weight;
			boneCounts[id]++;

			switch (boneCounts[id])
			{
			case 1:
				extractedInfo.outputVerts[id].boneIDs.x = static_cast<float>(i);
				extractedInfo.outputVerts[id].boneWeights.x = weight;
				break;
			case 2:
				extractedInfo.outputVerts[id].boneIDs.y = static_cast<float>(i);
				extractedInfo.outputVerts[id].boneWeights.y = weight;
				break;
			case 3:
				extractedInfo.outputVerts[id].boneIDs.z = static_cast<float>(i);
				extractedInfo.outputVerts[id].boneWeights.z = weight;
				break;
			case 4:
				extractedInfo.outputVerts[id].boneIDs.w = static_cast<float>(i);
				extractedInfo.outputVerts[id].boneWeights.w = weight;
				break;
			default:
				PE_CORE_WARN("Unable to allocate bone to vertex");
				break;
			}
		}
	}

	// Normalize all weights
	for (Vertex vertex : extractedInfo.outputVerts)
	{
		glm::vec4& boneWeights = vertex.boneWeights;
		float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
		if (totalWeight > 0.0f)
		{
			vertex.boneWeights = glm::vec4(
				boneWeights.x / totalWeight,
				boneWeights.y / totalWeight,
				boneWeights.z / totalWeight,
				boneWeights.w / totalWeight);
		}
	}
	
	// Loading indices
	unsigned int numFaces;
	readFile >> numFaces;
	for (unsigned int i = 0; i < numFaces; ++i)
	{
		unsigned int numIndices;
		readFile >> numIndices;
		for (unsigned int j = 0; j < numIndices; ++j)
		{
			unsigned int index;
			readFile >> index;
			extractedInfo.outputIdxs.push_back(index);
		}
	}

	// Loading Skeleton
	//read_skeleton(readFile, extractedInfo.skeleton, extractedInfo);

	// Loading animation
	//AnimationTrack animation;
	double ticksPerSecond_;
	readFile >> ticksPerSecond_;
	if (ticksPerSecond_ != 0.0f)
		extractedInfo.animtionClip.ticksPerSecond = ticksPerSecond_;
	else
		extractedInfo.animtionClip.ticksPerSecond = 1;

	readFile >> extractedInfo.animtionClip.duration;
	extractedInfo.animtionClip.boneTransforms = {};

	unsigned int channels;
	readFile >> channels;
	for (unsigned int i = 0; i < channels; ++i)
	{
		BoneTransformTrack track;
		unsigned int keys;
		double time;
		float x, y, z;

		readFile >> keys;
		for (unsigned int j = 0; j < keys; ++j)
		{
			readFile >> time;
			readFile >> x >> y >> z;
			track.positionTimeStamps.push_back((float)time);
			track.positions.push_back(glm::vec3(x, y, z));
		}

		readFile >> keys;
		for (unsigned int j = 0; j < keys; ++j)
		{
			float w;
			readFile >> time;
			readFile >> w >> x >> y >> z;
			track.rotationTimeStamps.push_back((float)time);
			track.rotations.push_back(glm::quat(w, x, y, z));
		}

		readFile >> keys;
		for (unsigned int j = 0; j < keys; ++j)
		{
			readFile >> time;
			readFile >> x >> y >> z;
			track.scaleTimeStamps.push_back((float)time);
			track.scales.push_back(glm::vec3(x, y, z));
		}

		std::string nodeName;
		readFile >> nodeName;
		extractedInfo.animtionClip.boneTransforms[nodeName] = track;
	}

	readFile.close();

	return extractedInfo;
}
*/

bool AnimationCompiler::readOwnFormat(const std::string& meshpath, const std::string& skeletonpath, const std::string& animationpath)
{
	//Ex_Info extractedInfo;

	// New things - formerly Ex_Info
	MeshInfo meshinfo;
	SkeletonInfo skeletoninfo;
	AnimationTrack animationTrackinfo;

	std::string meshName = Eos::FileManager::GetFileNameWithoutExtension(meshpath);
	std::string skeletonName = Eos::FileManager::GetFileNameWithoutExtension(skeletonpath);
	std::string animName = Eos::FileManager::GetFileNameWithoutExtension(animationpath);

	std::ifstream readFile;

	readFile.open(meshpath, std::ifstream::in);
	if (readFile.is_open())
	{
		//extractedInfo.globalInvTransform = read_MAT_4(readFile);

		// File is open, proceed to read the mesh
		//read_mesh(readFile, extractedInfo);
		
		if (read_mesh(readFile, meshinfo, skeletoninfo))
		{
			Eos::AssetManager::GetInstance().StoreMesh(meshName.c_str(), meshinfo);
			readFile.close();
		}
		else
		{
			readFile.close();
			PE_CORE_WARN("Mesh {} reading has failed", meshName);
			return false;
		}
	}
	else
	{
		PE_CORE_ERROR(meshpath + " unable to open");
		return false;
	}

	readFile.open(skeletonpath, std::ifstream::in);
	if (readFile.is_open())
	{
		// File is open, proceed to read the skeleton
		//read_skeleton(readFile, extractedInfo.skeleton, extractedInfo);

		if (read_skeleton(readFile, skeletoninfo.skeleton, skeletoninfo))
		{
			Eos::AssetManager::GetInstance().StoreSkeleton(skeletonName.c_str(), skeletoninfo);
			readFile.close();
		}
		else
		{
			readFile.close();
			PE_CORE_WARN("Skeleton {} reading has failed", skeletonName);
			return false;
		}
	}
	else
	{
		PE_CORE_ERROR(skeletonpath + " unable to open");
		return false;
	}

	readFile.open(animationpath, std::ifstream::in);
	if (readFile.is_open())
	{
		// File is open, proceed to read the animation
		//read_animation(readFile, extractedInfo);

		if (read_animation(readFile, animationTrackinfo))
		{
			Eos::AssetManager::GetInstance().StoreAnimation(animName.c_str(), animationTrackinfo);
			readFile.close();
		}
		else
		{
			readFile.close();
			PE_CORE_WARN("Animation {} reading has failed", animName);
			return false;
		}
	}
	else
	{
		PE_CORE_ERROR(animationpath + " unable to open");
		return false;
	}

	return true;
}

void AnimationCompiler::read_mesh_skeleton_from_file(const std::string& meshfile, const std::string& skeletonfile)
{
	MeshInfo meshinfo;
	SkeletonInfo skeletoninfo;

	std::ifstream readfile;

	// Read the mesh
	readfile.open(meshfile, std::ifstream::in);
	if (readfile.is_open())
	{
		//extractedInfo.globalInvTransform = read_MAT_4(readFile);

		// File is open, proceed to read the mesh
		//read_mesh(readFile, extractedInfo);

		if (read_mesh(readfile, meshinfo, skeletoninfo))
		{
			Eos::AssetManager::GetInstance().StoreMesh(meshfile.c_str(), meshinfo);
			readfile.close();
		}
		else
		{
			readfile.close();
			PE_CORE_WARN("Mesh {} reading has failed", meshfile);
			return;
		}
	}
	else
	{
		PE_CORE_ERROR(meshfile + " unable to be found");
		return;
	}

	// Read skeleton data
	readfile.open(skeletonfile, std::ifstream::in);
	if (readfile.is_open())
	{
		// File is open, proceed to read the skeleton
		//read_skeleton(readFile, extractedInfo.skeleton, extractedInfo);

		if (read_skeleton(readfile, skeletoninfo.skeleton, skeletoninfo))
		{
			Eos::AssetManager::GetInstance().StoreSkeleton(skeletonfile.c_str(), skeletoninfo);
			readfile.close();
		}
		else
		{
			readfile.close();
			PE_CORE_WARN("Skeleton {} reading has failed", skeletonfile);
			return;
		}
	}
	else
	{
		PE_CORE_ERROR(skeletonfile + " unable to open");
		return;
	}
}

void AnimationCompiler::read_animation_from_file(const std::string& file)
{
	AnimationTrack animationTrackinfo;

	std::ifstream readFile;
	readFile.open(file, std::ifstream::in);
	if (readFile.is_open())
	{
		// File is open, proceed to read the animation
		//read_animation(readFile, extractedInfo);

		if (read_animation(readFile, animationTrackinfo))
		{
			Eos::AssetManager::GetInstance().StoreAnimation(file.c_str(), animationTrackinfo);
			readFile.close();
		}
		else
		{
			readFile.close();
			PE_CORE_WARN("Animation {} reading has failed", file);
		}
	}
	else
	{
		PE_CORE_ERROR(file + " unable to be found");
	}
}

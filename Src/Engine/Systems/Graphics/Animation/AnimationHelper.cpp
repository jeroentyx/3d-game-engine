#include "AnimationHelper.h"

#include <glm/gtc/type_ptr.hpp>	//make_mat4 make_mat3

//#include <glew.h>	// remove with own vao wrappers TODO
#include <stb_image/stb_image.h>	// remove with own texture wrappers TODO
#include <fstream>

#include "Src/Engine/Systems/Graphics/Platform/OpenGLVertexArray.h"


static inline glm::vec3 vec3_cast(const aiVector3D& v) { return glm::vec3(v.x, v.y, v.z); }
static inline glm::vec2 vec2_cast(const aiVector3D& v) { return glm::vec2(v.x, v.y); } // it's aiVector3D because assimp's texture coordinates use that
static inline glm::quat quat_cast(const aiQuaternion& q) { return glm::quat(q.w, q.x, q.y, q.z); }
static inline glm::mat4 mat4_cast(const aiMatrix4x4& m) { return glm::transpose(glm::make_mat4(&m.a1)); }
static inline glm::mat4 mat4_cast(const aiMatrix3x3& m) { return glm::transpose(glm::make_mat3(&m.a1)); }

// Recursive function to load skeleton in
// if (!readSkeleton(skeletonOutput, scene->mRootNode, boneInfo))
bool readSkeleton(Bone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable)
{
	std::cout << node->mName.C_Str() << "   " << node->mNumChildren << "children" << std::endl;

	if (boneInfoTable.find(node->mName.C_Str()) != boneInfoTable.end())
	{
		boneOutput.name = node->mName.C_Str();
		boneOutput.id = boneInfoTable[boneOutput.name].first;
		boneOutput.offset = boneInfoTable[boneOutput.name].second;

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			//aiNode* childlvl1 = node->mChildren[i];
			//std::cout << childlvl1->mName.C_Str() << " " << childlvl1->mNumChildren << "children" << std::endl;
			
			Bone child;
			readSkeleton(child, node->mChildren[i], boneInfoTable);
			boneOutput.children.push_back(child);
		}
		return true;
	}
	else
	{
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			if (readSkeleton(boneOutput, node->mChildren[i], boneInfoTable))
				return true;
		}
	}

	return false;
}


void loadModel(const aiScene* scene, 
	aiMesh* mesh, 
	std::vector<Vertex>& verticesOutput, 
	std::vector<unsigned int>& indicesOutput, 
	Bone& skeletonOutput, 
	unsigned int& nBoneCount)
{
	//load position, normal, uv
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		//process position 
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		//process normal
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		//process uv
		glm::vec2 vec;
		vec.x = mesh->mTextureCoords[0][i].x;
		vec.y = mesh->mTextureCoords[0][i].y;
		vertex.uv = vec;

		vertex.boneIDs = glm::ivec4(0);
		vertex.boneWeights = glm::vec4(0.0f);

		verticesOutput.push_back(vertex);
	}

	//load boneData to vertices
	std::unordered_map<std::string, std::pair<int, glm::mat4>> boneInfo = {};
	std::vector<unsigned int> boneCounts;
	boneCounts.resize(verticesOutput.size(), 0);
	nBoneCount = mesh->mNumBones;

	//loop through each bone
	for (unsigned int i = 0; i < nBoneCount; i++)
	{
		aiBone* bone = mesh->mBones[i];
		glm::mat4 m = mat4_cast(bone->mOffsetMatrix);
		boneInfo[bone->mName.C_Str()] = { i, m };
		//std::cout << bone->mName.C_Str() << std::endl;

		//loop through each vertex that have that bone
		for (unsigned int j = 0; j < bone->mNumWeights; j++)
		{
			unsigned int id = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights[j].mWeight;
			boneCounts[id]++;

			//std::cout << "j: " << j << " bone_id: " << id << std::endl;

			switch (boneCounts[id])
			{
			case 1:
				verticesOutput[id].boneIDs.x = static_cast<float>(i);
				verticesOutput[id].boneWeights.x = weight;
				break;
			case 2:
				verticesOutput[id].boneIDs.y = static_cast<float>(i);
				verticesOutput[id].boneWeights.y = weight;
				break;
			case 3:
				verticesOutput[id].boneIDs.z = static_cast<float>(i);
				verticesOutput[id].boneWeights.z = weight;
				break;
			case 4:
				verticesOutput[id].boneIDs.w = static_cast<float>(i);
				verticesOutput[id].boneWeights.w = weight;
				break;
			default:
				//std::cout << "err: unable to allocate bone to vertex" << std::endl;
				break;

			}
		}
	}

	//normalize weights to make all weights sum 1
	for (int i = 0; i < verticesOutput.size(); i++) 
	{
		glm::vec4& boneWeights = verticesOutput[i].boneWeights;
		float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
		if (totalWeight > 0.0f) 
		{
			verticesOutput[i].boneWeights = glm::vec4(
				boneWeights.x / totalWeight,
				boneWeights.y / totalWeight,
				boneWeights.z / totalWeight,
				boneWeights.w / totalWeight
			);
		}
	}

	//load indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) 
	{
		aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indicesOutput.push_back(face.mIndices[j]);
	}


	// create bone hirerchy
	if (!readSkeleton(skeletonOutput, scene->mRootNode, boneInfo))
		PE_CORE_WARN("Animation skeleton has failed to be read");
}

void loadAnimation(const aiScene* scene, Animation& animation)
{
	aiAnimation* anim = scene->mAnimations[0];

	if (anim->mTicksPerSecond != 0.0f)
		animation.ticksPerSecond = (float)anim->mTicksPerSecond;
	else
		animation.ticksPerSecond = 1;

	animation.duration = (float)anim->mDuration;
	animation.boneTransforms = {};

	//load positions rotations and scales for each bone
	// each channel represents each bone
	for (unsigned int i = 0; i < anim->mNumChannels; i++)
	{
		//std::cout << anim->mChannels[i]->mNodeName.C_Str() << std::endl;
		aiNodeAnim* channel = anim->mChannels[i];
		BoneTransformTrack track;

		std::cout << "Channel: " << channel->mNodeName.C_Str() << std::endl;

		for (unsigned int j = 0; j < channel->mNumPositionKeys; j++)
		{
			track.positionTimeStamps.push_back((float)channel->mPositionKeys[j].mTime);
			track.positions.push_back(vec3_cast(channel->mPositionKeys[j].mValue));
		}

		for (unsigned int j = 0; j < channel->mNumRotationKeys; j++)
		{
			track.rotationTimeStamps.push_back((float)channel->mRotationKeys[j].mTime);
			track.rotations.push_back(quat_cast(channel->mRotationKeys[j].mValue));
		}

		for (unsigned int j = 0; j < channel->mNumScalingKeys; j++)
		{
			track.scaleTimeStamps.push_back((float)channel->mScalingKeys[j].mTime);
			track.scales.push_back(vec3_cast(channel->mScalingKeys[j].mValue));
		}
		animation.boneTransforms[channel->mNodeName.C_Str()] = track;
	}
}

unsigned int createVertexArray(std::vector<Vertex>& vertices, std::vector<unsigned int> indices)
{
	unsigned int vao = 0;
	unsigned int vbo = 0;
	unsigned int ebo = 0;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, boneIDs));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, boneWeights));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
	return vao;


	/*std::shared_ptr<VertexArray> VertexArray;
	std::shared_ptr<VertexBuffer> VertexBuffer;

	VertexArray = VertexArray::Create();
	VertexBuffer = VertexBuffer::Create(&vertices[0], sizeof(Vertex) * vertices.size());
	VertexBuffer->SetLayout({
		{ShaderDataType::Float3, "position"},
		{ShaderDataType::Float3, "normal"},
		{ShaderDataType::Float2, "uv"},
		{ShaderDataType::Float4, "boneIDs"},
		{ShaderDataType::Float4, "boneWeights"}
		});
	VertexArray->AddVertexBuffer(VertexBuffer);*/
}

std::istream& safeGetline(std::istream& is, std::string& t)
{
	t.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	std::istream::sentry se(is, true);
	std::streambuf* sb = is.rdbuf();

	for (;;) {
		int c = sb->sbumpc();
		switch (c) {
		case '\n':
			return is;
		case '\r':
			if (sb->sgetc() == '\n')
				sb->sbumpc();
			return is;
		case std::streambuf::traits_type::eof():
			// Also handle the case when the last line has no line ending
			if (t.empty())
				is.setstate(std::ios::eofbit);
			return is;
		default:
			t += (char)c;
		}
	}
}

unsigned int createTexture(std::string filepath)
{
	unsigned int textureId = 0;
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 4);

	//std::cout << data << std::endl;
	//std::ofstream outfile;
	//outfile.open("testimagebinary.eostexture", std::ios::binary);
	//outfile << data;
	//outfile.close();

	//unsigned char* newdata;
	//std::string str;
	//std::ifstream infile("testimagebinary.eostexture", std::ios::binary);
	//if (!infile.is_open())
	//	std::cout << "Failed to reopen" << std::endl;

	//while (!safeGetline(infile, str).eof()) {}
	//	//std::cout << str << std::endl;
	//infile.close();


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

void listOut(Bone& skeleton)
{
	//std::cout << "Name: " << skeleton.name << std::endl;
	//std::cout << "id: " << skeleton.id << std::endl;
	for (auto xy : skeleton.children)
	{
		listOut(xy);
	}
}

void ViewSkeleton(Bone skeleton)
{
	std::cout << skeleton.name << std::endl;
	for (Bone bone : skeleton.children)
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
		if(child.name != "")
			getPose(animation, child, dt, output, globalTransform, globalInverseTransform);
}

//void getPoseMultiMesh(Animation& animation, std::vector<BodyRig>& body,	float dt,
//	std::vector<glm::mat4>& output,	glm::mat4& parentTransform,	glm::mat4& globalInverseTransform)
//{
//	for (BodyRig bodypart : body)
//	{
//		// Check if body part has animation
//		if (bodypart.name != "Arm R")
//			continue;
//
//		// Calculate translation
//		BoneTransformTrack& btt = animation.boneTransforms["Arm R_$AssimpFbx$_Translation"];
//		dt = fmod(dt, animation.duration);
//		std::pair<unsigned int, float> fp;
//		fp = getTimeFraction(btt.positionTimeStamps, dt);
//
//		glm::vec3 position1 = btt.positions[fp.first - 1];
//		glm::vec3 position2 = btt.positions[fp.first];
//		glm::vec3 position = glm::mix(position1, position2, fp.second);
//
//		//calculate interpolated rotation
//		btt = animation.boneTransforms["Arm R_$AssimpFbx$_Rotation"];
//		fp = getTimeFraction(btt.rotationTimeStamps, dt);
//		glm::quat rotation1 = btt.rotations[fp.first - 1];
//		glm::quat rotation2 = btt.rotations[fp.first];
//		glm::quat rotation = glm::slerp(rotation1, rotation2, fp.second);
//
//		//calculate interpolated scale
//		btt = animation.boneTransforms["Arm R_$AssimpFbx$_Scaling"];
//		fp = getTimeFraction(btt.scaleTimeStamps, dt);
//		glm::vec3 scale1 = btt.scales[fp.first - 1];
//		glm::vec3 scale2 = btt.scales[fp.first];
//		glm::vec3 scale = glm::mix(scale1, scale2, fp.second);
//
//		glm::mat4 positionMat = glm::mat4(1.0),
//			scaleMat = glm::mat4(1.0);
//
//		// calculate localTransform
//		positionMat = glm::translate(positionMat, position);
//		glm::mat4 rotationMat = glm::toMat4(rotation);
//		scaleMat = glm::scale(scaleMat, scale);
//		glm::mat4 localTransform = positionMat * rotationMat * scaleMat;
//
//		glm::mat4 globalTransform = parentTransform * localTransform;
//
//		output[bodypart.BodyBone.id] = globalInverseTransform * globalTransform * bodypart.BodyBone.offset;
//	}
//}

TestAnimation::TestAnimation() : 
	viewProjectionMatrixLocation{0},
	modelMatrixLocation{0},
	boneMatricesLocation{0},
	textureLocation{0},
	diffuseTexture{0}
{
}

TestAnimation::~TestAnimation()
{
}


void TestAnimation::Init(std::string const& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string msg{ "ERROR::ASSIMP:: " };
		msg += importer.GetErrorString();
		PE_CORE_ERROR(msg.c_str());
	}

	aiMesh* mesh = nullptr;
	if (scene)
		mesh = scene->mMeshes[0];

	{
		//std::cout << "Check animations: " << (scene->HasAnimations() ? "Yes" : "No") << scene->mNumAnimations << std::endl;
		//std::cout << "Check cameras: " << (scene->HasCameras() ? "Yes" : "No") << scene->mNumCameras << std::endl;
		//std::cout << "Check lights: " << (scene->HasLights() ? "Yes" : "No") << scene->mNumLights << std::endl;
		//std::cout << "Check materials: " << (scene->HasMaterials() ? "Yes" : "No") << scene->mNumMaterials << std::endl;
		//std::cout << "Check meshes: " << (scene->HasMeshes() ? "Yes" : "No") << scene->mNumMeshes << std::endl;
		//std::cout << "Check textures: " << (scene->HasTextures() ? "Yes" : "No") << scene->mNumTextures << std::endl;

		//std::cout << "bones" << (mesh->HasBones() ? "Y" : "N") << mesh->mNumBones << std::endl;
		//std::cout << "faces" << (mesh->HasFaces() ? "Y" : "N") << mesh->mNumFaces << std::endl;
		//std::cout << "normals" << (mesh->HasNormals() ? "Y" : "N") << mesh->mNormals->x << "," << mesh->mNormals->y << "," << mesh->mNormals->z << std::endl;

		/*std::cout << "Bone names" << std::endl;
		for (unsigned int i = 0; i < mesh->mNumBones; ++i)
			std::cout << mesh->mBones[i]->mName.C_Str() << std::endl;
		*/
	}

	// Problem: instead of having 1 mesh with 1 body rig
	// we have 6 meshes with 6 individual body rigs that have 1 bone each
	// Load model and Save the individual body parts
	//for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	//{
	//	//SurgeNoColor
	//	BodyRig bodypart;
	//	unsigned int localBoneCount;
	//	loadModel(scene, scene->mMeshes[i], bodypart.BodyVertices, bodypart.BodyIndices, bodypart.BodyBone, localBoneCount);
	//	boneCount += localBoneCount;
	//	//std::cout << scene->mMeshes[i]->mBones[0]->mName.C_Str() << " local bone count: " << localBoneCount << std::endl;
	//	bodypart.name = scene->mMeshes[i]->mName.C_Str();
	//	SurgeNoColor.push_back(bodypart);
	//}

	//as the name suggests just inverse the global transform
	globalInverseTransform = glm::inverse(mat4_cast(scene->mRootNode->mTransformation));

	// For model loading wtih blender
	loadModel(scene, mesh, vertices, indices, skeleton, boneCount);
	loadAnimation(scene, animation);

	// Create VAO
	vao = createVertexArray(vertices, indices);

	// Load embedded texture
	diffuseTexture = createTexture("Assets/Animations/Surge_split.png");
	/*aiString texture_file;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file);
	const aiTexture* texture = scene->GetEmbeddedTexture(texture_file.C_Str());
	if (texture)
	{

	}
	else
	{
		PE_CORE_WARN("No textures present");
	}*/
	

	currentPose.resize(boneCount, identity); // use this for no animation

	// Create shader
	m_shader = Eos::Shader::Create("shaders/testanimation.glsl");

	//get all shader uniform locations
	viewProjectionMatrixLocation = glGetUniformLocation(m_shader->id, "view_projection_matrix");
	modelMatrixLocation = glGetUniformLocation(m_shader->id, "model_matrix");
	boneMatricesLocation = glGetUniformLocation(m_shader->id, "bone_transforms");
	textureLocation = glGetUniformLocation(m_shader->id, "diff_texture");
}


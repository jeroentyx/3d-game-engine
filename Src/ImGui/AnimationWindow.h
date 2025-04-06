#pragma once

#include "IImGuiWindow.h"
#include "Src\Engine\Systems\Graphics\Compiler\AnimationCompiler.h"

#include "Src/Engine/Systems/Graphics/Renderer/Camera/PerspectiveCamera.h"
#include "Src/Engine/Systems/Graphics/Platform/OpenGLFramebuffer.h"
#include "Src\ImGui\Context.h"

#include <memory>
#include "Src\Engine\Components\glslprogram.h"
#include "Src\Engine\Components\grid.h"

struct Sequence;

namespace Eos
{
	class AnimationWindow : public IImGuiWindow
	{
		ImGuiID id;
		void DisplayBoneSkeleton(AnimationCompiler::Bone bone);
		void DisplayAnimSequencer();
		void DisplayAnimPreview();
		void DisplayAnimDetails();

		//Sequence* mysequence;
		std::unique_ptr<Sequence> mysequence;
		int selectedEntry = -1;
		int firstFrame = 0;
		int currentFrame = 100;
		bool expanded = true;

		// Loaded clip
		AnimationCompiler::Ex_Info* clip;
		bool isLoaded = false;
		GLSLProgram prog;
		Grid gridProgram;


		//vertices for the framebuffer object
		float quadVertices[24]= { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
		};

		float fbo_width;
		float fbo_height;
		unsigned int rbo; //the depth buffers

		unsigned int diffuseTexture;
		std::vector<glm::mat4> currentPose;
		glm::mat4 identity{ glm::mat4(1.0f) };

		// Camera
		PerspectiveCamera m_camera;
		std::string playingString;
	public:
		AnimationWindow();

		void SetAnimationClip(AnimationCompiler::Ex_Info* targetClip);

		virtual void Update() override;
		unsigned int quadVAO, quadVBO;
		unsigned int framebuffer;
		unsigned int textureColorBuffer;
		std::shared_ptr<Framebuffer> m_Framebuffer;

		AnimationCompiler::MeshInfo* localMesh;
		AnimationCompiler::SkeletonInfo* localSkeleton;
		AnimationCompiler::AnimationTrack* localTrack;
		float localTimer;

	};
}
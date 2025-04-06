#pragma once

#include <map>
#include <string>
#include "Shader.h"
#include "..\Src\pch.h"
#include "..\..\Core\Interfaces\ISystem.h"
#include "..\..\Core\Interfaces\IComponent.h"
#include "..\..\ComponentSystemID.h"
#include "Src\Events\Events.h"
#include "Src\Events\EventDispatcher.h"
#include "Src\Events\EventObserver.h"

#include "UIElements.h"

#include "Renderer\Camera\PerspectiveCamera.h"
#include "Shader.h"
#include "Platform\OpenGLVertexArray.h"
#include "Renderer\RenderCommand.h"
#include "Font\Font.h"

#include "Src\Engine\Components\Transform\TransformComponent.h"
#include "Src\Engine\Components\Mesh\MeshComponent.h"
#include "Src\Engine\Components\Mesh\StaticMeshComponent.h"
#include "Src\Engine\Components\Mesh\AnimatedMeshComponent.h"
#include "Src\Engine\Components\UI\UI_Sprite2DComponent.h"
#include "Src\Engine\Components\Skybox\SkyboxComponent.h"
#include "Src\Engine\Components\Font\FontComponent.h"
#include "Src\Engine\Components\SmokeComponent.h"

#include "Src\Engine\Core\Manager\ComponentManager.h"
#include "Src\ImGui\Context.h"

#include "EosRenderer\TestMesh.h"
#include "EosRenderer\TestModel.h"
#include "EosRenderer\TestShader.h"
#include "EosRenderer\TestSkyBox.h"
#include "Tools\GraphicsHelper.h"
#include "Src\Engine\Components\LightComponent.h"
#include "Src\Engine\Components\GUIComponent.h"
#include "Src\Engine\Components\Camera\CameraComponent.h"
#include "Src\Engine\Components\\Billboard\BillboardComponent.h"

#include "tracy\Tracy.hpp"
#include "EosRenderer\EosTexture2D.h"
#include "EosRenderer\EosFramebuffer.h"

//#include "Animation\AnimationHelper.h"
#include "Src\Engine\Systems\Graphics\Renderer\Texture.h"
#include "Compiler\TextureLoader.h"
#include "Src\Engine\Components\ParticleComponent.h"
#include "Src\Engine\Components\SmokeComponent.h"

class Rendering_System : public ISystem
{

public:
	//std::map<int, std::shared_ptr<Transform_Component>> transforms;
	Rendering_System();

	std::map<EntityID, std::shared_ptr<Mesh_Component>>meshes;
	std::map<EntityID, std::shared_ptr<Light_Component>>lights;
	std::map<EntityID, std::shared_ptr<Transform_Component>>* trans{ nullptr };

	std::map<EntityID, std::shared_ptr<GUI_Component>> OpenGLGuis;


	std::map<EntityID, std::shared_ptr<Static_Mesh_Component>> static_meshes;
	std::map<EntityID, std::shared_ptr<Animated_Mesh_Component>> dyna_meshes;
	std::map<EntityID, std::shared_ptr<Camera_Component>> cam;

	std::map<EntityID, std::shared_ptr<Skybox_Component>> movingSkyboxes;
	std::map<EntityID, std::shared_ptr<Font_Component>> textAndFonts;
	std::map<EntityID, std::shared_ptr<Particle_Component>> particles;
	std::map<EntityID, std::shared_ptr<Smoke_Component>> SmokeParticles;

	std::map<EntityID, std::shared_ptr<BillBoard_Component>> billboards;


	float quadVertices[24] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};

	float planeVertices[48] = {
		// positions            // normals         // texcoords
		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};

	uint32_t quadVAO, quadVBO;
	GLuint texId = 0;
	uint32_t fbo_width, fbo_height;
	uint32_t framebuffer;
	uint32_t textureColorbuffer;
	uint32_t rbo;

	Eos::Shader UISceneShader;
	unsigned int UISceneFrameBuffer;
	unsigned int UISceneColorBuffer, UISceneRBO;
	unsigned int sceneVao, sceneVBO;


	uint32_t planeVAO, planeVBO;
	uint32_t floorTexture;


	//shaders
	Eos::Shader screenShader;
	Eos::Shader skyboxShader;
	Eos::Shader lighting;

	//shader in LearnOpengl
	
	Test::Shader shadow_mapping_vs;
	Test::Shader simpleDepthShader;
	Test::Shader debugDepthQuad;

	//ParticleComponent particleComp;
	std::shared_ptr<Smoke_Component> smokeComponent;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	unsigned int depthMap;
	glm::vec3 shadow_lightPos;
	std::shared_ptr<Eos::Shader> m_SurgeShader;

	std::vector<glm::vec3> lightPosition;
	std::vector<glm::vec3> lightColors;

	Test::GfxSkyBox TestSkyBox;

	Font font;
	PerspectiveCamera m_persCamera;
	uint32_t VBO, VAO;

	int32_t p_width, p_height;
	int32_t p_posX, p_posY;

	int32_t windowWidth, windowHeight;
	int32_t gamescreen_posX, gamescreen_posY;

	OpenGuiTexture uiElements_;

	bool blinn = false;
	bool gamma = true;

	std::vector<std::string> faces
	{
		/*"Assets/Textures/right.jpg",
		"Assets/Textures/left.jpg",
		"Assets/Textures/bottom.jpg",
		"Assets/Textures/top.jpg",
		"Assets/Textures/front.jpg",
		"Assets/Textures/back.jpg"*/

		"Assets/Textures/GameBox/Right.png",
		"Assets/Textures/GameBox/Left.png",
		"Assets/Textures/GameBox/Bottom.png",
		"Assets/Textures/GameBox/Up.png",
		"Assets/Textures/GameBox/Front.png",
		"Assets/Textures/GameBox/Back.png"
	};

	float skyboxVertices[108] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	uint32_t skyboxVAO, skyboxVBO;
	unsigned int cubemapTexture;

	//Light settings
	glm::vec3 light_direction = { -0.4f,-1.0f,-0.8f };
	glm::vec3 light_color = { 1.0f,1.0f,1.0f };
	glm::vec3 ambient_color{ 0 / (float)255, 110 / (float)255, 148 / (float)255 };
	/*std::shared_ptr<ParticleComponent> particleSystem;*/

	unsigned int diffuseMap = 0;
	unsigned int glossMap = 0;
	unsigned int normalMap = 0;

	glm::vec3 lightPos{ -1.2, -5.6f, 8.540 };

	bool DrawDebug{ false };
	bool updateCamera{ true };
	void Init();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void Exit();

	void AddingNewComponent(EntityID entityID, COMPONENTNAME compName, std::shared_ptr<IComponent> compPtr);
	void RemovingComponent(EntityID entityID, COMPONENTNAME compName);
	void ClearAllComponents();

	//Fuctions to get Meshes map out of graphics system
	const std::map<EntityID, std::shared_ptr<Static_Mesh_Component>>& GetStaticMeshMap();
	const std::map<EntityID, std::shared_ptr<Mesh_Component>>& GetMeshesMap();

	// Static mesh things
	Test::Shader m_staticMeshShader;

	//Animationthings
	unsigned int diffuseTexture;
	std::shared_ptr<Eos::Shader> m_animationshader;

public:
	void CreateUISceneFBO();
	void BindUISceneFBO();
	void ResizeSceneFBO();
	void SetUpSceneFBO();

	void CreateFbo();
	void BindFbo();
	void SetupToSendFbo();
	void ResizeFbo();


	bool blinnPhong = false;
	bool normal = false;
	bool shadow = false;

	float elapsedTime = 0.1f;	// cannot start from 0

	//std::shared_ptr<Texture2D> billboard;
	std::shared_ptr<Eos::Shader> m_billboard_shader;
	std::shared_ptr<VertexArray> m_billboard_vao;

	Eos::Shader bill_shader;
	std::shared_ptr<VertexArray> m_testVA;
	bool isFixedSize = false;				// Hard code value to control if fixed shape or not inside shader
};



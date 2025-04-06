#include "UI_System.h"

void UI_System::Init()
{
	//billboard = Texture2D::Create("Assets/Textures/Bill_Test/container.png");
	//bill_shader.Init("shaders/billboard.vert", "shaders/billboard.frag");

	// Create VAO, VBO and EBO
	//m_testVA = VertexArray::Create();
	//float vertices[3 * 4] = {
	//	 -0.5f, -0.5f, 0.0f,
	//	  0.5f, -0.5f, 0.0f,
	//	 -0.5f,  0.5f, 0.0f,
	//	  0.5f,  0.5f, 0.0f,
	//};
	//std::shared_ptr<VertexBuffer> testVB;
	//testVB = VertexBuffer::Create(vertices, sizeof(vertices));
	//testVB->SetLayout({
	//	{ShaderDataType::Float3, "a_Position"}
	//	});
	//m_testVA->AddVertexBuffer(testVB);

	//GLuint squareIndices[6] = { 0, 1, 2, 2, 3, 1 };
	//std::shared_ptr<IndexBuffer> squareIB;
	//squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(GLuint));
	//m_testVA->SetIndexBuffer(squareIB);
}

void UI_System::Update()
{
	glm::vec3 camPos = std::dynamic_pointer_cast<Rendering_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS))->m_persCamera.cameraPos;
	glm::vec3 camFront = std::dynamic_pointer_cast<Rendering_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS))->m_persCamera.cameraFront;
	cam = camPos + camFront;
	//For each Entity with UI component, get the transform and move it's position to the camera's position + offset
	for (auto const& x : uiComps)
	{
		if (GET_ENTITY_MANAGER.getActive(x.first)) 
		{
			std::shared_ptr<Transform_Component> temp = GET_COMPONENT_MANAGER.GetTransformComponent(x.first);
			temp->Translation = glm::vec3{ -cam.x + x.second->pos.x, -cam.y + x.second->pos.y, cam.z };
		}
	}

	//PerspectiveCamera& _cam = std::dynamic_pointer_cast<Rendering_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS))->m_persCamera;
	//glm::mat4 view = _cam.GetViewMaxtrix();
	//glm::mat4 proj = _cam.GetProjectionMatrix();
	//glm::mat4 viewproj = proj * view;

	//for (auto const& sprite : sprites)
	{
		//std::shared_ptr<Transform_Component> tc = ComponentManager::GetInstance().GetTransformComponent(sprite.first);

		//bill_shader.Bind();

		// Extract out texture or color
		// bind texture
		//glBindTexture(GL_TEXTURE_2D, billboard->GetID());

		//glm::mat4 viewproj = proj * view;
		// Set uniforms
		//bill_shader.SetMat4("u_ViewProjection", viewproj);
		//bill_shader.SetVec4("u_Color", { 1.0f, 0.0f, 0.0f, 1.0f });

		// transform consists of pos, scale and rotate
		//bill_shader.SetMat4("u_Transform", glm::mat4(1.0f));

		// Create vec3 and vec2 to pass into uniform locations
		//glm::vec3 u_camRight = glm::vec3(view[0][0], view[1][0], view[2][0]);
		//glm::vec3 u_camUp = glm::vec3(view[0][1], view[1][1], view[2][1]);


		//glm::vec3 u_billPos = glm::vec3(1.0f, 0.0f, 0.0f);
		//glm::vec2 u_billSize = glm::vec2(100.0f, 100.0f);

		// Pass data into uniforms
		//isFixedSize = false;	// control is fixed size or not on shader side

		//bill_shader.SetBool("FixedSize", isFixedSize);
		//bill_shader.SetVec3("CameraRight_worldspace", u_camRight);
		//bill_shader.SetVec3("CameraUp_worldspace", u_camUp);

		//bill_shader.SetVec3("BillboardPos", u_billPos);
		//bill_shader.SetVec2("BillboardSize", u_billSize);

		// Begin drawing
		//m_testVA->Bind();
		//GLuint count = m_testVA->GetIndexBuffer()->GetCount();
		//glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

		// After drawing
		//bill_shader.unBind();
		//glBindVertexArray(0);
	}
}

void UI_System::Exit()
{
}

void UI_System::AddingNewComponent(EntityID entityID, COMPONENTNAME compName, std::shared_ptr<IComponent> compPtr)
{
	if (compName == COMPONENTNAME::Comp_UI_Component)
	{
		if (!uiComps.contains(entityID))
		{
			std::shared_ptr<UI_Component> temp = std::dynamic_pointer_cast<UI_Component>(compPtr);
			uiComps.insert({ entityID, temp });
		}
	}
	else if (compName == COMPONENTNAME::Comp_UI_Sprite2D_Component)
	{
		if (!sprites.contains(entityID))
		{
			sprites.insert({ entityID, std::dynamic_pointer_cast<UI_Sprite2D_Component>(compPtr) });
		}
	}
}

void UI_System::RemovingComponent(EntityID entityID, COMPONENTNAME compName)
{
	if (compName == COMPONENTNAME::Comp_UI_Component)
	{
		uiComps.erase(entityID);
	}
	else if (compName == COMPONENTNAME::Comp_UI_Sprite2D_Component)
	{
		sprites.erase(entityID);
	}
}

void UI_System::ClearAllComponents()
{
	uiComps.clear();
	sprites.clear();
}

void UI_Component::Init()
{
}


#include "GameScreenWindow.h"

namespace Eos
{
	GameScreenWindow::GameScreenWindow()
	{
		renderingSystem = SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS);
		this->m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

		PE_SUBSCRIBE_EVENT
		(
			SendFrameBufferObjectEvent,
			[this](SendFrameBufferObjectEvent& _evn)
			{
				colorBuffer = _evn.textureColorBuffer;
			}
		);

		PE_SUBSCRIBE_EVENT
		(
			SendSelectionIDEvent,
			[this](SendSelectionIDEvent& _evn)
			{
				m_selectedEntity = _evn.selectedEntity;
			}
		);

		Eos::PE_SUBSCRIBE_EVENT
		(
			Eos::SetPickingEvent,
			[this](Eos::SetPickingEvent& _evn)
			{
				SetMousePickingOn = _evn.togglePicking;
			}
		);
	}

	void GameScreenWindow::Update()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 1,1 });
		if (ImGui::Begin("Scene", &active, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
		{
			
			if (Inputs::GetInstance().IsKeyPressed(KeyCode::LeftAlt))
				m_cursor != m_cursor;

			if(m_cursor)
				Inputs::GetInstance().SetCursorMode(Cursor::Normal);
		
			

			if (CoreManager::GetInstance().getEngineState() == ENGINESTATE::STOP ||
				CoreManager::GetInstance().getEngineState() == ENGINESTATE::PAUSE)
			{
				std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(renderingSystem);
				if (rs->m_persCamera.m_active)
				{
					//Another if check to disable the editor camera during play
					if (ImGui::IsWindowHovered() && Inputs::GetInstance().IsMouseButtonPressed(Eos::Button::Right))
					{
						rs->m_persCamera.UpdateZoom();
						rs->m_persCamera.Update();
					}
				}
			}

			//if (Inputs::GetInstance().IsMouseButtonPressed(Eos::Button::Left))
			{
				//
				//  MousePicking here
				//


				std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(renderingSystem);

				auto [currMouseX, currMouseY] = ImGui::GetCursorPos();
				auto [windowPosX, windowPosY] = ImGui::GetWindowPos();
				auto [windowSizeX, windowSizeY] = ImGui::GetWindowSize();
				auto [width, height] = ImGui::GetContentRegionAvail();
				auto [mx, my] = ImGui::GetMousePos();

				float fieldOfView = rs->m_persCamera.fov;

				glm::mat4 projection = glm::perspectiveFov(glm::radians(fieldOfView), width, height, 0.1f, 1000.0f);
				glm::vec2 minBound = glm::vec2(windowPosX, windowPosY);

				minBound.x += currMouseX;
				minBound.y += currMouseY;

				glm::vec2 maxBound = glm::vec2(minBound.x + windowSizeX, minBound.y + windowSizeY);

				glm::vec2 viewportBound[2];
				viewportBound[0] = { minBound.x,minBound.y };
				viewportBound[1] = { maxBound.x,maxBound.y };

				mx -= viewportBound[0].x;
				my -= viewportBound[0].y;

				auto viewportWidth = viewportBound[1].x - viewportBound[0].x;
				auto viewportHeight = viewportBound[1].y - viewportBound[0].y;

				std::pair<float, float>mouse = std::make_pair(
					(mx / viewportWidth) * 2.0f - 1.0f,
					((my / viewportHeight) * 2.0f - 1.0f) * -1.0f);

				//NDC Coordinates for deltaY
				//auto [deltaMouseX, deltaMouseY] = Context::GetInstance().cursorDelta();
				//minBound.x += deltaMouseX;
				//minBound.y += deltaMouseY;
				//deltaMouseX -= viewportBound[0].x;
				//deltaMouseY -= viewportBound[0].y;
				//
				//std::pair<float, float>NDCDeltaMouseCoordinates = std::make_pair(
				//	(deltaMouseX / viewportWidth) * 2.0f - 1.0f,
				//	((deltaMouseY / viewportHeight) * 2.0f - 1.0f) * -1.0f);
				//
				//std::cout << deltaMouseX<< ", " <<
					//deltaMouseY << std::endl;


				bool intersect = false;

				if (mouse.first > -1.0f && mouse.first < 1.0f && mouse.second > -1.0f &&
					mouse.second < 1.0f)
				{
					float mouseX = mouse.first;
					float mouseY = mouse.second;

					//
					//	Done NDC Coordinates Basically i normalized it to NDC Coordinates 
					//
					Inputs::GetInstance().SetMouseNDCCoordinates(glm::vec2(mouseX, mouseY)); // Just send to Inputs using instance
																							//avoided events because GameScreenWindow will not be active during Released mode
					//std::cout << Inputs::GetInstance().GetMouseNDCCoordinates().x << ", " << Inputs::GetInstance().GetMouseNDCCoordinates().y << std::endl;


					//Need to send this mouseX and mouseY to 

					


					glm::vec4 mouseClipPos = { mouseX,mouseY,-1.0f,1.0 };

					auto inverseProj = glm::inverse(projection);
					auto inverseView = glm::inverse(glm::mat3(rs->m_persCamera.GetViewMaxtrix()));

					glm::vec4 ray = inverseProj * mouseClipPos;
					glm::vec3 rayPos = rs->m_persCamera.GetPosition();
					glm::vec3 rayDir = inverseView * glm::vec3(ray);

					std::pair<glm::vec3, glm::vec3> raycast = std::make_pair(rayPos, rayDir);

					glm::vec3 origin = raycast.first;
					glm::vec3 direction = raycast.second;


					std::map<EntityID, std::shared_ptr<Mesh_Component>>meshes = rs->GetMeshesMap();

					for (auto mesh = meshes.begin(); mesh != meshes.end(); mesh++)
					{
						EntityID entt = (*mesh).first;
						Mesh_Component* mc = (*mesh).second.get();
						UNREFERENCED_PARAMETER(mc);

						Transform_Component* tc = nullptr;
						if (ComponentManager::GetInstance().GetTransformComponent(entt).get())
							tc = ComponentManager::GetInstance().GetTransformComponent(entt).get();
						if (!tc)
							continue;

						glm::mat4 transform = tc->GetModelMatrix();
						using Ray = std::pair<glm::vec3, glm::vec3>;

						Ray _ray;
						_ray.first = (glm::inverse(transform) * glm::vec4(origin, 1.0f));
						_ray.second = (glm::inverse(glm::mat3(transform)) * direction);

						float t;
						UNREFERENCED_PARAMETER(t);
						glm::vec3 boundingBox[3];
						glm::vec3 centerpoint = tc->Translation;
						glm::vec3 scale = tc->Scale;

						glm::vec3 min = centerpoint - (scale * 1.f);
						glm::vec3 max = centerpoint + (scale * 1.f);
						boundingBox[0] = centerpoint;
						boundingBox[1] = min;
						boundingBox[2] = max;

						glm::vec3 dirfrac;
						dirfrac.x = 1.0f / _ray.second.x;
						dirfrac.y = 1.0f / _ray.second.y;
						dirfrac.z = 1.0f / _ray.second.z;

						glm::vec3 lb = boundingBox[1];
						glm::vec3 rt = boundingBox[2];

						float t1 = (lb.x - _ray.first.x) * dirfrac.x;
						float t2 = (rt.x - _ray.first.x) * dirfrac.x;
						float t3 = (lb.y - _ray.first.y) * dirfrac.y;
						float t4 = (rt.y - _ray.first.y) * dirfrac.y;
						float t5 = (lb.z - _ray.first.z) * dirfrac.z;
						float t6 = (rt.z - _ray.first.z) * dirfrac.z;

						float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
						float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));



						if (tmin > tmax)
						{
							intersect = false;
							continue;
						}


						SendSelectionIDEvent sendEvent{ entt };
						PE_PUBLISH_EVENT(sendEvent);
						break;

					}


				}



			}

			//check if ImGui::DockNode is interacting

			const ImVec2 vMin = ImGui::GetWindowContentRegionMin();
			const ImVec2 vMax = ImGui::GetWindowContentRegionMax();


			//if (ImGui::IsWindowHovered())
			//{
			//	std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(renderingSystem);
			//	rs->m_persCamera.UpdateZoom();
			//}
			//
			//if (ImGui::IsWindowFocused() && Inputs::GetInstance().IsMouseButtonPressed(Eos::Button::Right))
			//{
			//	Inputs::GetInstance().SetCursorMode(Cursor::Locked);
			//	std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(renderingSystem);
			//	rs->m_persCamera.Update();
			//	//rs->trackball.MoveClick();
			//	//testcamera.updateInput(0.0166f, -1, mouseOffsetX, mouseOffsetY);
			//}
			//
			//std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(renderingSystem);
			//rs->m_persCamera.UpdateZoom();
			////	Inputs::GetInstance().SetCursorMode(Cursor::Locked);
			//rs->m_persCamera.Update();

			ImVec2 windowPos = ImGui::GetWindowPos();
			windowPos.y += ImGui::GetWindowContentRegionMin().y;

			ImVec2 contentRegionSize = ImGui::GetContentRegionAvail();
			if (m_size.x != contentRegionSize.x || m_size.y != contentRegionSize.y ||
				m_position.x != windowPos.x || m_position.y != windowPos.y)
			{
				// Top-Left as origin
				m_size.x = contentRegionSize.x;
				//contentRegionSize.y = contentRegionSize.x * 9.0f / 16.0f; //maintain aspect ratio
				m_size.y = contentRegionSize.y;
				m_position.x = windowPos.x;
				m_position.y = windowPos.y;

				// resize framebuffer to update with the edited position and size of this window
				//Send events at a interval
				ResizeFrameBufferEvent resizeEvn{ { windowPos.x, windowPos.y}, m_size };
				PE_PUBLISH_EVENT(resizeEvn);
			}

			ImGui::Image((void*)(intptr_t)colorBuffer,
				ImVec2(m_size.x, m_size.y),
				ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

			//Gizmo rendering
			bool snap = Inputs::GetInstance().IsKeyPressed(KeyCode::LeftControl);
			float snapValue = 0.25f;
			float snapValues[3] = { snapValue,snapValue,snapValue };

			if (m_selectedEntity != NULLENTITY)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(m_position.x, m_position.y, m_size.x, m_size.y);
				//Need camera
				std::shared_ptr<Rendering_System>  rs =
					std::dynamic_pointer_cast<Rendering_System>(renderingSystem);

				glm::mat4 view = rs->m_persCamera.GetViewMaxtrix();
				glm::mat4 proj = rs->m_persCamera.GetProjectionMatrix();


				if (ComponentManager::GetInstance().EntityHaveComponent(m_selectedEntity, COMPONENTNAME::Comp_Transform_Component))
				{
					std::shared_ptr<Transform_Component> tc = std::dynamic_pointer_cast<Transform_Component>(
						ComponentManager::GetInstance().GetComponent(m_selectedEntity, COMPONENTNAME::Comp_Transform_Component));

					glm::mat4 translation = glm::translate(glm::mat4(1.0f), tc->Translation);
					glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), tc->Rotation.x, glm::vec3(0, 0, 1));
					glm::mat4 scale = glm::scale(glm::mat4(1.0f), tc->Scale);

					glm::vec3 prevTC = tc->Translation;

					auto newTransform = translation * rotation * scale;

					ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj),
						(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(newTransform),
						nullptr, snap ? snapValues : nullptr);

					if (ImGuizmo::IsUsing())
					{
						glm::vec3 m_scale, m_translation, m_skew;
						glm::vec4 perspective;
						glm::quat orientation;


						glm::decompose(newTransform, m_scale, orientation, m_translation, m_skew, perspective);


						//Update transform
						tc->Translation.x = m_translation.x;
						tc->Translation.y = m_translation.y;
						tc->Translation.z = m_translation.z;
					}

				}
			}


			//if (Inputs::GetInstance().IsMouseButtonReleased(Eos::Button::Right))
			//{
			//    Inputs::GetInstance().SetCursorMode(Cursor::Normal);
			//    //std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(renderingSystem);
			//    //rs->trackball.MoveRelease();
			//}



		}
		ImGui::End();
		ImGui::PopStyleVar();

		glUseProgram(0);
		glBindVertexArray(0);
	}
}
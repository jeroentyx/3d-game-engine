#pragma once

#include "InspectorWindow.h"
#include "imgui_stdlib.h"
#include <type_traits>
#include <iostream>

namespace Eos
{
	char entityName[64];
	ImGui::FileBrowser fileDialog;

	void InspectorWindow::resetSelectedEntity()
	{
		m_selectedEntity = NULLENTITY;
	}

	InspectorWindow::InspectorWindow() :
		m_selectedEntity(NULLENTITY)
		, prevSelected(NULLENTITY)
	{
		renderingSystem = SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS);

		PE_SUBSCRIBE_EVENT
		(
			SendSelectionIDEvent,
			[this](SendSelectionIDEvent& _evn)
			{
				prevSelected = m_selectedEntity;
				m_selectedEntity = _evn.selectedEntity;
			}
		);
	}
	void InspectorWindow::Update()
	{
		ImGui::Begin("Inspector", &active);

		using ComponentMap = std::multimap<EntityID, std::shared_ptr<IComponent>>;
		ComponentMap mapOfComponents = GET_ALL_COMPONENTS_MAP;

		if (mapOfComponents.size() == 0)
		{
			m_selectedEntity = NULLENTITY;
		}

		auto& compManager = ComponentManager::GetInstance();

		std::vector<std::shared_ptr<IComponent>>selectedEntityComps;

		for (auto temp = mapOfComponents.begin(); temp != mapOfComponents.end(); temp++)
		{
			if (temp->first == m_selectedEntity)
			{
				selectedEntityComps.push_back(temp->second);
			}
		}

		if (m_selectedEntity != NULLENTITY)
		{
			if (prevSelected != m_selectedEntity)
			{
				prevSelected = m_selectedEntity;
				strcpy_s(entityName, sizeof(entityName), EntityManager::GetInstance().getEntityName(m_selectedEntity).c_str());
				item_current_idx = 0;
			}
			ImGui::InputText("Entity Name", entityName, IM_ARRAYSIZE(entityName));
			if (ImGui::Button("Rename"))
			{
				EntityManager::GetInstance().setEntityName(m_selectedEntity, std::string(entityName));
			}
			bool CurrActive = GET_ENTITY_MANAGER.getActive(m_selectedEntity);
			bool PrevActive = CurrActive;
			ImGui::Checkbox("Active", &CurrActive);
			if (CurrActive != PrevActive)
			{
				GET_ENTITY_MANAGER.setActive(m_selectedEntity, CurrActive);
			}
			std::string displayText2("EntityID: ");
			displayText2 += std::to_string(m_selectedEntity._id);
			std::string displayText3("Entity Generations: ");
			displayText3 += std::to_string(m_selectedEntity._generations);
			ImGui::Text(displayText2.c_str());
			ImGui::Text(displayText3.c_str());
			ImGui::Separator();
			std::string parentText("Parent: ");
			EntityID parent = GET_ENTITY_MANAGER.getParentOfChild(m_selectedEntity);
			if (parent == NULLENTITY)
			{
				parentText += "NIL";
			}
			else
			{
				parentText += GET_ENTITY_MANAGER.getEntityName(parent);
				parentText += " || " + std::to_string(parent._id);
				parentText += "." + std::to_string(parent._generations);
			}
			if (ImGui::CollapsingHeader(parentText.c_str()))
			{
				std::vector<EntityID> selectableEntities;
				std::vector<std::string> selectableEntityNames;
				selectableEntities.emplace_back(NULLENTITY);
				selectableEntityNames.emplace_back("No Parent");

				std::vector<EntityID> children = GET_ENTITY_MANAGER.findAllChild(m_selectedEntity, true);

				for (auto itr = GET_ENTITY_MANAGER.usedIDs.begin(); itr != GET_ENTITY_MANAGER.usedIDs.end(); ++itr)
				{
					bool isChild = false;
					bool isCurrParent = false;
					for (int i = 0; i < children.size(); ++i)
					{
						if (itr->first == children[i])
						{
							isChild = true;
							break;
						}
					}
					if (itr->first == GET_ENTITY_MANAGER.getParentOfChild(m_selectedEntity))
					{
						isCurrParent = true;
					}
					if ((itr->first != m_selectedEntity) && (isChild == false) && (isCurrParent == false))
					{
						selectableEntities.emplace_back(itr->first);
						selectableEntityNames.emplace_back(itr->second);
					}
				}
				//static int item_current_idx = 0; // Here we store our selection data as an index.
				if (ImGui::BeginCombo(" ", selectableEntityNames[item_current_idx].c_str()))
				{

					for (int n = 0; n < selectableEntityNames.size(); n++)
					{
						const bool is_selected = (item_current_idx == n);
						if (ImGui::Selectable(selectableEntityNames[n].c_str(), is_selected))
							item_current_idx = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				if (ImGui::Button("Change Parent"))
				{
					GET_ENTITY_MANAGER.attachChildtoParent(selectableEntities[item_current_idx], m_selectedEntity);
					item_current_idx = 0;
				}
			}
			//ImGui::NextColumn();
			ImGui::Separator();
		}


		for (auto& E : selectedEntityComps)
		{
			int shadertype = 0;
			int meshtype = 0;


			std::shared_ptr<Rendering_System>  rs =
				std::dynamic_pointer_cast<Rendering_System>(renderingSystem);
			std::map<EntityID, std::shared_ptr<Mesh_Component>> meshMap = rs->GetMeshesMap();

			if (!meshMap.empty())
			{
				if (meshMap.contains(m_selectedEntity))
				{
					shadertype = meshMap[m_selectedEntity]->type;
					meshtype = meshMap[m_selectedEntity]->sType;
				}
			}

			std::string ComponentName;
			ComponentName = compManager.COMPNAMEtoString(E->componentType);

			if (ImGui::TreeNode(ComponentName.data()))
			{
				property::entry PropEntry;

				property::DisplayEnum(*E, [&](std::string_view PropertyName, property::data&& Data, const property::table&, std::size_t, property::flags::type Flags)
				{
					UNREFERENCED_PARAMETER(Flags);
					//Getting the properties name

					std::string Property;

					if (PropertyName.find('/') != std::string::npos)
					{
						Property = PropertyName.substr(PropertyName.find_last_of('/') + 1);
					}

					std::visit([&](auto&& Value)
					{
						PropEntry.first = PropertyName;

						using T = std::decay_t<decltype(Value)>;

						if constexpr (std::is_same_v<T, int>)
						{
							T var = Value;
							ImGui::DragInt(Property.c_str(), &var, 0.01f);
							PropEntry.second = var;
						}
						else if constexpr (std::is_same_v<T, float>)
						{
							T var = Value;
							ImGui::DragFloat(Property.c_str(), &var, 0.01f);
							PropEntry.second = var;
						}
						else if constexpr (std::is_same_v<T, uint32_t>)
						{

						}
						else if constexpr (std::is_same_v<T, bool>)
						{
							T check = Value;
							ImGui::Checkbox(Property.c_str(), &check);
							PropEntry.second = check;
						}
						else if constexpr (std::is_same_v<T, string_t>)
						{
							std::string name = Value;
							if (E->componentType == COMPONENTNAME::Comp_Static_Mesh_Component)
							{
								// If import button is clicked
								if (ImGui::Button("Import"))
								{
									//std::optional< std::string> m_filepath;
									//m_filepath = DialogOpenFile(L"Import Static Mesh", L"../Assets/Models/");
									//if (m_filepath.has_value())
									//{
									//	auto mesh_comp_ptr = std::dynamic_pointer_cast<Static_Mesh_Component>(E);
									//	mesh_comp_ptr->LoadModelAndShader(m_filepath.value());
									//	name = Eos::FileManager::GetFileNameWithExtension(m_filepath.value());
									//}
									//else
									//{
									//	PE_CORE_WARN("Cannot Import Static Mesh");
									//}
								}

								ImGui::InputText("Current Model", &name, 64);

								if (ImGui::Button("Clear"))
								{
									name.clear();
								}
							}
							else if (E->componentType == COMPONENTNAME::Comp_Script_Component)
							{

								if (ImGui::Button("Import"))
								{
									std::optional< std::string> m_filepath;
									m_filepath = DialogOpenFile(L"Import Scripts", L"../EosScriptCor/EosScriptCore/Scripts");
									if (m_filepath.has_value())
									{
										name = Eos::FileManager::GetFileNameWithExtension(m_filepath.value());
										auto script = std::dynamic_pointer_cast<Script_Component>(E);
										script->ModuleName = name;
										ScriptSystem::InitScriptEntity(m_selectedEntity);
									}
									else
									{
										PE_CORE_WARN("Cannot Import Scripts");
									}
								}

								// To clear image
								if (ImGui::Button("Clear"))
								{
									name.clear();
								}

								ImGui::InputText("Script Selected", &name, 64);
							}
							else if (E->componentType == COMPONENTNAME::Comp_Animated_Mesh_Component)
							{
								static std::string inputName;

								ImGui::InputText("Mesh Name", &inputName, 64);

								if (ImGui::Button("Set"))
								{
									name = inputName;
									inputName.clear();
								}
							}
							else if (E->componentType == COMPONENTNAME::Comp_GUI_Component)
							{
								if (ImGui::Button("Import"))
								{
									std::optional< std::string> m_filepath;
									m_filepath = DialogOpenFile(L"Import GUI", L"../Assets/Textures/");
									if (m_filepath.has_value())
									{
										auto ui_sprite = std::dynamic_pointer_cast<GUI_Component>(E);
										ui_sprite->SetFilePath(name);
										name = Eos::FileManager::GetFileNameWithExtension(m_filepath.value());
									}
									else
									{
										PE_CORE_WARN("Cannot Import GUI");
									}
								}
								// To clear image
								if (ImGui::Button("Clear"))
								{
									name.clear();
								}

								ImGui::InputText("Texture Selected", name.data(), 64);
							}
							else if (E->componentType == COMPONENTNAME::Comp_Particle_Component)
							{
								if (ImGui::Button("Import Texture"))
								{
									std::optional<std::string> m_filePath;
									m_filePath = DialogOpenFile(L"Import GUI", L"../Assets/Textures/");
									if (m_filePath.has_value())
									{
										name = m_filePath.value();
										auto particleComponent = std::dynamic_pointer_cast<Particle_Component>(E);
										particleComponent->SetFilePath(name);
										name = Eos::FileManager::GetFileNameWithExtension(m_filePath.value());
									}
									else
									{
										PE_CORE_WARN("Error loading texture! ,ParticleComponent can only load PNG");
									}
								}

							}
							else if (E->componentType == COMPONENTNAME::Comp_Smoke_Component)
							{
								if (ImGui::Button("Import Texture"))
								{
									std::optional<std::string> m_filePath;
									m_filePath = DialogOpenFile(L"Import GUI", L"../Assets/Textures/");
									if (m_filePath.has_value())
									{

										name = m_filePath.value();
										auto smokeParticle = std::dynamic_pointer_cast<Smoke_Component>(E);
										smokeParticle->SetFilePath(m_filePath.value());
										name = Eos::FileManager::GetFileNameWithExtension(m_filePath.value());
									}
									else
									{
										PE_CORE_WARN("Error loading texture! ,ParticleComponent can only load PNG");
									}
								}
							}
							else if (E->componentType == COMPONENTNAME::Comp_BillBoard_Component)
							{
								ImGui::InputText(Property.c_str(), &name, 64);

								if (ImGui::Button("Set"))
								{
									auto billboard = std::dynamic_pointer_cast<BillBoard_Component>(E);
									billboard->isSet = true;
								}
							}
							else
							{
								ImGui::InputText(Property.c_str(), &name, 64);
							}


							// Set properties to the new variable
							PropEntry.second = name;
						}
						else if constexpr (std::is_same_v<T, oobb>)
						{

						}
						else if constexpr (std::is_same_v<T, glm::vec2>)
						{
							glm::vec2 size = Value;
							ImGui::DragFloat2("Size", &size.x);
							PropEntry.second = size;
						}
						else if constexpr (std::is_same_v<T, glm::vec3>)
						{

							if (E->componentType == COMPONENTNAME::Comp_Light_Component)
							{
								glm::vec3 color = Value;
								ImGui::ColorEdit3("Color", &color.x);
								PropEntry.second = color;
							}
							else
							{
								glm::vec3 vector = Value;
								ImGui::DragFloat3(Property.c_str(), &vector.x, 0.01f);
								PropEntry.second = vector;
							}
						}
						else if constexpr (std::is_same_v<T, script_ref>)
						{

							script_ref script = Value;
							ImGui::Text("Scripting Values");
							ImGui::Separator();

							auto data = std::dynamic_pointer_cast<Script_Component>(E);


							if (ScriptSystem::ModuleExists(data->GetModuleName()))
							{


								EntityInstanceData& entityInstanceData = ScriptSystem::GetEntityInstanceData(m_selectedEntity);
								ScriptModuleField& moduleFieldMap = entityInstanceData.m_modulefieldmap;

								if (moduleFieldMap.find(data->GetModuleName()) != moduleFieldMap.end())
								{
									std::unordered_map<std::string, PublicField>& publicFields = moduleFieldMap.at(data->GetModuleName());
									for (auto& [name, field] : publicFields)
									{
										bool isRuntime = (CoreManager::GetInstance().getEngineState() == ENGINESTATE::PLAY) && field.IsRunTimeAvaliable();
										switch (field.m_Type)
										{
										case FieldType::BOOL:
										{
											bool value = isRuntime ? field.GetRuntimeValue<bool>() : field.GetStoredValue<bool>();
											if (ImGui::Checkbox(field.m_name.c_str(), &value))
											{
												if (isRuntime)
													field.SetRuntimeValue(value);
												else
													field.SetStoredValue(value);
											}
											break;
										}
										case FieldType::INT:
										{
											int value = isRuntime ? field.GetRuntimeValue<int>() : field.GetStoredValue<int>();
											if (ImGui::DragInt(field.m_name.c_str(), &value))
											{
												if (isRuntime)
													field.SetRuntimeValue(value);
												else
													field.SetStoredValue(value);
											}
											break;
										}
										case FieldType::FLOAT:
										{
											float value = isRuntime ? field.GetRuntimeValue<float>() : field.GetStoredValue<float>();
											if (ImGui::DragFloat(field.m_name.c_str(), &value))
											{
												if (isRuntime)
												{
													field.SetRuntimeValue(value);
												}
												else
												{
													field.SetStoredValue(value);
												}
											}
											break;
										}
										case FieldType::UINT:
										{
											int value = isRuntime ? field.GetRuntimeValue<int>() : field.GetStoredValue<int>();
											if (ImGui::DragInt(field.m_name.c_str(), &value))
											{
												if (isRuntime)
													field.SetRuntimeValue(value);
												else
												{
													field.SetStoredValue(value);
												}
											}
											break;
										}
										case FieldType::STRING:
										{
											std::string value = isRuntime ? field.GetRuntimeValue<std::string>() : field.GetStoredValue<std::string>();
											if (ImGui::InputText(Property.c_str(), &value, 64))
											{
												if (isRuntime)
													field.SetRuntimeValue(value);
												else
													field.SetStoredValue(value);
											}
											break;
										}
										case FieldType::VEC3:
										{
											glm::vec3 value = isRuntime ? field.GetRuntimeValue<glm::vec3>() : field.GetStoredValue<glm::vec3>();
											if (ImGui::DragFloat3(field.m_name.c_str(), &value.x))
											{
												if (isRuntime)
													field.SetRuntimeValue(value);
												else
													field.SetStoredValue(value);
											}
											break;
										}
										default:
											break;
										}
									}
								}
							}
							ImGui::InputText("Script Selected", &script.Modulename, 64);

						}
						else if constexpr (std::is_same_v<T, MeshType>)
						{
							meshtype = Value;
							ImGui::Combo("Mesh :", &meshtype,
								"sphere.obj\0"
								"cube.obj\0" // 0- sphere  // 1 - cube
								"None\0");

							PropEntry.second = meshtype;
						}
						else if constexpr (std::is_same_v<T, ShaderType>)
						{
							shadertype = Value;
							ImGui::Combo("Shader :", &shadertype,
								"Phong Shading\0"
								"Default\0" // 0- sphere  // 1 - cube
								"None\0");
							PropEntry.second = shadertype;
						}
						else if constexpr (std::is_same_v<T, glm::mat4>)
						{

						}
						else if constexpr (std::is_same_v<T, A_State>)
						{
							auto animComp = std::dynamic_pointer_cast<Animated_Mesh_Component>(E);

							static std::string stateNameInput;
							ImGui::InputText("Name", &stateNameInput, 64);
							static int item_current_2 = 0;
							ImGui::Combo("Mode", &item_current_2, "Loop\0Single\0\0");
							static std::string AnimTracknameInput;
							ImGui::InputText("Track", &AnimTracknameInput, 64);

							if (ImGui::Button("Add State"))
							{
								animComp->list_animations.push_back({ stateNameInput, (AnimationWrapMode)item_current_2, AnimTracknameInput });

								// Clear the entry
								stateNameInput.clear();
								item_current_2 = 0;
								AnimTracknameInput.clear();
							}
									
							ImGui::NewLine();
							ImGui::Text("List of current states:");

							// Display all existing states
							for (int row = 0; row < animComp->list_animations.size(); row++)
							{
								//ImGui::InputText("", &std::get<0>(animComp->list_animations[row]));
								ImGui::Text(animComp->list_animations[row].stateName.c_str());
								ImGui::SameLine();

								if (animComp->list_animations[row].mode == AnimationWrapMode::Loop)
									ImGui::Text("Loop");
								else if (animComp->list_animations[row].mode == AnimationWrapMode::Single)
									ImGui::Text("single");

								ImGui::SameLine();

								ImGui::Text(animComp->list_animations[row].animTrackName.c_str());
							}

							if (ImGui::Button("Remove State"))
							{
								animComp->list_animations.pop_back();
							}
							ImGui::SameLine();
							if (ImGui::Button("Clear States"))
							{
								animComp->list_animations.clear();
							}
						}
						else static_assert(always_false<T>::value, "No Such Property Value!");

						if (E->componentType == COMPONENTNAME::Comp_Physics_Component)
						{
							auto phycomp = std::dynamic_pointer_cast<Physics_Component>(E);
							//phycomp->compToBody();
							phycomp->compToBody2();
						}

						property::set(*E, PropEntry.first.c_str(), PropEntry.second);
					}
					, Data);

				});

				ImGui::TreePop();
			}



			ImGui::Separator();

			if (meshMap.contains(m_selectedEntity))
			{
				std::shared_ptr<Mesh_Component> mc = meshMap[m_selectedEntity];

				if (meshtype == MeshType::SPHERE)
				{
					//mc->type = MeshType::SPHERE;
					Test::Model sphere = EosEditorAssets::GetInstance().GetModel("sphere");
					mc->m_model = sphere;
					Test::Shader shader = EosEditorAssets::GetInstance().GetShader("cube");
					mc->m_shader = shader;
					mc->type = MeshType::SPHERE;
				}
				if (meshtype == MeshType::CUBE)
				{
					Test::Model cube = EosEditorAssets::GetInstance().GetModel("cube");
					mc->m_model = cube;
					Test::Shader shader = EosEditorAssets::GetInstance().GetShader("cube");
					mc->m_shader = shader;
					mc->type = MeshType::CUBE;

				}

				//TODO: let a resourceManager holds all the data for meshes and texture then get from there
				if (shadertype == ShaderType::DEFAULT)
				{
					Test::Shader shader = EosEditorAssets::GetInstance().GetShader("cube");
					mc->m_shader = shader;
					mc->sType = ShaderType::DEFAULT;
					rs->normal = true;
					rs->blinnPhong = false;
					rs->shadow = false;
				}
				if (shadertype == ShaderType::PHONG)
				{
					Test::Shader shader = EosEditorAssets::GetInstance().GetShader("phong");
					mc->m_shader = shader;
					mc->sType = ShaderType::PHONG;
					rs->normal = false;
					rs->blinnPhong = true;
					rs->shadow = false;
				}
			}

		}



		if (m_selectedEntity != NULLENTITY)
		{
			if (ImGui::Button("Add Component", ImVec2(300.0f, 20.0f)))
			{
				ImGui::OpenPopup("List of Components");
			}
			if (ImGui::BeginPopup("List of Components"))
			{
				ImGui::Separator();
				if (ImGui::Selectable("TransformComponent"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Transform_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Transform_Component);
					}
				}
				if (ImGui::Selectable("MeshComponent"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Mesh_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Mesh_Component);
					}
				}
				if (ImGui::Selectable("StaticMeshComponent"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Static_Mesh_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Static_Mesh_Component);
					}
				}
				if (ImGui::Selectable("AnimatedMeshComponent"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Animated_Mesh_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Animated_Mesh_Component);
					}
				}
				if (ImGui::Selectable("LightComponent"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Light_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Light_Component);
					}
				}
				if (ImGui::Selectable("PhysicsComponent"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Physics_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Physics_Component);
					}
				}
				if (ImGui::Selectable("ScriptComponent"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Script_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Script_Component);
					}
				}
				if (ImGui::Selectable("CameraComponent"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Camera_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Camera_Component);
					}
				}
				if (ImGui::Selectable("AudioComponent"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Audio_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Audio_Component);
					}
				}
				if (ImGui::Selectable("GUI Component"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_GUI_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_GUI_Component);
					}
				}
				if (ImGui::Selectable("Moving Skybox"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Skybox_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Skybox_Component);
					}
				}
				if (ImGui::Selectable("Text Component"))
                {
                    if (!ComponentManager::GetInstance().EntityHaveComponent(
                        m_selectedEntity, COMPONENTNAME::Comp_Skybox_Component))
                    {
                        ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Font_Component);
					}
				}
				if (ImGui::Selectable("Particle Component"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Particle_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Particle_Component);
					}
				}
				if (ImGui::Selectable("Smoke Component"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Smoke_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_Smoke_Component);
					}
				}
				if (ImGui::Selectable("Billboard Component"))
				{
					if (!ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_BillBoard_Component))
					{
						ComponentManager::GetInstance().AddComponent(m_selectedEntity, COMPONENTNAME::Comp_BillBoard_Component);
					}
				}
				ImGui::EndPopup();
			}
		}

		if (m_selectedEntity != NULLENTITY)
		{
			if (ImGui::Button("Delete Component", ImVec2(300.0f, 20.0f)))
			{
				ImGui::OpenPopup("Components");
			}
			if (ImGui::BeginPopup("Components"))
			{
				ImGui::Separator();

				if (ImGui::Selectable("TransformComponent"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Transform_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Transform_Component);
					}
				}
				if (ImGui::Selectable("MeshComponent"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Mesh_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Mesh_Component);
					}
				}
				if (ImGui::Selectable("StaticMeshComponent"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Static_Mesh_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Static_Mesh_Component);
					}
				}
				if (ImGui::Selectable("AnimatedMeshComponent"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Animated_Mesh_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Animated_Mesh_Component);
					}
				}
				if (ImGui::Selectable("LightComponent"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Light_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Light_Component);
					}
				}
				if (ImGui::Selectable("PhysicsComponent"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Physics_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Physics_Component);
					}
				}
				if (ImGui::Selectable("ScriptComponent"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Script_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Script_Component);
					}
				}
				if (ImGui::Selectable("AudioComponent"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Audio_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Audio_Component);
					}
				}
				if (ImGui::Selectable("CameraComponent"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Camera_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Camera_Component);
					}
				}
				if (ImGui::Selectable("GUI Component"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_GUI_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_GUI_Component);
					}
				}
				if (ImGui::Selectable("Moving Skybox"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Skybox_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Skybox_Component);
					}
				}
				if (ImGui::Selectable("Text Component"))
                {
                    if (ComponentManager::GetInstance().EntityHaveComponent(
                        m_selectedEntity, COMPONENTNAME::Comp_Font_Component))
                    {
                        ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Font_Component);
                    }
                }
				if (ImGui::Selectable("ParticleComponent"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Particle_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Particle_Component);
					}
				}
				if (ImGui::Selectable("SmokeComponent"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_Smoke_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_Smoke_Component);
					}
				}
				if (ImGui::Selectable("Billboard Component"))
				{
					if (ComponentManager::GetInstance().EntityHaveComponent(
						m_selectedEntity, COMPONENTNAME::Comp_BillBoard_Component))
					{
						ComponentManager::GetInstance().RemoveComponent(m_selectedEntity, COMPONENTNAME::Comp_BillBoard_Component);
					}
				}
				ImGui::EndPopup();
			}

		}

		ImGui::End();

	}
}

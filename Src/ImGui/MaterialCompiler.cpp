#include "MaterialCompiler.h"
#pragma once

namespace Eos
{
	MaterialCompiler::MaterialCompiler()
	{
	}

	void MaterialCompiler::Update()
	{
		ImGui::Begin("Material Editor", &active, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_HorizontalScrollbar);
		
		// Push button green
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));

		if (ImGui::Button("Save to file"))
		{
			// Do something to save to file
		}
		ImGui::SameLine();

		// Remove green
		ImGui::PopStyleColor(3);

		// Push red
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));

		if (ImGui::Button("Reset to default"))
		{
			// Do something
		}

		ImGui::PopStyleColor(3);
		ImGui::Separator();

		ImGui::Text("Material Compiler");
			


		ImGui::Combo("Material :", &materialFlag,
			"Default1.eos\0"
			"Material2.eos\0"); // 0- sphere  // 1 - cube

		/*ImGui::SetCursorPosY(ImGui::GetCursorPosY()
			+ ImGui::GetScrollY() + 0.5 * ImGui::GetStyle().ItemSpacing.y);*/

		ImGui::Combo("Shader :", &shaderFlag,
			"lit.glsl\0"
			"mesh.glsl\0"); // 0- sphere  // 1 - cube

		/*ImGui::SetCursorPosY(ImGui::GetCursorPosY()
			+ ImGui::GetScrollY() + 2 * ImGui::GetStyle().ItemSpacing.y);
		ImGui::Text("Material Setting");
		ImGui::Checkbox("\tBlendable", &blendable);
		ImGui::Checkbox("\tBack-face culling", &backfaceCulling);
		ImGui::Checkbox("\tFront-face culling", &frontFaceCulling);
		ImGui::Checkbox("\tDepth Test", &DepthTest);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY()
			+ ImGui::GetScrollY() + 2 * ImGui::GetStyle().ItemSpacing.y);*/

		// Material settings
		if (ImGui::CollapsingHeader("Material Settings"))
		{
			if (ImGui::BeginTable("Table", 2))
			{
				ImGui::TableNextColumn(); ImGui::Text("Blendable");
				ImGui::TableNextColumn(); ImGui::Checkbox("", &blendable);

				ImGui::TableNextColumn(); ImGui::Text("Back-face culling");
				ImGui::TableNextColumn(); ImGui::Checkbox("", &backfaceCulling);

				ImGui::TableNextColumn(); ImGui::Text("Front-face culling");
				ImGui::TableNextColumn(); ImGui::Checkbox("", &frontFaceCulling);

				ImGui::TableNextColumn(); ImGui::Text("Depth Test");
				ImGui::TableNextColumn(); ImGui::Checkbox("", &DepthTest);

				// Stop the table
				ImGui::EndTable();
			}
		}

		// Below onwards is shader settings
		// Classified under one collapsing header
		if (ImGui::CollapsingHeader("Shader Settings"))
		{
			// Push red
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));

			// Diffuse map settings
			{
				ImGui::Text("Diffuse Map");
				ImGui::SameLine();

				//ImGui::ImageButton();
				//ImGui::SameLine();

				if (ImGui::Button("Clear"))
				{
					// Action when clear button is clicked
				}

				ImGui::NewLine();
			}

			// Normal map settings
			{
				ImGui::Text("Normal map");
				ImGui::SameLine();

				//ImGui::ImageButton();
				//ImGui::SameLine();

				if (ImGui::Button("Clear"))
				{
					// Action when clear button is clicked
				}

				ImGui::NewLine();
			}

			// Specular map settings
			{
				ImGui::Text("Specular map");
				ImGui::SameLine();

				//ImGui::ImageButton();
				//ImGui::SameLine();

				if (ImGui::Button("Clear"))
				{
					// Action when clear button is clicked
				}

				ImGui::NewLine();
			}

			// Remove color style of clear button
			ImGui::PopStyleColor(3);


			// Diffuse slider bar
			{
				ImGui::Text("Diffuse");
				ImGui::SameLine();
					
				if (enableXYZWDiffuse)
					ImGui::DragFloat4("", diffuseMeasurement, 0.01f);
				if (enableRGBDiffuse)
					ImGui::ColorEdit3("", colorDiffuse);

				ImGui::SameLine();
				if (ImGui::Button("XYZW"))
				{
					enableXYZWDiffuse = true;
					enableRGBDiffuse = false;
				}
				ImGui::SameLine();
				if (ImGui::Button("RGB"))
				{
					enableXYZWDiffuse = false;
					enableRGBDiffuse = true;
				}
			}

			// Specular
			{
				ImGui::Text("Specular");
				ImGui::SameLine();

				if (enableXYZSpecular)
					ImGui::DragFloat3("", specularXYZ, 0.01f);
				if (enableRGBSpecular)
					ImGui::ColorEdit3("", specularRGB);

				ImGui::SameLine();
				if (ImGui::Button("XYZ"))
				{
					enableXYZSpecular = true;
					enableRGBSpecular = false;
				}
				ImGui::SameLine();
				if (ImGui::Button("RGB"))
				{
					enableXYZSpecular = false;
					enableRGBSpecular = true;
				}
			}

			// Texture Offset bar
			{
				ImGui::Text("Texture Offset");
				ImGui::SameLine();
				ImGui::DragFloat2("", textOffset, 0.01f);
			}

			// Texture tiling
			{
				ImGui::Text("Texture Tiling");
				ImGui::SameLine();
				ImGui::DragFloat2("", textureTiling, 0.01f);
			}

			// Enable / Disable noral mapping
			{
				ImGui::Text("Enable Normal Mapping");
				ImGui::SameLine();
				ImGui::Checkbox("", &enableNormal);
			}

		} // End of collapsing header


		//ImGui::Text("Shader Setting");

		//static int diffuseSelection = -1;
		//static int normalSelection = -1;
		//static int specularSelection = -1;

		//ImGui::Combo("DiffuseMap:", &diffuseSelection,
		//	"Bricks\0"
		//	"Concrete\0"
		//	"None\0");
		//ImGui::Combo("NormalMap:", &normalSelection,
		//	"Bricks\0"
		//	"Concrete\0"
		//	"None\0");

		//ImGui::Combo("SpecularMap:", &specularSelection,
		//	"Bricks\0"
		//	"Concrete\0"
		//	"None\0");
		//static bool enableNormal = false;
		//static bool linearColor = false, CPUcolor = false;
		//glm::vec4 diffuse(1.0f);
		//glm::vec3 specular(1.0f);
		//glm::vec2 offset(1.0f);

		////Placeholders for now
		//ImGui::DragFloat4("Diffuse", &diffuse.x, 0.01f);

		//linearColor = ImGui::Button("XYZW");
		//CPUcolor = ImGui::Button("RGB");
		//ImGui::DragFloat3("Specular", &specular.x, 0.01f);
		/*ImGui::DragFloat2("Texture Offset", &offset.x, 0.01f);

		ImGui::Checkbox("Enable Normal Mapping", &enableNormal);*/
		
		ImGui::End();
	}
}

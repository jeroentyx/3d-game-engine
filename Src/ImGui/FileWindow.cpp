#include "FileWindow.h"
#include "Src/Engine/Systems/Graphics/Compiler/AnimationCompiler.h"
#include "Src/Engine/Core/Manager/AssetManager.h"

#include "Src/Events/Events.h"

static std::filesystem::path s_AssetsPath = "Assets";

namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}
}

Eos::FileWindow::FileWindow() :
	path{ "Assets" },
	m_CurrentDirectory{ s_AssetsPath }
{

	m_DirectoryIcon = Texture2D::Create("Buttons/FolderIcon.png");
	m_FileIcon = Texture2D::Create("Buttons/FileIcon_General.png");
	m_SkeletonIcon = Texture2D::Create("Buttons/skull2.png");
	m_MeshIcon = Texture2D::Create("Buttons/mesh.png");
	m_AnimationIcon = Texture2D::Create("Buttons/Animation.png");
	m_ScriptIcon = Texture2D::Create("Buttons/Cscript.png");
}

void Eos::FileWindow::Update()
{

	m_ComboImGui.clear();


	ImGui::Begin("Assets", &active);


	// Going back functionality
	if (m_CurrentDirectory != std::filesystem::path(s_AssetsPath))
	{
		if (ImGui::Button("<-"))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}
	}

	ImGui::SameLine();

	// Button for Importing Textures / Static Meshes / Animations / Fonts
	if (ImGui::Button("Import Assets"))
	{
		std::optional<std::string> m_filepath;
		m_filepath = DialogOpenFile(L"Import from...", L"Assets/");

		// Valid path has been chosen
		if (m_filepath.has_value())
		{
			std::string fileName = FileManager::GetFileNameWithoutExtension(m_filepath.value());
			std::string extension = FileManager::GetExtention(m_filepath.value());

			// Run compiler for corresponding filetype
			// Fonts - font compiler
			// Animated mesh - animation compiler
			if (extension == ".fbx")
			{
				// TODO: open up mini window and enable user to choose options on what details to import
				AnimationCompiler::readFBX_to_ownFormat(m_filepath.value());

				std::string meshPath = "Assets/Animations/Imports/" + fileName + ".eosmesh";
				std::string skeletonPath = "Assets/Animations/Imports/" + fileName + ".eosskeleton";
				std::string animationPath = "Assets/Animations/Imports/" + fileName + ".eosanimation";
				AnimationCompiler::readOwnFormat(meshPath, skeletonPath, animationPath);

				PE_CORE_TRACE("Loading and saving completed");
			}
			else if (extension == ".ttf")
			{
				// Font compiler stuff
			}
			else if (extension == ".png")
			{
				// Copy file from source to destination path
				std::string filenameext = FileManager::GetFileNameWithExtension(m_filepath.value());
				std::string destpath = "Assets/Textures/Billboard/" + filenameext;
				std::ifstream src(m_filepath.value().c_str(), std::ios::binary);
				std::ofstream dest(destpath, std::ios::binary);
				dest << src.rdbuf();

				std::shared_ptr<Texture2D> tex = Texture2D::Create(m_filepath.value());
				Eos::AssetManager::GetInstance().StoreTexture(fileName.c_str(), tex);
			}
		}
		else // invalid path selected
		{
			PE_CORE_WARN("cannot Import Asset");
		}
	}

	ImGui::SameLine();

	// Button to move to asset folder
	if (ImGui::Button("Assets Path"))
	{
		s_AssetsPath = "Assets";
		m_CurrentDirectory = "Assets";
	}

	ImGui::SameLine();

	// Button for importing scripts
	if (ImGui::Button("Scripts Path"))
	{
		s_AssetsPath = "EosScriptCore/EosScriptCore/Scripts";
		m_CurrentDirectory = "EosScriptCore/EosScriptCore/Scripts";
	}

	ImGui::NewLine();
	{
		std::string number = std::to_string(Eos::AssetManager::GetInstance().No_StoredMesh());
		ImGui::Text("Meshes Loaded: ");
		ImGui::SameLine();
		ImGui::Text(number.c_str());
		ImGui::SameLine();

		number = std::to_string(Eos::AssetManager::GetInstance().No_StoredSkeleton());
		ImGui::Text("Skeletons Loaded: ");
		ImGui::SameLine();
		ImGui::Text(number.c_str());
		ImGui::SameLine();

		number = std::to_string(Eos::AssetManager::GetInstance().No_StoredAnimation());
		ImGui::Text("Animations Loaded: ");
		ImGui::SameLine();
		ImGui::Text(number.c_str());
		ImGui::SameLine();

		number = std::to_string(Eos::AssetManager::GetInstance().No_StoredStaticMeshes());
		ImGui::Text("Static Meshes Loaded: ");
		ImGui::SameLine();
		ImGui::Text(number.c_str());
	}
	ImGui::NewLine();

	float padding = 16.0f;
	float thumbnailSize = 64.0f;
	float cellSize = thumbnailSize + padding;
	float panelWidth = ImGui::GetContentRegionAvail().x;

	// Calculating number of items in the columns
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	// Going through each item in the current directory
	// Can be folder or files
	for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		auto& _path = directoryEntry.path();
		auto relativePath = std::filesystem::relative(_path, s_AssetsPath);
		std::string filenameString = relativePath.filename().string();
		//std::cout << relativePath << std::endl;

		// Display image icons
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		m_ComboImGui.push_back(relativePath.string());

		// if a folder
		if (directoryEntry.is_directory())
		{

			ImGui::ImageButton((ImTextureID)m_DirectoryIcon->GetID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });

		}
		else
		{


			if (relativePath.string().find(".eosmesh") != std::string::npos)
			{
				SetAnimationPlayingEvent animEvent{ Eos::FileManager::GetFileNameWithoutExtension(relativePath.string()) };
				PE_CORE_DEBUG("Set animation playing {}", Eos::FileManager::GetFileNameWithoutExtension(relativePath.string()));
				PE_PUBLISH_EVENT(animEvent);
			}
			else if (relativePath.extension() == ".eosskeleton")
			{
				SetAnimationPlayingEvent animEvent{ Eos::FileManager::GetFileNameWithoutExtension(relativePath.string()) };
				PE_CORE_DEBUG("Set animation playing {}", Eos::FileManager::GetFileNameWithoutExtension(relativePath.string()));
				PE_PUBLISH_EVENT(animEvent);
			}
			else if (relativePath.extension() == ".eosanimation")
			{
				SetAnimationPlayingEvent animEvent{ Eos::FileManager::GetFileNameWithoutExtension(relativePath.string()) };
				PE_CORE_DEBUG("Set animation playing {}", Eos::FileManager::GetFileNameWithoutExtension(relativePath.string()));
				PE_PUBLISH_EVENT(animEvent);
			}
			else if (relativePath.extension() == ".cs")
			{
				ImGui::ImageButton((ImTextureID)m_ScriptIcon->GetID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });
			}
			else
			{
				ImGui::ImageButton((ImTextureID)m_FileIcon->GetID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });
			}

		}

		ImGui::PopStyleColor();

		// Navigating through different folders
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
			{
				m_CurrentDirectory /= _path.filename();
			}
		}


		// Show filename under icon
		ImGui::Text(filenameString.c_str());

		// Put next icon on the next column
		ImGui::NextColumn();
	}

	int currIndex = -1;
	if (ImGui::Combo("Select To Show", &currIndex, m_ComboImGui))
	{
		auto relativePathing = m_ComboImGui.at(currIndex);

		if (m_ComboImGui.at(currIndex).find(".eosmesh") != std::string::npos)
		{
			SetAnimationPlayingEvent animEvent{ Eos::FileManager::GetFileNameWithoutExtension(relativePathing) };
			PE_CORE_DEBUG("Set animation playing {}", Eos::FileManager::GetFileNameWithoutExtension(relativePathing));
			PE_PUBLISH_EVENT(animEvent);
		}
		else if (m_ComboImGui.at(currIndex).find(".eosskeleton") != std::string::npos)
		{
			SetAnimationPlayingEvent animEvent{ Eos::FileManager::GetFileNameWithoutExtension(relativePathing) };
			PE_CORE_DEBUG("Set animation playing {}", Eos::FileManager::GetFileNameWithoutExtension(relativePathing));
			PE_PUBLISH_EVENT(animEvent);
		}
		else if (m_ComboImGui.at(currIndex).find(".eosanimation") != std::string::npos)
		{
			SetAnimationPlayingEvent animEvent{ Eos::FileManager::GetFileNameWithoutExtension(relativePathing) };
			PE_CORE_DEBUG("Set animation playing {}", Eos::FileManager::GetFileNameWithoutExtension(relativePathing));
			PE_PUBLISH_EVENT(animEvent);
		}

	}



	ImGui::Columns(1);
	ImGui::End();

}

#pragma once


#include "..\pch.h"
#include "IImGuiWindow.h"
#include "Src\Events\Events.h"
#include "Src\Events\EventDispatcher.h"
#include "Src\Engine\Systems\Graphics\Platform\OpenGLTexture.h"
#include <filesystem>

namespace Eos
{

	class FileWindow : public IImGuiWindow
	{

	public:
		FileWindow();
		virtual void Update() override;



	private:
		std::string path;

		std::filesystem::path m_CurrentDirectory;
		std::shared_ptr<Texture2D> m_DirectoryIcon;
		std::shared_ptr<Texture2D> m_FileIcon;
		std::shared_ptr<Texture2D> m_SkeletonIcon;
		std::shared_ptr<Texture2D> m_MeshIcon;
		std::shared_ptr<Texture2D> m_AnimationIcon;
		std::shared_ptr<Texture2D> m_ScriptIcon;
		std::vector<std::string> m_ComboImGui;
	};
}
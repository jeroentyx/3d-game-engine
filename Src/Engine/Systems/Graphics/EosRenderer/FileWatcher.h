#pragma once

#include "Src\pch.h"
#include <filesystem>

//
//				Editor To check if the directory files has been modified/ added
//

namespace editor
{
	class FileWatcher
	{
		typedef std::string file;

	public:
		FileWatcher(file _path);
		~FileWatcher() = default;
		void Init();
		void OnUpdate();
		void SetActive(bool flag = true) { isDirty = flag; }
	private:
		std::filesystem::path m_Path;
		bool isDirty = true;
		file path = "Assets\\Data";
	};
}
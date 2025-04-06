
#include "FileWatcher.h"

namespace editor
{
	FileWatcher::FileWatcher(file _path)
	{
		path = _path;
	}
	void FileWatcher::Init()
	{
		this->isDirty = true;
	}

	void FileWatcher::OnUpdate()
	{
		if (isDirty)
		{
			//
			//	Iterate through the end of path and show filename
			//
			const std::filesystem::path sandbox{ (const char*)path.c_str() };
			for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ sandbox })
			{
				file name = std::filesystem::path(dir_entry).filename().string();


				//
				//	Only want the filename
				//
				std::size_t strLength = name.size();
				file substr = name.substr(0, strLength);
				std::cout << substr << std::endl;

			}
			isDirty = false;
		}
	}
}
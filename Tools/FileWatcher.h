#pragma once

#include "Src\pch.h"

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>

namespace tools
{
	enum class Status
	{
		CREATED,
		MODIFIED,
		ERASED
	};

	class FileWatcher
	{
	public:
		std::string m_Path;
		std::chrono::duration<int, std::milli> m_Internal; //num of delays

		FileWatcher(
			std::string Path,
			std::chrono::duration<int, std::milli>delay)
			:
			m_Path(Path),
			m_Internal(delay)
		{

			//
			//	Keep a record of files and last modified time
			//
			for (auto& file : std::filesystem::recursive_directory_iterator(Path))
			{
				m_Path[file.path().string()] =
					std::filesystem::last_write_time(file);
			}
		}

		void Start(const std::function<void(std::string, Status)& action)
		{
			
		}


	private:
		

	};
}
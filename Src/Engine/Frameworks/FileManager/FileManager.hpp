#include <filesystem>
#include <string>
#include <fstream>
#include <optional>

namespace Eos
{

	std::optional<std::string>	DialogOpenFolder(std::wstring dialogTitle, std::wstring defaultLocation);
	std::optional<std::string> DialogOpenFile(std::wstring dialogTitle, std::wstring defaultLocation);
	std::string ConvertWStringToString(const std::wstring& wstr);

	class FileManager
	{
	public:
		static std::string GetFileNameWithoutExtension(const std::string& assestFilePath);
		static std::string GetFileNameWithExtension(const std::string& assestFilePath);
		static std::string GetExtention(const std::string& assetFilePath);
		static std::string GetParentPath(const std::string& fullpath);
		static std::string ReadFile(const std::string& path);
		static bool WriteFile(const std::string& filepath, const std::string& text);
		static std::vector<char> ReadBinaryFile(const std::string& filepath);
		static bool Exists(const std::string& path);
	};
}

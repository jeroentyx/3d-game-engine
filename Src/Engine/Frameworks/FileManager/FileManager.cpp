#include "FileManager.hpp"
#include <Windows.h>
#include <shobjidl.h> 
#include <commdlg.h>
#include "Src/Engine/Frameworks/Logger/Logger.h"



namespace Eos
{
	std::string FileManager::GetFileNameWithoutExtension(const std::string& assestFilePath)
	{
		std::string name;
		auto lastSlash = assestFilePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = assestFilePath.rfind('.');
		auto count = lastDot == std::string::npos ? assestFilePath.size() - lastSlash : lastDot - lastSlash;
		name = assestFilePath.substr(lastSlash, count);
		return name;
	}

	std::string FileManager::GetFileNameWithExtension(const std::string& assetFilePath)
	{
		return std::filesystem::path(assetFilePath).filename().string();
	}

	std::string FileManager::GetExtention(const std::string& assetFilePath)
	{
		return std::filesystem::path(assetFilePath).extension().string();
	}

	std::string FileManager::GetParentPath(const std::string& fullpath)
	{
		std::filesystem::path p = fullpath;
		return p.parent_path().string();
	}

	std::string FileManager::ReadFile(const std::string& path)
	{
		std::string result;
		std::ifstream fp(path, std::ios::in | std::ios::binary);
		if (fp)
		{
			fp.seekg(0, std::ios::end);
			result.resize(fp.tellg());
			fp.seekg(0, std::ios::beg);
			fp.read(&result[0], result.size());
		}
		else
		{//Logging
		}

		return result;
	}

	bool FileManager::WriteFile(const std::string& filepath, const std::string& text)
	{
		std::ofstream out(filepath, std::ios::out);
		if (out.good())
		{
			out.write(text.c_str(), text.size());
			return true;
		}
		
		//Logging
		return false;
	}

	std::vector<char> FileManager::ReadBinaryFile(const std::string& filepath)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream) {}
			//Logging
		//Get the position in the input sequence
		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		size_t size = std::size_t(end - stream.tellg());
		if (size == 0) return {};

		std::vector<char> buffer(size);
		if (!stream.read((char*)buffer.data(), buffer.size())) {}
			//Logging

		return buffer;
	}

	bool FileManager::Exists(const std::string& path)
	{
		return std::filesystem::exists(path);
	}

	//Helper Function
	std::optional<std::string> DialogOpenFolder(std::wstring dialogTitle, std::wstring defaultLocation)
	{
		std::optional<std::string> val = std::nullopt;

		// Ref: https://stackoverflow.com/questions/8269696/how-to-use-ifiledialog-with-fos-pickfolder-while-still-displaying-file-names-in
		IFileDialog* pfd{};
		if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
		{
			// Configurations
			DWORD dwOptions;
			if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
			{
				if (SUCCEEDED(pfd->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_NOCHANGEDIR)))
				{
					IShellItem* location{};
					if (SUCCEEDED(SHCreateItemFromParsingName(defaultLocation.c_str(), nullptr, IID_PPV_ARGS(&location))))
					{
						pfd->SetTitle(dialogTitle.c_str()); // Sets the Dialog Title
						pfd->SetFolder(location); // Sets default location
						location->Release();
					}
				}
			}

			// Display the Dialog after configuring
			if (SUCCEEDED(pfd->Show(NULL)))
			{
				IShellItem* psi;
				if (SUCCEEDED(pfd->GetResult(&psi)))
				{
					LPWSTR szTitle{};

					if (SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &szTitle)))
					{
						val = ConvertWStringToString(szTitle);
						CoTaskMemFree(szTitle);
					}
					else
					{
						PE_CORE_ERROR("Failed to open directory!!\n");
					}

					psi->Release();
				}
			}
			pfd->Release();
		}

		return val;
	}

	std::optional<std::string> DialogOpenFile(std::wstring dialogTitle, std::wstring defaultLocation)
	{
		// Ref: https://docs.microsoft.com/en-us/windows/win32/api/commdlg/ns-commdlg-openfilenamea
		// common dialog box structure, setting all fields to 0 is important
		OPENFILENAME ofn{ 0, };
		TCHAR szFile[MAX_PATH] = { 0 };

		// Initialize remaining fields of OPENFILENAME structure
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = GetActiveWindow();
		ofn.lpstrTitle = dialogTitle.c_str();
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = L"*.*\0\0"; // All files - Must be terminated by 2 null characters
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = defaultLocation.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileName(&ofn) == TRUE)
		{
			return ConvertWStringToString(szFile);
		}

		return std::nullopt;
	}

	std::string ConvertWStringToString(const std::wstring& wstr)
	{
		const int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr);
		std::string str(count, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, nullptr, nullptr);
		return str;
	}
}
#pragma once

#include <Windows.h>
#include <string>

namespace Octo {

	struct FileInfo {
		std::string name;
		std::string path;
	};

	class Directories {

	private:

	public:

		void CreateDirectoryIfNotExists(const std::string& path) {
			if (GetFileAttributesA(path.c_str()) == INVALID_FILE_ATTRIBUTES) {
				CreateDirectoryA(path.c_str(), NULL);
			}
		}

		void DeleteDirectoryIfExists(const std::string& path) {
			if (GetFileAttributesA(path.c_str()) != INVALID_FILE_ATTRIBUTES) {
				RemoveDirectoryA(path.c_str());
			}
		}

		std::string GetCurrentDirectory() {
			char buffer[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, buffer);
			return std::string(buffer);
		}

		std::string GetExecutableDirectory() {
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);
			std::string::size_type pos = std::string(buffer).find_last_of("\\/");
			return std::string(buffer).substr(0, pos);
		}

		int CountDirectoriesInPath(const std::string& path) {
			int count = 0;
			std::string::size_type pos = 0;
			while ((pos = path.find("\\", pos)) != std::string::npos) {
				count++;
				pos++;
			}
			return count;
		}

		int CountFilesInDirectory(const std::string& path) {
			int count = 0;
			WIN32_FIND_DATAA findFileData;
			HANDLE hFind = FindFirstFileA((path + "\\*").c_str(), &findFileData);
			if (hFind != INVALID_HANDLE_VALUE) {
				do {
					if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
						count++;
					}
				} while (FindNextFileA(hFind, &findFileData) != 0);
				FindClose(hFind);
			}
			return count;
		}

	};
}
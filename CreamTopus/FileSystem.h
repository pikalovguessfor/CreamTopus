#pragma once

#include <Windows.h>
#include <string>
#include <fstream>

namespace Octo {

	struct FileInfo { // Struct to store information about a file
		std::string name;
		std::string path;
	};

	class Directories { // Class to provide functionalities related to directories

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

	class File { // Class to provide functionalities related to files

	private:

	public:
		void CreateTxTFileIfNotExists(const std::string& path, const std::string& content) {
			if (GetFileAttributesA(path.c_str()) == INVALID_FILE_ATTRIBUTES) {
				std::ofstream file(path);
				if (file.is_open()) {
					file << content;
					file.close();
				}
			}
		}

		void DeleteTxTFileIfExists(const std::string& path) {
			if (GetFileAttributesA(path.c_str()) != INVALID_FILE_ATTRIBUTES) {
				DeleteFileA(path.c_str());
			}
		}

		void WriteToTxTFile(const std::string& path, const std::string& content) {
			std::ofstream file(path, std::ios::out | std::ios::trunc);
			if (file.is_open()) {
				file << content;
				file.close();
			}
		}
	};
}
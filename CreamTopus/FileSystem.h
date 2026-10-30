#pragma once

#include <Windows.h>

class Directories {

private:

public:

	void CreateDirectoryIfNotExists(const std::string& path) {
		if (GetFileAttributesA(path.c_str()) == INVALID_FILE_ATTRIBUTES) {
			CreateDirectoryA(path.c_str(), NULL);
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

};

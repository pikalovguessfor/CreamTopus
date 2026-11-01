#pragma once

#include <string>
#include <Windows.h>
#include <iostream>

#include "OS.h"
#include "_Logger.h"
#include "FileSystem.h"

namespace Octo {

	struct ApplicationInfo { // Struct to store information about an application
		std::string name;
		std::string version;
		std::string publisher;
		std::string installLocation;
		std::string AppId;
	};

	class Steam {
	
	private:



	public:

		std::string GetSteamInstallLocation() {
			// Logic to find the Steam installation directory
			char buffer[MAX_PATH];
			HKEY hKey;
			if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Valve\\Steam", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
				DWORD bufferSize = sizeof(buffer);
				if (RegQueryValueExA(hKey, "InstallPath", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
					RegCloseKey(hKey);
					return std::string(buffer);
				}
				RegCloseKey(hKey);
			}
			return "";
		}
	
	};

}
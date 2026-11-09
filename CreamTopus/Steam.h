#pragma once

#include <string>
#include <Windows.h>
#include <iostream>
#include <vector>

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

		std::string GetSteamInstallLocation() { //Method to get the steam installation location from the registry
			return "";
		}

		std::vector<std::string> GetAllSteamLocationsFromVDF() { //Method to get all steam locations from the local file VDF
			return {};
		}
		
		std::vector<std::string> ParseSteamInstalledGames() { // Method to parse the installed games from the steam installation location
			return {};
		}

		std::vector<std::string> ParseGameAppId(const std::string& gameName) { // Method to parse the app id of the game from the steam installation location
			return {};
		}
	
	};

}
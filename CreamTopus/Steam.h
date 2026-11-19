#pragma once

#include <string>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <cwctype>

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

	class SteamGamesParcer {
	
	private:

		static std::wstring UnescapeVDFString(const std::wstring& input) { //Method to unescape the vdf string
			std::wstring output;
			output.reserve(input.size());
			for (size_t i = 0; i < input.size(); ++i) {
				if (input[i] == L'\\' && i + 1 < input.size()) {
					switch (input[i + 1]) {
					case L'"':
						output += L'"';
						++i;
						break;
					case L'\\':
						output += L'\\';
						++i;
						break;
					default:
						output += input[i];
						break;
					}
				}
				else {
					output += input[i];
				}
			}
			return output;
		}

		static bool getQuotedToken(const std::wstring& str, size_t& pos, std::wstring& token) { //Method to get the quoted token from the vdf string
			token.clear();
			if (pos >= str.size() || str[pos] != L'"') {
				return false;
			}
			++pos; // Skip the opening quote
			while (pos < str.size()) {
				if (str[pos] == L'"') {
					++pos; // Skip the closing quote
					return true;
				}
				else if (str[pos] == L'\\' && pos + 1 < str.size()) {
					switch (str[pos + 1]) {
					case L'"':
						token += L'"';
						pos += 2;
						break;
					case L'\\':
						token += L'\\';
						pos += 2;
						break;
					default:
						token += str[pos];
						++pos;
						break;
					}
				}
				else {
					token += str[pos];
					++pos;
				}
			}
			return false; // No closing quote found
		}
		static bool isLookLikePath(const std::wstring& str) {
			return str.find(L":\\") != std::wstring::npos || str.find(L"/") != std::wstring::npos;
		}
		
		std::wstring SteamInstallLocation{}; // Variable to store the steam installation location

		std::wstring GetSteamInstallLocation() { //Method to get the steam installation location from the registry
			HKEY hKey = nullptr;
			LONG result = RegOpenKeyExW(
				HKEY_LOCAL_MACHINE,
				L"SOFTWARE\\WOW6432Node\\Valve\\Steam",
				0,
				KEY_READ,
				&hKey);

			if (result != ERROR_SUCCESS) {
				result = RegOpenKeyExW(
					HKEY_LOCAL_MACHINE,
					L"SOFTWARE\\Valve\\Steam",
					0,
					KEY_READ,
					&hKey);
			}

			if (result != ERROR_SUCCESS) {
				return L"";
			}

			DWORD type = 0;
			DWORD pathSize = 0;
			result = RegQueryValueExW(hKey, L"InstallPath", nullptr, &type, nullptr, &pathSize);
			if (result != ERROR_SUCCESS || (type != REG_SZ && type != REG_EXPAND_SZ) || pathSize == 0) {
				RegCloseKey(hKey);
				return L"";
			}

			std::wstring steamPath(pathSize / sizeof(wchar_t), L'\0');
			result = RegQueryValueExW(
				hKey,
				L"InstallPath",
				nullptr,
				nullptr,
				reinterpret_cast<LPBYTE>(steamPath.data()),
				&pathSize);
			RegCloseKey(hKey);

			if (result != ERROR_SUCCESS) {
				return L"";
			}

			if (!steamPath.empty() && steamPath.back() == L'\0') {
				steamPath.pop_back();
			}

			return steamPath;
		}

		std::vector<std::wstring> GetAllSteamLocationsFromVDF() { //Method to get all steam locations from the local file VDF
			const std::wstring steamInstallLocation = GetSteamInstallLocation();
			if (steamInstallLocation.empty()) {
				return { L"STEAM_INSTALL_LOCATION_NOT_FOUND" };
			}

			const std::filesystem::path vdfPath =
				std::filesystem::path(steamInstallLocation) / L"steamapps" / L"libraryfolders.vdf";

			std::error_code ec;
			if (!std::filesystem::is_regular_file(vdfPath, ec) || ec) {
				return { L"STEAM_VDF_FILE_NOT_FOUND" };
			}

			std::wifstream vdfFile(vdfPath);
			if (!vdfFile) {
				return { L"STEAM_VDF_FILE_NOT_FOUND" };
			}

			std::vector<std::wstring> locations;
			locations.push_back(steamInstallLocation);

			std::wstring line;
			while (std::getline(vdfFile, line)) {
				size_t position = line.find_first_not_of(L" \t\r\n");
				if (position == std::wstring::npos) {
					continue;
				}

				std::wstring key;
				if (!getQuotedToken(line, position, key) || key != L"path") {
					continue;
				}

				position = line.find_first_not_of(L" \t\r\n", position);
				std::wstring value;
				if (!getQuotedToken(line, position, value)) {
					continue;
				}

				value = UnescapeVDFString(value);
				if (!value.empty() && isLookLikePath(value)) {
					locations.push_back(value);
				}
			}

			std::vector<std::wstring> uniqueLocations;
			for (const auto& location : locations) {
				if (std::find(uniqueLocations.begin(), uniqueLocations.end(), location) == uniqueLocations.end()) {
					uniqueLocations.push_back(location);
				}
			}

			return uniqueLocations;
		}
		std::vector<std::wstring> ParseSteamInstalledGamesWithPath(const std::vector<std::wstring>& steamLocations) { // Method to parse the installed games from all Steam installation locations


			std::vector<std::wstring> installedGames;

			for (const auto& steamLocation : steamLocations) {
				if (steamLocation.empty()) {
					continue;
				}

				const std::filesystem::path steamAppsPath =
					std::filesystem::path(steamLocation) / L"steamapps";
				std::error_code ec;
				if (!std::filesystem::is_directory(steamAppsPath, ec) || ec) {
					continue;
				}

				for (const auto& entry : std::filesystem::directory_iterator(steamAppsPath, ec)) {
					if (ec) {
						break;
					}

					if (!entry.is_regular_file(ec) || ec) {
						ec.clear();
						continue;
					}

					const std::filesystem::path manifestPath = entry.path();
					const std::wstring fileName = manifestPath.filename().wstring();
					if (fileName.size() <= 16 ||
						fileName.compare(0, 12, L"appmanifest_") != 0 ||
						fileName.substr(fileName.size() - 4) != L".acf") {
						continue;
					}

					std::wifstream manifestFile(manifestPath);
					if (!manifestFile) {
						continue;
					}

					std::wstring installDirectory;
					std::wstring line;
					while (std::getline(manifestFile, line)) {
						size_t position = line.find_first_not_of(L" \t\r\n");
						if (position == std::wstring::npos) {
							continue;
						}

						std::wstring key;
						if (!getQuotedToken(line, position, key) || key != L"installdir") {
							continue;
						}

						position = line.find_first_not_of(L" \t\r\n", position);
						if (position != std::wstring::npos &&
							getQuotedToken(line, position, installDirectory)) {
							installDirectory = UnescapeVDFString(installDirectory);
						}
						break;
					}

					if (installDirectory.empty()) {
						continue;
					}

					const std::filesystem::path gamePath =
						steamAppsPath / L"common" / installDirectory;
					if (std::filesystem::is_directory(gamePath, ec) && !ec &&
						std::find(installedGames.begin(), installedGames.end(), gamePath.wstring()) == installedGames.end()) {
						installedGames.push_back(gamePath.wstring());
					}
					ec.clear();
				}
			}

			return installedGames;
		}

	public:

		std::vector<std::wstring> ParseGameAppId(const std::string& gameName) { // Method to parse the app id of the game from the steam installation location
			std::vector<std::wstring> appIds;
			if (gameName.empty()) {
				return appIds;
			}

			int wideLength = MultiByteToWideChar(
				CP_UTF8,
				MB_ERR_INVALID_CHARS,
				gameName.data(),
				static_cast<int>(gameName.size()),
				nullptr,
				0);
			if (wideLength == 0) {
				return appIds;
			}

			std::wstring requestedName(wideLength, L'\0');
			MultiByteToWideChar(
				CP_UTF8,
				MB_ERR_INVALID_CHARS,
				gameName.data(),
				static_cast<int>(gameName.size()),
				requestedName.data(),
				wideLength);

			std::transform(requestedName.begin(), requestedName.end(), requestedName.begin(),
				[](wchar_t character) { return static_cast<wchar_t>(std::towlower(character)); });

			for (const auto& steamLocation : AllSteamInstalledLocationsVEC) {
				const std::filesystem::path steamAppsPath =
					std::filesystem::path(steamLocation) / L"steamapps";
				std::error_code ec;
				if (!std::filesystem::is_directory(steamAppsPath, ec) || ec) {
					continue;
				}

				for (const auto& entry : std::filesystem::directory_iterator(steamAppsPath, ec)) {
					if (ec) {
						break;
					}

					if (!entry.is_regular_file(ec) || ec) {
						ec.clear();
						continue;
					}

					const std::wstring fileName = entry.path().filename().wstring();
					if (fileName.size() <= 16 ||
						fileName.compare(0, 12, L"appmanifest_") != 0 ||
						fileName.substr(fileName.size() - 4) != L".acf") {
						continue;
					}

					std::wifstream manifestFile(entry.path());
					if (!manifestFile) {
						continue;
					}

					std::wstring appId;
					std::wstring installedGameName;
					std::wstring line;
					while (std::getline(manifestFile, line)) {
						size_t position = line.find_first_not_of(L" \t\r\n");
						if (position == std::wstring::npos) {
							continue;
						}

						std::wstring key;
						if (!getQuotedToken(line, position, key)) {
							continue;
						}

						position = line.find_first_not_of(L" \t\r\n", position);
						std::wstring value;
						if (position == std::wstring::npos || !getQuotedToken(line, position, value)) {
							continue;
						}

						value = UnescapeVDFString(value);
						if (key == L"appid") {
							appId = value;
						}
						else if (key == L"name") {
							installedGameName = value;
						}
					}

					std::transform(installedGameName.begin(), installedGameName.end(), installedGameName.begin(),
						[](wchar_t character) { return static_cast<wchar_t>(std::towlower(character)); });

					if (!appId.empty() && installedGameName == requestedName &&
						std::find(appIds.begin(), appIds.end(), appId) == appIds.end()) {
						appIds.push_back(appId);
					}
				}
			}

			return appIds;
		}

		std::vector<std::wstring> AllSteamInstalledLocationsVEC; // Vector to store all the steam installation locations
		std::vector<std::wstring> AllSteamInstalledGamesWithAppIdVEC; // Vector to store all the steam installed games
		std::vector<std::wstring> AllSteamInstalledGamesWithPathVEC; // Vector to store all the steam installed games with path

		std::wstring GameAppId = {};


		SteamGamesParcer() { // Constructor

			SteamInstallLocation = GetSteamInstallLocation(); // Get the steam installation location from the registry
			AllSteamInstalledLocationsVEC = GetAllSteamLocationsFromVDF(); // Get all the steam installation locations from the local file VDF
			AllSteamInstalledGamesWithPathVEC = ParseSteamInstalledGamesWithPath(AllSteamInstalledLocationsVEC); // Parse the path of the game from the steam installation location

		}

	};

}
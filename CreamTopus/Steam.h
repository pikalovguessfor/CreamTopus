#pragma once

#include <string>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <filesystem>

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

	public:

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
		
		std::vector<std::string> ParseSteamInstalledGames() { // Method to parse the installed games from the steam installation location
			return {};
		}

		std::vector<std::string> ParseGameAppId(const std::string& gameName) { // Method to parse the app id of the game from the steam installation location
			return {};
		}
	
	};

}
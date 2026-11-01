#pragma once

#include <string>
#include <Windows.h>

#pragma warning(disable: 4996)

namespace Octo {

	class Services // Class to provide information about the operating system and system architecture
	{

	private:

		std::string GetCurrentVersionOperatingSystem() {
			OSVERSIONINFOEX osvi;
			ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
			GetVersionEx((OSVERSIONINFO*)&osvi);
			return std::to_string(osvi.dwMajorVersion) + "." + std::to_string(osvi.dwMinorVersion) + "." + std::to_string(osvi.dwBuildNumber);
		}
		std::string GetCurrentVersionWindows() {
			OSVERSIONINFOEX osvi;
			ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
			GetVersionEx((OSVERSIONINFO*)&osvi);
			return std::to_string(osvi.dwMajorVersion) + "." + std::to_string(osvi.dwMinorVersion) + "." + std::to_string(osvi.dwBuildNumber);
		}
		std::string GetCurrentSystemArchitecture() {
			SYSTEM_INFO siSysInfo;
			GetSystemInfo(&siSysInfo);
			switch (siSysInfo.wProcessorArchitecture) {
			case PROCESSOR_ARCHITECTURE_AMD64:
				return "x64 (AMD or Intel)";
			case PROCESSOR_ARCHITECTURE_ARM:
				return "ARM";
			case PROCESSOR_ARCHITECTURE_IA64:
				return "Intel Itanium-based";
			case PROCESSOR_ARCHITECTURE_INTEL:
				return "x86";
			default:
				return "Unknown architecture";
			}
		}
		std::string GetCurrentSystemLanguage() {
			LCID lcid = GetUserDefaultLCID();
			char localeName[LOCALE_NAME_MAX_LENGTH];
			if (GetLocaleInfoA(lcid, LOCALE_SISO639LANGNAME, localeName, LOCALE_NAME_MAX_LENGTH)) {
				return std::string(localeName);
			}
			return "Unknown language";
		}

	public:

		std::string CurrentOperatingSystem = GetCurrentVersionOperatingSystem();
		std::string CurrentWindowsVersion = GetCurrentVersionWindows();
		std::string CurrentSystemArchitecture = GetCurrentSystemArchitecture();
		std::string CurrentSystemLanguage = GetCurrentSystemLanguage();

	};

	class Process // Class to provide functionalities to create and manage processes
	{

	private:



	public:

		void CreateProcess(const std::string& applicationPath, const std::string& commandLineArgs) {
			STARTUPINFOA si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));
			std::string commandLine = applicationPath + " " + commandLineArgs;
			if (!CreateProcessA(NULL, const_cast<char*>(commandLine.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
				throw std::runtime_error("Failed to create process. Error: " + std::to_string(GetLastError()));
			}
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

	};

}
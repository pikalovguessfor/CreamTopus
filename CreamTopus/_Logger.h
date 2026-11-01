#pragma once

#include <string>
#include <fstream>
#include <chrono>

#include "OS.h"
#include "fileSystem.h"

namespace Octo {

	class Logger {

	private:

		std::string logFilePath;

	public:

		const std::string LDEBUG = "DEBUG";
		const std::string LINFO = "INFO";
		const std::string LWARNING = "WARNING";
		const std::string LERROR = "ERROR";
		const std::string LFATAL = "FATAL";
		const std::string LCRITICAL = "CRITICAL";

		void Write(const std::string& message) {
			std::ofstream logFile(logFilePath, std::ios::app);
			if (logFile.is_open()) {
				logFile << message << std::endl;
				logFile.close();
			}
		}

		Logger() {
			// Constructor
			Services services;
			Directories directories;

			directories.CreateDirectoryIfNotExists("./logs");

			logFilePath = "./logs/" + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()) + ".txt";
			Write("Logger initialized. Log file created at: " + logFilePath);
			Write("Operating System: " + services.CurrentOperatingSystem);
			Write("Windows Version: " + services.CurrentWindowsVersion);
			Write("System Architecture: " + services.CurrentSystemArchitecture);
			Write("System Language: " + services.CurrentSystemLanguage);
		}
		~Logger() {
			// Destructor

		}
	};
}
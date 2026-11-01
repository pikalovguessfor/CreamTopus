#pragma once

#include <string>
#include <fstream>
#include <chrono>

#include "OS.h"
#include "fileSystem.h"

namespace Octo {

	class Logger { // Class to provide logging functionalities

	private:

		std::string logFilePath; // Path to the log file where log messages will be written
		std::string GetCurrentTimestamp() {
			auto now = std::chrono::system_clock::now();
			auto in_time_t = std::chrono::system_clock::to_time_t(now);
			std::tm buf;
			localtime_s(&buf, &in_time_t);
			char timestamp[20];
			strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &buf);
			return std::string(timestamp);
		}

	public:

		// Log level constants to categorize log messages
		const std::string LDEBUG = "DEBUG";
		const std::string LINFO = "INFO";
		const std::string LWARNING = "WARNING";
		const std::string LERROR = "ERROR";
		const std::string LFATAL = "FATAL";
		const std::string LCRITICAL = "CRITICAL";

		void Write(const std::string& message) {
			std::ofstream logFile(logFilePath, std::ios::app);
			if (logFile.is_open()) {
				logFile << "[" + GetCurrentTimestamp() + "]" << " - " << message << std::endl; // Write the log message to the log file with a timestamp
				logFile.close();
			}
		}

		Logger() { 
			// Constructor
			Services services;
			Directories directories;

			directories.CreateDirectoryIfNotExists("./logs");

			logFilePath = "./logs/" + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()) + ".txt"; // Create a unique log file name based on the current timestamp
			Write("Logger initialized. Log file created at: " + logFilePath);  // Log the creation of the log file
			Write("Operating System: " + services.CurrentOperatingSystem); // Log the current operating system version
			Write("Windows Version: " + services.CurrentWindowsVersion);// Log the current Windows version
			Write("System Architecture: " + services.CurrentSystemArchitecture); // Log the current system architecture
			Write("System Language: " + services.CurrentSystemLanguage); // Log the current system language
		}
		~Logger() {
			// Destructor

		}
	};
}
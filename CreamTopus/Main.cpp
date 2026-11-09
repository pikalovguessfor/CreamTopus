#include <iostream>
#include <string>

#include "OS.h"
#include "_Logger.h"
#include "FileSystem.h"
#include "_Debug.h"
#include "Steam.h"


/*
* Welcome to creamTopus! This is a utility to unlock Steam DLCs for games.
* The program interacts with the Steam installation, retrieves information about installed games, and allows users to unlock DLCs for those games.
* It also provides logging and debugging functionalities to help track the process and any issues that may arise.
* Code is organized into classes and structs to encapsulate related functionalities, making it easier to maintain and extend.
* All classes and structs are defined in their respective header files, and the main logic is implemented in the Main.cpp file.
* All classes and structs are encapsulated within the Octo namespace to avoid naming conflicts and provide a clear structure for the codebase.
* If you have any questions or need further assistance, please refer to the documentation or contact the developer.
* Else if you want to modificate this utility, please make sure to understand the code and its dependencies before making any changes.
* Good luck and happy coding, sir or madam!
*/

int main() {

	Octo::Logger logger; // Creating a object of the Logger class to log messages
	Octo::Steam steam; // Creating a object of the Steam class to interact with Steam
	Octo::Debug debug; // Creating a object of the Debug class to debug the application
	Octo::Services services; // Creating a object of the Services class to get information about the operating system
	Octo::Directories directories; // Creating a object of the Directories class to interact with directories
	Octo::File file; // Creating a object of the File class to interact with files
	Octo::FileInfo fileInfo; // Creating a object of the FileInfo struct to store information about files
	Octo::ApplicationInfo appInfo; // Creating a object of the ApplicationInfo struct to store information about applications

	std::cout << "Welcome to creamTopus! Utility to unlock steam dlc`s" << std::endl;
	
	directories.CreateDirectoryIfNotExists("temp"); // Creating a temporary directory to store temporary files
	file.CreateTxTFileIfNotExists("temp/InputMain.txt", " ");// Creating a temporary input file to store the input from the CPP Program
	file.CreateTxTFileIfNotExists("temp/OutputScript.txt", " "); // Creating a temporary output file to output from the c# script

	while (true){

		//----------------------Logic to Find the Steam Installation Directories----------------------//





		//-------------------End of Logic to Find the Steam Installation Directories------------------//
		//----------------------Logic to Find Installed games-----------------------------------------//




		//-------------------End of Logic to Find Installed games-------------------------------------//



		//----------------------Logic to Select which game need to be unlocked-----------------------//



		//-------------------End of Logic to Select which game need to be unlocked-------------------//
		//----------------------Logic to Parse Game DLC`s for Game-----------------------------------//




		//-------------------End of Logic to Parse Game DLC`s for Game---------------------------------//
		//----------------------Logic to Unlock Game DLC`s---------------------------------------------//




		//-------------------End of Logic to Unlock Game DLC`s-----------------------------------------//
	}

	file.DeleteTxTFileIfExists("temp/InputMain.txt"); // Deleting the temporary input file after the process is completed	
	file.DeleteTxTFileIfExists("temp/OutputScript.txt"); // Deleting the temporary output file after the process is completed
	directories.DeleteDirectoryIfExists("temp"); // Deleting the temporary directory after the process is completed
}
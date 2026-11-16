#pragma once

#include <string>
#include <Windows.h>

#include "FileSystem.h"
#include "Steam.h"

namespace Octo {

	class SmokeApi
	{
	private:

		Octo::Directories directories; // Creating a object of the Directories class to interact with directories

	public:

		bool IsSmokeApiExistInWin_Smoke_ApiDirectory() { // Method to check if the SmokeApi.dll file exists in the Win_Smoke_Api directory
			if (directories.isFileExists("win_smoke_api") && directories.isFileExists("win_smoke_api/SmokeApi.dll")) {
				return true;
			}
			return false;
		}

	};

};
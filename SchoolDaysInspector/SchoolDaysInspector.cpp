#include "Constants.h"
#include "FilmEngine.h"
#include "OverflowINIExtractor.h"
#include "StringConverter.h"
#include <RouteProcSDHQ.h>
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

//TODO: Look into ROute 4, Scene 51 (0x33). This calls an evil function at 0x1c in vtable

void TextSchoolDays(int startRouteIndex = 0, int startSceneIndex = 0)
{
	//Initialize the FilmEngine
	FilmEngine filmEngine;

	//Initialize the root path of the script
	std::wstring rootScript = Constants::SCRIPT_ROOT_PATH;

	//Set the root to the beginning of game
	int currentRouteIndex = startRouteIndex;
	int currentSceneIndex = startSceneIndex;
	const size_t BUFFER_SIZE = 1024;
	wchar_t buffer[BUFFER_SIZE];
	SetRoot(startRouteIndex, startSceneIndex, &filmEngine);
	SetScript(currentRouteIndex, currentSceneIndex, buffer, BUFFER_SIZE);

	while (filmEngine.GetFlagValue(L"ROUTE") != -1)
	{
		//Get the route and scene index
		currentRouteIndex = filmEngine.GetFlagValue(L"ROUTE");
		currentSceneIndex = filmEngine.GetFlagValue(L"SCENE");

		//Get the relative script path
		std::wstring relativeScriptPath = buffer;

		//Convert all / to \ in relative script path
		std::replace(relativeScriptPath.begin(), relativeScriptPath.end(), L'/', L'\\');

		//Add extension to relative script path
		relativeScriptPath = fs::path(relativeScriptPath).replace_extension(L".ENG.ORS");

		//Print the current relative script path
		std::wcout << std::format(L"Current relative script path: {} ({},{})\n", relativeScriptPath, currentRouteIndex, currentSceneIndex);

		//Print the entire script
		std::wstring fullScriptPath = (fs::path(rootScript) / relativeScriptPath);
		std::ifstream scriptStream(fullScriptPath);
		if(!scriptStream.is_open())
		{
			//For some reason, School Days has a bug, where they will try to load scripts that don't exist. They cope by just loading the next
			GetNextScriptFile(&filmEngine, buffer, BUFFER_SIZE);
			continue;
		}
		std::cout << scriptStream.rdbuf();
		std::cout << "\n\n";

		//Seek back to beginning of file
		scriptStream.seekg(0);

		//Check if the script has [SetSELECT] command and find the line
		std::string setSelectCommand = "[SetSELECT]";
		std::string setSelectLine;
		while (std::getline(scriptStream, setSelectLine))
		{
			if (setSelectLine.find(setSelectCommand) != std::string::npos)
			{
				//Take the line and split by tabs. The second and third elements are the top and bottom choices
				std::vector<std::string> lineElements = OverflowINIExtractor::SplitByString(setSelectLine, "\t");

				//Print the choices
				bool isBottomChoiceDefined = lineElements[2] != "null";
				std::cout << "Silence (-1)\n";
				std::cout << "Top choice (0): " << lineElements[1] << "\n";
				if (isBottomChoiceDefined)
				{
					std::cout << "Bottom choice (1): " << lineElements[2] << "\n";
				}

				//Get the user input
				int userInput = -2;
				bool isUserInputValid = false;
				do
				{
					do
					{
						std::cin.clear();
						std::cin >> userInput;
					} 
					while (std::cin.fail());

					isUserInputValid = userInput == -1 || userInput == 0 || (userInput == 1 && isBottomChoiceDefined);
				} 
				while (!isUserInputValid);

				//Set the last choice, trigger a SetFeeling
				filmEngine.LastFeelingSelection = userInput;
				SetFeeling(&filmEngine, 1);

				//Exit the loop going through every line
				break;
			}
		}

		//Get the next script
		GetNextScriptFile(&filmEngine, buffer, BUFFER_SIZE);
	}

	std::cout << "Game has ended\n";
}

void WriteJumpInstructionToNewFunction(std::uintptr_t oldFunctionPointer, std::uintptr_t newFunctionPointer)
{
	//Get the relative address to the new function for JMP instruction
	std::uintptr_t currentAddress = oldFunctionPointer;
	std::uintptr_t relativeAddress = newFunctionPointer - currentAddress - 5;

	//Write the shellcode for the JMP instruction
	uint8_t shellcode[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
	*(std::uintptr_t*)&shellcode[1] = relativeAddress;

	//Write the shellcode to the old function pointer
	DWORD oldProtect;
	VirtualProtect((void*)oldFunctionPointer, sizeof(shellcode), PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void*)oldFunctionPointer, shellcode, sizeof(shellcode));
	VirtualProtect((void*)oldFunctionPointer, sizeof(shellcode), oldProtect, &oldProtect);
}

int main()
{
	//Ensure RouteProcSDHQ_OG.dll is loaded and find its base address
	HMODULE routeProcModule = LoadLibrary(L"RouteProcSDHQ_OG.dll");
	if (routeProcModule == NULL)
	{
		std::cerr << "Failed to load RouteProcSDHQ_OG.dll\n";
		return 1;
	}
	std::uintptr_t routeProcBase = (std::uintptr_t)routeProcModule;

	//Replace olf AdjustFeelingFlag function with new one
	std::uintptr_t oldAdjustFeelingFlag = routeProcBase + 0x5EB0;
	std::uintptr_t newAdjustFeelingFlag = (std::uintptr_t)&AdjustFeelingFlag;
	WriteJumpInstructionToNewFunction(oldAdjustFeelingFlag, newAdjustFeelingFlag);
	
	//Replace old IsStanderdScriptFlagMet function with new one
	std::uintptr_t oldIsStanderdScriptFlagMet = routeProcBase + 0x6000;
	std::uintptr_t newIsStanderdScriptFlagMet = (std::uintptr_t)&IsStanderdScriptFlagMet;
	WriteJumpInstructionToNewFunction(oldIsStanderdScriptFlagMet, newIsStanderdScriptFlagMet);

	TextSchoolDays();
}
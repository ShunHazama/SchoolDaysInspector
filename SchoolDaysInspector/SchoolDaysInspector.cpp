#include "Constants.h"
#include "FilmEngine.h"
#include "OverflowINIExtractor.h"
#include "StringConverter.h"
#include <RouteProcSDHQ.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "ScriptObject.h"
#include <optional>

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

		//Get the script (if the script is not openable, just move onto next one)
		std::optional<ScriptObject> possibleCurrentScript;
		try
		{
			possibleCurrentScript = ScriptObject(buffer, currentRouteIndex, currentSceneIndex);
		}
		catch (std::invalid_argument)
		{
			GetNextScriptFile(&filmEngine, buffer, BUFFER_SIZE);
			continue;
		}
		ScriptObject currentScript = possibleCurrentScript.value();

		//Print the current relative script path
		std::wcout << std::format(L"Current relative script path: {} ({},{})\n", currentScript.GetRelativePath(), currentRouteIndex, currentSceneIndex);

		//Print the entire script
		std::cout << currentScript.PrintScript();
		std::cout << "\n\n";

		//Check if there are choices in the scene
		if (currentScript.IsChoiceScript())
		{
			//Print the choices
			std::cout << "Silence (-1)\n";
			std::cout << "Top choice (0): " << currentScript.GetTopChoice() << "\n";
			if (currentScript.IsBottomChoiceDefined())
			{
				std::cout << "Bottom choice (1): " << currentScript.GetBottomChoice() << "\n";
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
				} while (std::cin.fail());

				isUserInputValid = userInput == -1 || userInput == 0 || (userInput == 1 && currentScript.IsBottomChoiceDefined());
			} while (!isUserInputValid);

			//Set the last choice, trigger a SetFeeling
			filmEngine.LastFeelingSelection = userInput;
			SetFeeling(&filmEngine, 1);
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
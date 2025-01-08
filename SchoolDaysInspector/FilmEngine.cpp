#include "Constants.h"
#include "FilmEngine.h"
#include <iostream>
#include <format>

FilmEngine::FilmEngine()
{
	//Give the selection a default
	LastFeelingSelection = SILENCE;

	//Create the feeling flag increments
	FeelingFlagIncrements = OverflowINIExtractor::ExtractAllFeelingFlagIncrements(Constants::FEELING_SCRIPT_PATH);

	//Get the standard script requirements
	StanderdScriptRequirements = OverflowINIExtractor::ExtractAllStanderdScriptRequirements(Constants::STANDERD_SCRIPT_PATH);

	//Create function ptr for each member function
	int (FilmEngine::*GetLastFeelingSelectionPtr)() = &FilmEngine::GetLastFeelingSelection;
	int (FilmEngine::*GetFlagValuePtr)(const wchar_t*) = &FilmEngine::GetFlagValue;
	void (FilmEngine::*SetFlagValuePtr)(const wchar_t*, int) = &FilmEngine::SetFlagValue;
	void (FilmEngine::*ProbablyTriggersBarPtr)(int) = &FilmEngine::ProbablyTriggersBar;
	void (FilmEngine::*SetDiscoveredStringPtr)(const wchar_t*, bool) = &FilmEngine::SetDiscoveredString;
	bool (FilmEngine:: *IsDiscoveredStringSetPtr)(const wchar_t*) = &FilmEngine::IsDiscoveredStringSet;
	void (FilmEngine::*SetStrangeCodePtr)(const wchar_t*, bool) = &FilmEngine::SetStrangeCode;
	int (FilmEngine::*IsStrangeCodeSetPtr)(const wchar_t*) = &FilmEngine::IsStrangeCodeSet;
	void (FilmEngine:: * SetStartupFlagValuePtr)(const wchar_t*, int) = &FilmEngine::SetStartupFlagValue;

	int (FilmEngine::*UselessEvilFunctionPtr)() = &FilmEngine::UselessEvilFunction;

	void (FilmEngine::*Function0_0x0Ptr)(int, wchar_t*) = &FilmEngine::Function0_0x0;
	void (FilmEngine::*Function11_0x2CPtr)() = &FilmEngine::Function11_0x2C;
	int (FilmEngine::*Function13_0x34Ptr)() = &FilmEngine::Function13_0x34;
	void (FilmEngine::*Function14_0x38Ptr)(int) = &FilmEngine::Function14_0x38;
	void (FilmEngine:: * Function15_0x3CPtr)() = &FilmEngine::Function15_0x3C;

	//Initialize the vtable
	VTable = new std::uintptr_t[VTABLE_SIZE];
	for (int index = 0; index < VTABLE_SIZE; index++)
	{
		//Fill every entry with a pointer to the UselessEvilFunction
		VTable[index] = *reinterpret_cast<uintptr_t*>(&UselessEvilFunctionPtr);
	}

	//Replace the entries that are actually used
	VTable[GET_LAST_FEELING_SELECTION] = *reinterpret_cast<uintptr_t*>(&GetLastFeelingSelectionPtr);
	VTable[GET_FLAG_VALUE] = *reinterpret_cast<uintptr_t*>(&GetFlagValuePtr);
	VTable[SET_FLAG_VALUE] = *reinterpret_cast<uintptr_t*>(&SetFlagValuePtr);
	VTable[PROBABLY_TRIGGERS_BAR] = *reinterpret_cast<uintptr_t*>(&ProbablyTriggersBarPtr);
	VTable[SET_DISCOVERED_STRING] = *reinterpret_cast<uintptr_t*>(&SetDiscoveredStringPtr);
	VTable[IS_DISCOVERED_STRING_SET] = *reinterpret_cast<uintptr_t*>(&IsDiscoveredStringSetPtr);
	VTable[SET_STRANGE_CODE] = *reinterpret_cast<uintptr_t*>(&SetStrangeCodePtr);
	VTable[IS_STRANGE_CODE_SET] = *reinterpret_cast<uintptr_t*>(&IsStrangeCodeSetPtr);
	VTable[SET_STARTUP_FLAG_VALUE] = *reinterpret_cast<uintptr_t*>(&SetStartupFlagValuePtr);
	VTable[FUNCTION0_0x0] = *reinterpret_cast<uintptr_t*>(&Function0_0x0Ptr);
	VTable[FUNCTION11_0x2C] = *reinterpret_cast<uintptr_t*>(&Function11_0x2CPtr);
	VTable[FUNCTION13_0x34] = *reinterpret_cast<uintptr_t*>(&Function13_0x34Ptr);
	VTable[FUNCTION14_0x38] = *reinterpret_cast<uintptr_t*>(&Function14_0x38Ptr);
	VTable[FUNCTION15_0x3C] = *reinterpret_cast<uintptr_t*>(&Function15_0x3CPtr);
}

FilmEngine::~FilmEngine()
{
	delete[] VTable;
}

int FilmEngine::GetLastFeelingSelection()
{
	return LastFeelingSelection;
}

int FilmEngine::GetFlagValue(const wchar_t* flagString)
{
	return Flags[flagString];
}

void FilmEngine::SetFlagValue(const wchar_t* flagString, int newFlagValue)
{
	Flags[flagString] = newFlagValue;
}

void FilmEngine::ProbablyTriggersBar(int isBarUpdated)
{
	if (isBarUpdated == 1)
	{
		std::cout << "Bar updated!\n";
	}
	else
	{
		std::cout << "Why????\n";
		std::exit(-1);
	}
}

void FilmEngine::SetDiscoveredString(const wchar_t* discoveredString, bool isSet)
{
	DiscoveredStringFlags[discoveredString] = isSet;
}

bool FilmEngine::IsDiscoveredStringSet(const wchar_t* discoveredString)
{
	return DiscoveredStringFlags[discoveredString];
}

void FilmEngine::SetStrangeCode(const wchar_t* strangeCode, bool isSet)
{
	StrangeCodeFlags[strangeCode] = isSet;
}

int FilmEngine::IsStrangeCodeSet(const wchar_t* strangeCode)
{
	return StrangeCodeFlags[strangeCode];
}

void FilmEngine::SetStartupFlagValue(const wchar_t* startupFlag, int value)
{
	StartupFlags[startupFlag] = value;
}

void FilmEngine::Function0_0x0(int weirdCode, wchar_t* scriptPath)
{
}

void FilmEngine::Function11_0x2C()
{
}

int FilmEngine::Function13_0x34()
{
	return 0;
}

void FilmEngine::Function14_0x38(int randomThang)
{
}

void FilmEngine::Function15_0x3C()
{
}

int FilmEngine::UselessEvilFunction()
{
	std::cout << "This function is evil and should never be called!\n";
	return 0;
}

void __cdecl AdjustFeelingFlag(FilmEngine* filmEngine, const wchar_t* nextScene, bool isIncrement)
{
	//Strip "/" and everything before it
	std::wstring nextSceneString(nextScene);
	size_t pos = nextSceneString.find_first_of(L"/");
	if (pos == std::wstring::npos)
	{
		exit(-1);
	}
	nextSceneString = nextSceneString.substr(pos + 1);

	FeelingFlagIncrement flagIncrement = filmEngine->FeelingFlagIncrements[nextSceneString];

	//Get the old flag value
	int oldFlagValue = filmEngine->GetFlagValue(flagIncrement.flagName.c_str());

	if(isIncrement)
	{
		//Increment the flag value
		filmEngine->SetFlagValue(flagIncrement.flagName.c_str(), oldFlagValue + flagIncrement.feelingIncrement);
	}
	else
	{
		//Decrement the flag value
		filmEngine->SetFlagValue(flagIncrement.flagName.c_str(), oldFlagValue - flagIncrement.feelingIncrement);
	}
}

int __cdecl IsStanderdScriptFlagMet(FilmEngine* filmEngine, const wchar_t* scriptPath)
{
	//Strip "/" and everything before it
	std::wstring scriptPathString(scriptPath);
	size_t pos = scriptPathString.find_first_of(L"/");
	if (pos == std::wstring::npos)
	{
		exit(-1);
	}
	scriptPathString = scriptPathString.substr(pos + 1);

	FeelingFlagIncrement flagRequirement = filmEngine->StanderdScriptRequirements[scriptPathString];

	//Get the flag value
	int flagValue = filmEngine->GetFlagValue(flagRequirement.flagName.c_str());

	//Return 1 if the flag value meets the requirement, 0 if not
	if (flagValue >= flagRequirement.feelingIncrement)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

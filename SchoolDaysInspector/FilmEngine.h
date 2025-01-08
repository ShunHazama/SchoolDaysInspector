#pragma once
#include "OverflowINIExtractor.h"
#include <unordered_map>
#include <unordered_set>
#include <string>

enum vtable_index
{
	GET_LAST_FEELING_SELECTION = 1,
	GET_FLAG_VALUE = 2,
	SET_FLAG_VALUE = 3,
	IS_STRANGE_CODE_SET = 4,
	SET_STRANGE_CODE = 5,
	IS_DISCOVERED_STRING_SET = 6,
	SET_DISCOVERED_STRING = 7,
	SET_STARTUP_FLAG_VALUE = 9,
	PROBABLY_TRIGGERS_BAR = 12,

	FUNCTION0_0x0 = 0,
	FUNCTION11_0x2C = 11,
	FUNCTION13_0x34 = 13,
	FUNCTION14_0x38 = 14,
	FUNCTION15_0x3C = 15
};

enum feeling_selection
{
	SILENCE = -1,
	TOP = 0,
	BOTTOM = 1,
};

#define VTABLE_SIZE 86

struct FilmEngine
{
	std::uintptr_t* VTable;

	std::unordered_map<std::wstring, FeelingFlagIncrement> FeelingFlagIncrements;
	std::unordered_map<std::wstring, FeelingFlagIncrement> StanderdScriptRequirements;

	std::unordered_map<std::wstring, int> Flags;
	std::unordered_map<std::wstring, int> StartupFlags;
	std::unordered_map<std::wstring, bool> DiscoveredStringFlags;
	std::unordered_map<std::wstring, bool> StrangeCodeFlags;

	int LastFeelingSelection;

	FilmEngine();
	~FilmEngine();

	int GetLastFeelingSelection();
	int GetFlagValue(const wchar_t* flagString);
	void SetFlagValue(const wchar_t* flagString, int newFlagValue);
	void ProbablyTriggersBar(int isBarUpdated);
	void SetDiscoveredString(const wchar_t* discoveredString, bool isSet);
	bool IsDiscoveredStringSet(const wchar_t* discoveredString);
	void SetStrangeCode(const wchar_t* strangeCode, bool isSet);
	int IsStrangeCodeSet(const wchar_t* strangeCode);
	void SetStartupFlagValue(const wchar_t* startupFlag, int value);

	//Function's I'm not too sure about
	void Function0_0x0(int weirdCode, wchar_t* scriptPath);
	void Function11_0x2C(); //Idk even what this be doing
	int Function13_0x34(); //Literally just grabs some random variable from the filmEngine. Just return 0
	void Function14_0x38(int randomThang); //Literally just sets some random OTHER variable from filmEngine
	void Function15_0x3C(); //Idk lol

	//Backup function
	int UselessEvilFunction();

};

void __cdecl AdjustFeelingFlag(FilmEngine* filmEngine, const wchar_t* nextScene, bool isIncrement);
int __cdecl IsStanderdScriptFlagMet(FilmEngine* filmEngine, const wchar_t* scriptPath);
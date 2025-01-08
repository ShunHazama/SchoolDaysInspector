#pragma once
#include <string>
#include <unordered_map>

struct FeelingFlagIncrement
{
	std::wstring flagName;
	int feelingIncrement = 0;
};

class OverflowINIExtractor
{
	public:
		static std::vector<std::string> SplitByString(std::string inputString, const std::string& delimiter);

		static std::unordered_map<std::string, std::string> ExtractOverflowINIFile(const std::wstring& filePath);
		static std::unordered_map<std::wstring, FeelingFlagIncrement> ExtractAllFeelingFlagIncrements(const std::wstring& feelingFilePath);
		static std::unordered_map<std::wstring, FeelingFlagIncrement> ExtractAllStanderdScriptRequirements(const std::wstring& standerdScriptPath);
};
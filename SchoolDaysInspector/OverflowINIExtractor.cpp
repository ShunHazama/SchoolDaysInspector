#include "OverflowINIExtractor.h"
#include "StringConverter.h"
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::string> OverflowINIExtractor::SplitByString(std::string inputString, const std::string& delimiter)
{
	std::vector<std::string> splitStrings;
	splitStrings.reserve(4);
	size_t pos = 0;

	while ((pos = inputString.find(delimiter)) != std::string::npos)
	{
		std::string token = inputString.substr(0, pos);
		splitStrings.push_back(token);
		inputString.erase(0, pos + delimiter.length());
	}
	splitStrings.push_back(inputString);

	return splitStrings;
}

std::unordered_map<std::string, std::string> OverflowINIExtractor::ExtractOverflowINIFile(const std::wstring & filePath)
{
	//Ensure file exists
	if (!fs::exists(filePath))
	{
		throw std::invalid_argument(std::format("File ({}) does not exist", filePath));
	}

	//Fill the result map with values, where each line formatted as [key]="value" is added to the map
	std::unordered_map<std::string, std::string> result;
	std::ifstream fileStream(filePath, std::ios::in);
	std::string line;
	while (std::getline(fileStream, line))
	{
		//Find the key and value, ensuring that the brackets and quotes are in the right places. If not, then we go to the next line
		size_t keyStart = line.find_first_of('[', 0);
		if(keyStart == std::wstring::npos)
		{
			continue;
		}

		size_t keyEnd = line.find_first_of(']', keyStart+1);
		if (keyEnd == std::wstring::npos)
		{
			continue;
		}

		size_t valueStart = line.find_first_of('"', keyEnd +1);
		if (valueStart == std::wstring::npos)
		{
			continue;
		}

		size_t valueEnd = line.find_last_of('"');
		if (valueEnd == std::wstring::npos)
		{
			continue;
		}

		//Extract the key and value from the line
		std::string key = line.substr(keyStart + 1, keyEnd - keyStart - 1);
		std::string value = line.substr(valueStart + 1, valueEnd - valueStart - 1);

		//Store them if they are not empty
		if(key.empty() || value.empty())
		{
			continue;
		}
		result[key] = value;
	}

	return result;
}

std::unordered_map<std::wstring, FeelingFlagIncrement> OverflowINIExtractor::ExtractAllFeelingFlagIncrements(const std::wstring& feelingFilePath)
{
	//Grab all the flags from the feeling file
	std::unordered_map<std::string, std::string> flagValues = OverflowINIExtractor::ExtractOverflowINIFile(feelingFilePath);

	//Delete the flags that are not feeling flags
	flagValues.erase("Number");
	flagValues.erase("flag0");
	flagValues.erase("flag1");
	flagValues.erase("flag2");
	flagValues.erase("flag3");
	flagValues.erase("flag4");
	flagValues.erase("flag5");

	//Create a map of feeling flags to their value increments
	std::unordered_map<std::wstring, FeelingFlagIncrement> result;
	for (const auto& [scriptName, flagAndValueIncrement] : flagValues)
	{
		std::vector<std::string> flagAndValueIncrementSplit = OverflowINIExtractor::SplitByString(flagAndValueIncrement, ",");

		result[StringConverter::ConvertNarrowStringToWideString(scriptName)] = { StringConverter::ConvertNarrowStringToWideString(flagAndValueIncrementSplit[0]), std::stoi(flagAndValueIncrementSplit[1]) };
	}

	return result;
}

std::unordered_map<std::wstring, FeelingFlagIncrement> OverflowINIExtractor::ExtractAllStanderdScriptRequirements(const std::wstring& standerdScriptPath)
{
	//Grab all the flags from the standerd script file
	std::unordered_map<std::string, std::string> flagValues = OverflowINIExtractor::ExtractOverflowINIFile(standerdScriptPath);

	//Delete the flags that are not feeling flags
	flagValues.erase("Number");
	flagValues.erase("flag0");
	flagValues.erase("flag1");
	flagValues.erase("flag2");
	flagValues.erase("flag3");
	flagValues.erase("flag4");
	flagValues.erase("flag5");

	//Create a map of feeling flags to their requirements
	std::unordered_map<std::wstring, FeelingFlagIncrement> result;
	for (const auto& [scriptName, flagAndValueIncrement] : flagValues)
	{
		std::vector<std::string> flagAndValueIncrementSplit = OverflowINIExtractor::SplitByString(flagAndValueIncrement, ",");

		result[StringConverter::ConvertNarrowStringToWideString(scriptName)] = { StringConverter::ConvertNarrowStringToWideString(flagAndValueIncrementSplit[0]), std::stoi(flagAndValueIncrementSplit[1]) };
	}

	return result;
}

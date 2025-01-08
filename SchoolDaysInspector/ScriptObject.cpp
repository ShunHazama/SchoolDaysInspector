#include "ScriptObject.h"
#include "Constants.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include "OverflowINIExtractor.h"

namespace fs = std::filesystem;

ScriptObject::ScriptObject(const wchar_t* inputBuffer, int routeIndex, int sceneIndex)
{
	RelativePath = inputBuffer;

	Index = { .Route = routeIndex, .Scene = sceneIndex };

	//Convert all / to \ in relative script path
	std::replace(RelativePath.begin(), RelativePath.end(), L'/', L'\\');

	//Add extension to script path
	RelativePath = fs::path(RelativePath).replace_extension(L".ENG.ORS");

	//Try to open up the file and if the file is unopenable, throw an exception
	std::wstring fullPath = fs::path(Constants::SCRIPT_ROOT_PATH) / RelativePath;
	std::ifstream scriptStream(fullPath);
	if (!scriptStream.is_open())
	{
		throw std::invalid_argument("File unavailable");
	}

	//Check if the script has [SetSELECT] command and find the line
	std::string setSelectCommand = "[SetSELECT]";
	std::string setSelectLine;
	while (std::getline(scriptStream, setSelectLine))
	{
		if (setSelectLine.find(setSelectCommand) != std::string::npos)
		{
			//Take the line and split by tabs. The second and third elements are the top and bottom choices
			std::vector<std::string> lineElements = OverflowINIExtractor::SplitByString(setSelectLine, "\t");

			//Insert top choice and bottom choice if defined
			bool isBottomChoiceDefined = lineElements[2] != "null";
			TopAndBottomChoices.push_back(lineElements[1]);
			if (isBottomChoiceDefined)
			{
				TopAndBottomChoices.push_back(lineElements[2]);
			}

			//The choice has been found, so exit the loop
			break;
		}
	}
}

std::wstring ScriptObject::GetFullPath()
{
	return fs::path(Constants::SCRIPT_ROOT_PATH) / RelativePath;
}

std::wstring ScriptObject::GetRelativePath()
{
	return RelativePath;
}

std::string ScriptObject::PrintScript()
{
	std::ifstream scriptStream(GetFullPath());
	if (!scriptStream.is_open())
	{
		throw std::runtime_error("File unavailable");
	}

	std::ostringstream ss;
	ss << scriptStream.rdbuf();
	return ss.str();
}

bool ScriptObject::IsChoiceScript()
{
	return TopAndBottomChoices.size() != 0;
}

std::string ScriptObject::GetTopChoice()
{
	//Uses at() so it will throw an exception if the user of this code is stupid
	return TopAndBottomChoices.at(0);
}

bool ScriptObject::IsBottomChoiceDefined()
{
	return TopAndBottomChoices.size() == 2;
}

std::string ScriptObject::GetBottomChoice()
{
	return TopAndBottomChoices.at(1);
}



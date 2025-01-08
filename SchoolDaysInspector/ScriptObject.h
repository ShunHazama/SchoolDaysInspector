#pragma once
#include <unordered_map>
#include <string>

struct ScriptIndex
{
	int Route;
	int Scene;
};

class ScriptObject
{
private:
	std::wstring RelativePath;
	std::vector<std::string> TopAndBottomChoices;
	ScriptIndex Index;

	std::wstring GetFullPath();

public:
	ScriptObject(wchar_t* inputBuffer, int routeIndex, int sceneIndex);

	std::string PrintScript();
	bool IsChoiceScript();
	std::string GetTopChoice();
	bool IsSecondChoiceDefined();
	std::string GetBottomChoice();
};
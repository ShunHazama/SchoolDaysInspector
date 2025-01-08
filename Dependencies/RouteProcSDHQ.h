#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" __declspec(dllimport) int __stdcall CheckInputScript(wchar_t* param_1);
extern "C" __declspec(dllimport) int __stdcall CheckScript(int* filmEngine, int chapterIndex);
extern "C" __declspec(dllimport) int __stdcall CheckScriptNo(int* filmEngine);
extern "C" __declspec(dllimport) int __stdcall GetBackScriptFile(FilmEngine* filmEngine, wchar_t* outputBuffer, rsize_t bufferSize);
extern "C" __declspec(dllimport) int __stdcall GetNextScriptFile(FilmEngine* filmEngine, wchar_t* outputBuffer, rsize_t bufferSize);
extern "C" __declspec(dllimport) void __stdcall GetPackFile(int param_1);
extern "C" __declspec(dllimport) int __stdcall GetPackMax();
extern "C" __declspec(dllimport) int __stdcall GetPatchMax();
extern "C" __declspec(dllimport) int __stdcall GetReadScriptCount(int* filmEngine);
extern "C" __declspec(dllimport) int __stdcall GetRouteMapPage(int* filmEngine, int chapterIndex);
extern "C" __declspec(dllimport) int __stdcall GetScriptMax();
extern "C" __declspec(dllimport) int __stdcall GetStory(int* filmEngine);
extern "C" __declspec(dllimport) float __stdcall GetVersionToRoute(int* filmEngine);
extern "C" __declspec(dllimport) void __stdcall LoadInitScript(int* filmEngine);
extern "C" __declspec(dllimport) void __stdcall SetDigScript(HWND param_1, int param_2, int param_3);
extern "C" __declspec(dllimport) void __stdcall SetFeeling(FilmEngine* filmEngine, int alwaysOne);
extern "C" __declspec(dllimport) void __stdcall SetPackName(wchar_t* outputBuffer, size_t bufferSize, wchar_t* param_3, int param_4);
extern "C" __declspec(dllimport) void __stdcall SetRoot(int newRouteIndex, int newSceneIndex, FilmEngine* filmEngine);
extern "C" __declspec(dllimport) void __stdcall SetScript(int routeIndex, int sceneIndex, wchar_t* destBuffer, rsize_t destSize);
extern "C" __declspec(dllimport) void __stdcall ZeroReset(int* filmEngine);
extern "C" __declspec(dllimport) void __stdcall searchRoot(wchar_t* param_1, int param_2, int* filmEngine);
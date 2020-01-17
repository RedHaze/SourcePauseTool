#pragma once

#include <SPTLib\IHookableNameFilter.hpp>

#include "..\..\utils\patterncontainer.hpp"


typedef void(__thiscall* _CUniformRandomStream__SetSeed)(void* thisptr, int seed);
typedef int(__thiscall* _CUniformRandomStream__GenerateRandomNumber)(void* thisptr);
typedef float(__thiscall* _CUniformRandomStream__RandomFloat)(void* thisptr, float flLow, float flHigh);

class VstdlibDLL : public IHookableNameFilter
{
public:
	VstdlibDLL() : IHookableNameFilter({L"vstdlib.dll"}){};
	virtual void Hook(const std::wstring& moduleName,
	                  void* moduleHandle,
	                  void* moduleBase,
	                  size_t moduleLength,
	                  bool needToIntercept);
	virtual void Unhook();
	virtual void Clear();

	static void __thiscall HOOKED_CUniformRandomStream__SetSeed(void* thisptr, int seed);
	static int __thiscall HOOKED_CUniformRandomStream__GenerateRandomNumber(void* thisptr);
	static float __thiscall HOOKED_CUniformRandomStream__RandomFloat(void* thisptr, float flLow, float flHigh);

	void __thiscall HOOKED_CUniformRandomStream__SetSeed_Func(void* thisptr, int seed);
	int __thiscall HOOKED_CUniformRandomStream__GenerateRandomNumber_Func(void* thisptr);
	float __thiscall HOOKED_CUniformRandomStream__RandomFloat_Func(void* thisptr, float flLow, float flHigh);

protected:
	PatternContainer patternContainer;

	_CUniformRandomStream__SetSeed ORIG_CUniformRandomStream__SetSeed;
	_CUniformRandomStream__GenerateRandomNumber ORIG_CUniformRandomStream__GenerateRandomNumber;
	_CUniformRandomStream__RandomFloat ORIG_CUniformRandomStream__RandomFloat;

	int last_seed;
};
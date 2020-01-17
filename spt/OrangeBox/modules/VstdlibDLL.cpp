#include "stdafx.h"

#include "VstdlibDLL.hpp"

#include "..\modules.hpp"
#include "..\patterns.hpp"

#define DEF_FUTURE(name) auto f##name = FindAsync(ORIG_##name, patterns::vstdlib::##name);
#define GET_HOOKEDFUTURE(future_name) \
	{ \
		auto pattern = f##future_name.get(); \
		if (ORIG_##future_name) \
		{ \
			DevMsg("[vstdlib dll] Found " #future_name " at %p (using the %s pattern).\n", \
			       ORIG_##future_name, \
			       pattern->name()); \
			patternContainer.AddHook(HOOKED_##future_name, (PVOID*)&ORIG_##future_name); \
			for (int i = 0; true; ++i) \
			{ \
				if (patterns::vstdlib::##future_name.at(i).name() == pattern->name()) \
				{ \
					patternContainer.AddIndex((PVOID*)&ORIG_##future_name, i, pattern->name()); \
					break; \
				} \
			} \
		} \
		else \
		{ \
			DevWarning("[vstdlib dll] Could not find " #future_name ".\n"); \
		} \
	}

#define GET_FUTURE(future_name) \
	{ \
		auto pattern = f##future_name.get(); \
		if (ORIG_##future_name) \
		{ \
			DevMsg("[vstdlib dll] Found " #future_name " at %p (using the %s pattern).\n", \
			       ORIG_##future_name, \
			       pattern->name()); \
			for (int i = 0; true; ++i) \
			{ \
				if (patterns::vstdlib::##future_name.at(i).name() == pattern->name()) \
				{ \
					patternContainer.AddIndex((PVOID*)&ORIG_##future_name, i, pattern->name()); \
					break; \
				} \
			} \
		} \
		else \
		{ \
			DevWarning("[vstdlib dll] Could not find " #future_name ".\n"); \
		} \
	}

void VstdlibDLL::Hook(const std::wstring& moduleName,
                      void* moduleHandle,
                      void* moduleBase,
                      size_t moduleLength,
                      bool needToIntercept)
{
	Clear(); // Just in case.
	m_Name = moduleName;
	m_Base = moduleBase;
	m_Length = moduleLength;
	patternContainer.Init(moduleName);

	// Init last seed
	last_seed = 0;

	DEF_FUTURE(CUniformRandomStream__SetSeed);
	DEF_FUTURE(CUniformRandomStream__GenerateRandomNumber);
	DEF_FUTURE(CUniformRandomStream__RandomFloat);

	GET_HOOKEDFUTURE(CUniformRandomStream__SetSeed);
	GET_HOOKEDFUTURE(CUniformRandomStream__GenerateRandomNumber);
	GET_HOOKEDFUTURE(CUniformRandomStream__RandomFloat);

	if (!ORIG_CUniformRandomStream__SetSeed)
	{
		Warning("Failed to hook SetSeed!\n");
	}

	if (!ORIG_CUniformRandomStream__GenerateRandomNumber)
	{
		Warning("Failed to hook GenerateRandomNumber!\n");
	}

	if (!ORIG_CUniformRandomStream__RandomFloat)
	{
		Warning("Failed to hook RandomFloat!\n");
	}

	patternContainer.Hook();
}

void VstdlibDLL::Unhook()
{
	patternContainer.Unhook();
	Clear();
}

void VstdlibDLL::Clear()
{
	IHookableNameFilter::Clear();
	ORIG_CUniformRandomStream__SetSeed = nullptr;
}

void __thiscall VstdlibDLL::HOOKED_CUniformRandomStream__SetSeed(void* thisptr, int seed)
{
	TRACE_ENTER();
	return vstdlibDLL.HOOKED_CUniformRandomStream__SetSeed_Func(thisptr, seed);
}

int __thiscall VstdlibDLL::HOOKED_CUniformRandomStream__GenerateRandomNumber(void* thisptr)
{
	TRACE_ENTER();
	return vstdlibDLL.HOOKED_CUniformRandomStream__GenerateRandomNumber_Func(thisptr);
}

float __thiscall VstdlibDLL::HOOKED_CUniformRandomStream__RandomFloat(void* thisptr, float flLow, float flHigh)
{
	TRACE_ENTER();
	return vstdlibDLL.HOOKED_CUniformRandomStream__RandomFloat_Func(thisptr, flLow, flHigh);
}

void __thiscall VstdlibDLL::HOOKED_CUniformRandomStream__SetSeed_Func(void* thisptr, int seed)
{
	if (last_seed != seed)
		Msg("NEW SEED: %d\n", seed);
	last_seed = seed;
	ORIG_CUniformRandomStream__SetSeed(thisptr, seed);
}

int __thiscall VstdlibDLL::HOOKED_CUniformRandomStream__GenerateRandomNumber_Func(void* thisptr)
{
	return ORIG_CUniformRandomStream__GenerateRandomNumber(thisptr);
}

float __thiscall VstdlibDLL::HOOKED_CUniformRandomStream__RandomFloat_Func(void* thisptr, float flLow, float flHigh)
{
	float rand_float = ORIG_CUniformRandomStream__RandomFloat(thisptr, flLow, flHigh);
	Msg("RandomFloat(%f, %f) = %f\n", flLow, flHigh, rand_float);
	return rand_float;
}

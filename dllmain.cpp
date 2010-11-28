#include <windows.h>

#include "AIMP2WebControlHeader.hpp"

void ShowErrorMessage(const char *error);

static AIMP2WebControlHeader header;

extern "C" {
BOOL __declspec(dllexport) WINAPI AIMP_QueryAddonEx(IAIMPAddonHeader **newAddon);
}

BOOL __declspec(dllexport) WINAPI AIMP_QueryAddonEx(IAIMPAddonHeader **newAddon)
{
        *newAddon = new AIMP2WebControlHeader();
        return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

void ShowErrorMessage(const char *error)
{
	MessageBoxA(NULL, error, "Error", MB_ICONERROR);
}


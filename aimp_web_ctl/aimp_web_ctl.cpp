// aimp_web_ctl.cpp: определ€ет экспортированные функции дл€ приложени€ DLL.
//

#include "stdafx.h"
#include "aimp_web_ctl.h"

static AIMP2WebControlHeader header;

// Ёкспортируетс€
BOOL WINAPI AIMP_QueryAddonEx(IAIMPAddonHeader **newAddon)
{
	*newAddon = new AIMP2WebControlHeader();
	return TRUE;
}
#include "AIMP2_SDK.h"
#include "simpleserver.h"

static wchar_t PLG_NAME[] = L"AIMP2: Web Control";
static wchar_t PLG_AUTOR[] = L"Dyatlov Vitaliy";

class AIMP2WebControlHeader
	:public IAIMPAddonHeader
{
private:
	LONG m_cRef;
public:
	AIMP2WebControlHeader()
	{
		m_cRef = 1;
	}

	HRESULT WINAPI QueryInterface (REFIID riid, LPVOID * ppvObj)
	{
		if (!ppvObj)
			return E_INVALIDARG;
		*ppvObj = (LPVOID)this;
		AddRef();
		return NOERROR;
	}

	ULONG WINAPI AddRef()
	{
		m_cRef++;
		return (ULONG)m_cRef;
	}

	ULONG WINAPI Release()
	{
		--m_cRef;
		if (0 == m_cRef)
		{
			delete this;
		}
		return m_cRef;
	}

	BOOL WINAPI GetHasSettingsDialog()
	{
		return TRUE;
	}

	PWCHAR WINAPI GetPluginAuthor()
	{
		return (PWCHAR)PLG_AUTOR;
	}

	PWCHAR WINAPI GetPluginName()
	{
		return (PWCHAR)PLG_NAME;
	}

	void WINAPI Finalize()
	{
		stop_server();
	}

	void WINAPI Initialize(IAIMP2Controller *AController)
	{
		start_server(AController);
	}

	void WINAPI ShowSettingsDialog(HWND AParentWindow)
	{
		MessageBoxW(AParentWindow, L"AIMP2 Web Control plugin is written by Vitaly Dyatlov.\nTo control your player over network just type in browser\nIP address of this computer and port number 38475\nExample: http://192.168.1.100:38475/, or simple http://localhost:38475/", L"Information about AIMP2 Web Control", MB_ICONINFORMATION);
	}
};

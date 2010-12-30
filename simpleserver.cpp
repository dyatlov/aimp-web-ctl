#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <windows.h>
#include <shlwapi.h>

#include "mongoose.h"
#include "dealer.h"
#include "simpleserver.h"
#include "AIMP2_SDK.h"
#include "ConfigFile.h"

static void *pAimpInterface;

int CACHE_TIME = 60;
int UPDATE_TIME = 10;

static const char *ajax_reply =
  "HTTP/1.1 200 OK\r\n"
  "Server: AIMP Web Control by Vitaly Dyatlov\r\n"
  "Cache: no-cache\r\n"
  "Content-Type: application/json\r\n"
  "\r\n";

static void *event_handler(enum mg_event event,
                           struct mg_connection *conn,
                           const struct mg_request_info *request_info)
{
	char *processed = const_cast<char*>("yes");

	if (event == MG_NEW_REQUEST)
	{
		if(request_info->query_string != NULL)
		{
			mg_printf(conn, "%s", ajax_reply);
			std::string response;
			get_response(pAimpInterface, response, request_info->query_string);
			mg_write(conn, response.c_str(), response.size() * sizeof(char));
		}
		else
		{
			processed = NULL;
		}
	}
	else
	{
		processed = NULL;
	}

	return processed;
}

static struct mg_context *ctx;

std::string GetStartPathDll()
{
	TCHAR szPath[MAX_PATH + 1] = {0};
	
	if (!GetModuleFileName(GetModuleHandle("aimp_web_ctl.dll"), szPath, MAX_PATH) || !PathRemoveFileSpec(szPath)) 
	{
		return std::string();
	}
	else
	{
		return std::string(szPath);
	}
}

void start_server(void *inf)
{
	//get plugins path
	std::string path = GetStartPathDll();
	path += "\\aimp_web_ctl\\config.cfg";
	
	std::string port("38475");
	std::string thread_count("5");
	
	try
	{
		ConfigFile config( path );
		config.readInto( port, string("port"), string("38475") );
		config.readInto( thread_count, string("thread_count"), string("5") );
		config.readInto( CACHE_TIME, string("cache_time"), 60);
		config.readInto( UPDATE_TIME, string("update_time"), 10);
	}
	catch(...)
	{
		//do nothing
	}
	
	const char *list_ports_value = port.c_str();
	const char *num_threads_value = thread_count.c_str();
	
	const char *options[] = {
		"document_root", "aimp_web_ctl",
		"listening_ports", list_ports_value,
		"num_threads", num_threads_value,
		NULL
	};

	pAimpInterface = inf;
	ctx = mg_start(&event_handler, NULL, options);
	
	while(ctx == NULL)
	{
		Sleep(1000);
		ctx = mg_start(&event_handler, NULL, options);
		if(MessageBoxA(0, "Port is busy, try again?\r\nMake sure that all AIMP Web Control web-pages are closed and port is free", "AIMP Web Control: Error", MB_RETRYCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON1 | MB_TOPMOST) != IDRETRY)
		{
			MessageBoxA(0, "To enable server you should restart AIMP", "AIMP Web Control: Information", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
			break;
		}
	}
}

void stop_server()
{	
	mg_stop(ctx);
}

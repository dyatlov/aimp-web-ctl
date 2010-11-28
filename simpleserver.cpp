#include "simpleserver.h"

static void *pAimpInterface;

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

static const char *options[] = {
  "document_root", "aimp_web_ctl",
  "listening_ports", "38475",
  "num_threads", "5",
  NULL
};

static struct mg_context *ctx;

void start_server(void *inf)
{
	pAimpInterface = inf;
	ctx = mg_start(&event_handler, options);
	
	while(ctx == NULL)
	{
		Sleep(1000);
		ctx = mg_start(&event_handler, options);
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

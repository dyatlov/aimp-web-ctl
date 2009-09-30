#include "StdAfx.h"
#include "WebController.h"

extern void ShowLastError(LPTSTR lpszFunction);


void CWebController::Answer(SOCKET ClientSocket)
{
	char buf[BUF_SIZE] = {0};
	HANDLE semaphore = NULL;

	if (semaphore = OpenSemaphore(SEMAPHORE_MODIFY_STATE, FALSE, SZ_SEMAPHORE_NAME))
	{
		int count = recv(ClientSocket, buf, sizeof(buf), 0);

		CResponder resp(buf, count, cache.get());
		std::string result = resp.GetResponse();

		send(ClientSocket, result.c_str(), result.length(), 0);
		closesocket(ClientSocket);

		ReleaseSemaphore(semaphore, 1, NULL);
	}
}

void CWebController::Listen(int portNumber)
{
	srv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (srv_sock == INVALID_SOCKET)
	{
		// error message
		return;
	}

	sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	sock_addr.sin_port = htons(portNumber);

	if (bind(srv_sock, (sockaddr*)&sock_addr, sizeof(sock_addr)) == SOCKET_ERROR)
	{
		// error message
		closesocket(srv_sock);
		return;
	}

	if (listen(srv_sock, SOMAXCONN) == SOCKET_ERROR)
	{
		// error message
		closesocket(srv_sock);
		return;
	}

	while(SOCKET ClientSocket = accept(srv_sock, NULL, NULL))
	{
		if(quit == 1)
		{
			closesocket(ClientSocket);
			break;
		}

		boost::thread(boost::bind(&CWebController::Answer, this, ClientSocket));		
	}
	
	closesocket(srv_sock);
}

CWebController::CWebController(IAIMP2Controller *AIMP, int portNumber)
{	
	quit = 0;

	this->portNumber = portNumber;

	cache.reset(new AIMPCache(AIMP, 10000));

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		MessageBox(0 , L"Could not find a usable version of Winsock.dll", L"Error", 0);
    }
	else
	{
		hSemaphore = CreateSemaphore(NULL, SEMAPHORE_MAX_COUNT, SEMAPHORE_MAX_COUNT, SZ_SEMAPHORE_NAME);
		
		if (hSemaphore)
		{
			mainThread.reset(new boost::thread(boost::bind(&CWebController::Listen, this, portNumber)));
		}
		else
		{
			ShowLastError(L"CreateSemaphore");
		}
	}
}

CWebController::~CWebController(void)
{	
	quit = 1;

	closesocket(srv_sock);
	mainThread.get()->join();

	WaitForSingleObject(hSemaphore, WAIT_FOR_THREADS_TIME);
	CloseHandle(hSemaphore);

	WSACleanup();
}
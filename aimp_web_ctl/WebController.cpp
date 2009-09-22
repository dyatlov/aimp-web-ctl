#include "StdAfx.h"
#include "WebController.h"

void CWebController::Answer(SOCKET ClientSocket)
{

	char buf[4096] = {0};
	int count = recv(ClientSocket, buf, sizeof(buf), 0);

	CResponder resp(buf, count, cache.get());
	std::string result = resp.GetResponse();

	send(ClientSocket, result.c_str(), result.length(), 0);
	closesocket(ClientSocket);
	
	{
		boost::mutex::scoped_lock lock(m_mutex);
		this->countWorkers--;
	}
}

void CWebController::Listen(int portNumber)
{
	SOCKET srv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	sock_addr.sin_port = htons(portNumber);

	int result = bind(srv_sock, (sockaddr*)&sock_addr, sizeof(sock_addr));

	listen(srv_sock, SOMAXCONN);
	while(SOCKET ClientSocket = accept(srv_sock, NULL, NULL))
	{
		if(quit == 1)
		{
			closesocket(ClientSocket);
			while(countWorkers != 0)
				Sleep(250);
			Sleep(250);
			break;
		}

		{
			boost::mutex::scoped_lock lock(m_mutex);
			this->countWorkers++;
		}

		boost::thread(boost::bind(&CWebController::Answer, this, ClientSocket));		
	}
	closesocket(srv_sock);
}

CWebController::CWebController(IAIMP2Controller *AIMP, int portNumber)
{	
	quit = 0;
	countWorkers = 0;

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
		mainThread.reset(new boost::thread(boost::bind(&CWebController::Listen, this, portNumber)));
	}
}

CWebController::~CWebController(void)
{	
	quit = 1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in addr;	

	char name[255];
	gethostname(name, 255);	
	hostent *h = gethostbyname(name);

	addr.sin_addr.S_un.S_addr = *(u_long *)h->h_addr_list[0];
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->portNumber);
	connect(sock, (sockaddr*)&addr, sizeof(addr));
	closesocket(sock);

	mainThread.get()->join();

	WSACleanup();
}
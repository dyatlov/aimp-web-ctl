#pragma once

#pragma comment(lib, "Ws2_32.lib")

#include <ws2tcpip.h>
#include <Winsock2.h>

#include "AIMP2_SDK.h"
#include "Responder.h"
#include "AIMPCache.h"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>


#define BUF_SIZE				4096
#define SEMAPHORE_MAX_COUNT		64
#define WAIT_FOR_THREADS_TIME	3000

#define SZ_SEMAPHORE_NAME		L"Local\\ThreadSemaphore"


struct ListenParams
{
	IAIMP2Controller *AIMP;
	int portNumber;
};

struct RequestData
{
	SOCKET sock;
	IAIMP2Controller *AIMP;
};

class CWebController
{
private:
	boost::shared_ptr<boost::thread> mainThread;
    //boost::mutex m_mutex;
	//volatile int countWorkers;
	volatile int quit;
	int portNumber;
	void Answer(SOCKET ClientSocket);
	void Listen(int portNumber);
	boost::shared_ptr<AIMPCache> cache;

	SOCKET srv_sock;
	HANDLE hSemaphore;

public:
	HANDLE hThread;
	CWebController(IAIMP2Controller *AIMP, int portNumber = 38475);
	~CWebController(void);
};

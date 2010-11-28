//
// Multi-reader Single-writer concurrency base class for Win32
//
// (c) 1999-2003 by Glenn Slayden (glenn@glennslayden.com)
//
//

#ifndef MULTIREADERSINGLEWRITER_HPP
#define MULTIREADERSINGLEWRITER_HPP

#include <windows.h>

class MultiReaderSingleWriter
{
private:
	CRITICAL_SECTION m_csWrite;
	CRITICAL_SECTION m_csReaderCount;
	long m_cReaders;
	HANDLE m_hevReadersCleared;

public:
	MultiReaderSingleWriter()
	{
		m_cReaders = 0;
		InitializeCriticalSection(&m_csWrite);
		InitializeCriticalSection(&m_csReaderCount);
		m_hevReadersCleared = CreateEvent(NULL,TRUE,TRUE,NULL);
	}

	~MultiReaderSingleWriter()
	{
		WaitForSingleObject(m_hevReadersCleared,INFINITE);
		CloseHandle(m_hevReadersCleared);
		DeleteCriticalSection(&m_csWrite);
		DeleteCriticalSection(&m_csReaderCount);
	}


	void EnterReader(void)
	{
		EnterCriticalSection(&m_csWrite);
		EnterCriticalSection(&m_csReaderCount);
		if (++m_cReaders == 1)
			ResetEvent(m_hevReadersCleared);
		LeaveCriticalSection(&m_csReaderCount);
		LeaveCriticalSection(&m_csWrite);
	}

	void LeaveReader(void)
	{
		EnterCriticalSection(&m_csReaderCount);
		if (--m_cReaders == 0)
			SetEvent(m_hevReadersCleared);
		LeaveCriticalSection(&m_csReaderCount);
	}

	void EnterWriter(void)
	{
		EnterCriticalSection(&m_csWrite);
		WaitForSingleObject(m_hevReadersCleared,INFINITE);
	}

	void LeaveWriter(void)
	{
		LeaveCriticalSection(&m_csWrite);
	}
};

static MultiReaderSingleWriter concurencyInstance;

#endif //MULTIREADERSINGLEWRITER_HPP


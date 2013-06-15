#include <Windows.h>
#include "..\syncobj.h"


void CCriticalSection::Lock() {
	EnterCriticalSection(&m_csAceess);
}

void CCriticalSection::Unlock() {
	m_fflinet_name = NULL;
	m_lockstate = "unlock";
	LeaveCriticalSection(&m_csAceess);
}

void CCriticalSection::Infolock(const char* ffline_name) {
	EnterCriticalSection(&m_csAceess);
	m_fflinet_name = ffline_name;
	m_lockstate = "locked";
	m_lockthd = GetCurrentThreadId();
}

CCriticalSection::CCriticalSection() {
	InitializeCriticalSection(&m_csAceess);
	m_fflinet_name = NULL;
	m_lockstate = "unlock";
	m_lockthd = 0;
}

CCriticalSection::~CCriticalSection() {
	DeleteCriticalSection(&m_csAceess);
}
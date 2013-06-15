#ifndef __SYNCOBJ_H__
#define __SYNCOBJ_H__

#include <boost/typeof/typeof.hpp>
#include <windows.h>
#include "define.h"

class CNullMutex
{
public:
	CNullMutex(){}
	~CNullMutex(){}
	void Infolock(const char* ffline_name=NULL){}

	void Lock(){}
	void Unlock(){}
};

class CCriticalSection {
	CRITICAL_SECTION m_csAceess;

	const char* m_fflinet_name;
	const char* m_lockstate;
	int m_lockthd;
public:
	CCriticalSection();
	~CCriticalSection();

	void Infolock(const char* ffline_name=NULL);

	void Lock();
	void Unlock();
};


template<class mutex>
class CAutoLockInfoGuad{
	mutex* m_lock;
public:
	CAutoLockInfoGuad(mutex& m,const char* pszInfo) {
		m.Infolock(pszInfo);
		m_lock = &m;
	}
	~CAutoLockInfoGuad() {
		m_lock->Unlock();
	}
};

typedef CCriticalSection CMutex;

#define  AILOCKT(v) CAutoLockInfoGuad<BOOST_TYPEOF(v)> VAR_AILOCK(v,__FF_LINE__);
#define  AILOCKP(v) CAutoLockInfoGuad<BOOST_TYPEOF(*v)> VAR_AILOCK(*v,__FF_LINE__);


#include <list>
template<class T,class mutex = CCriticalSection>
class CSyncList:public std::list<T>,public mutex
{

};

#include <vector>
template<class T,class mutex = CCriticalSection>
class CSyncVector:public std::vector<T>,public mutex
{

};


template<class T,class mutex = CCriticalSection>
class CEventObjList
{
	std::list<T> m_list;
	void Post(const T& event) {
		AILOCKT(this);
		m_list.push_back(event);
	}

	bool Recv(T*& pEvent) {
		AILOCKT(this);
		if (m_list.empty())
			return false;
	}
};









#endif
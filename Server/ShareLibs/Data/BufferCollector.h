#pragma once 

#include <boost/pool/pool.hpp>
#include <boost\thread\mutex.hpp>
#include <list>
#include <set>


class IBufferCollector
{
public:
	virtual void* GetBuffer() = 0;
	virtual void  ReleaseBuffer(void*) = 0;
	virtual size_t BufferSize() = 0;
	virtual ~IBufferCollector() {}
};

class CPool{
	std::list<char*> m_free;
	std::set<char*> m_used;
	size_t m_size;
public:
	CPool(size_t s):m_size(s){};
	~CPool() {
		clear();
	}
	char* malloc() {
		char * szMalloc = 0;
		//if (m_free.empty()){
			szMalloc = new char[m_size];
		//}else {
		//	szMalloc = *m_free.begin();
		//	m_free.pop_front();
		//}
		//if (szMalloc)
		//	m_used.insert(szMalloc);
		return szMalloc;
	}
	void free(char* pMalloc) {
		//if (m_used.find(pMalloc) != m_used.end()) {
		//	m_used.erase(pMalloc);
		//	m_free.push_back(pMalloc);
		//}
		delete []pMalloc;
	}
	size_t size() {
		return m_size;
	}

	void clear() {
		for (std::list<char*>::iterator it = m_free.begin();it != m_free.end(); ++it) {
			char* pMalloc = *it;
			delete []pMalloc;
		}
		for (std::set<char*>::iterator it = m_used.begin();it != m_used.end(); ++it) {
			char* pMalloc = *it;
			delete []pMalloc;
		}
		m_used.clear();
		m_used.clear();
	}
};

class CSessionBufferCollector:public IBufferCollector
{
	//boost::pool<> m_pool;
	CPool m_pool;
	boost::mutex m_mem_lock; 

public:

	void* GetBuffer()
	{
		m_mem_lock.lock();
		void* p = m_pool.malloc();
		m_mem_lock.unlock();
		return p;
	}
	
	void ReleaseBuffer(void* buffer)
	{
		m_mem_lock.lock();
		m_pool.free((char*)buffer);
		m_mem_lock.unlock();
	}

	size_t BufferSize()
	{
		return m_pool.size();
	}

	CSessionBufferCollector(size_t size):m_pool(size){}
};
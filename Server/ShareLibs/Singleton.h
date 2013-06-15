#ifndef _SINGLETON_H
#define _SINGLETON_H
#include <boost\thread\mutex.hpp>
#include <memory>

template<typename T,typename TMutex = boost::mutex>
class Singleton { 
private:
	Singleton(const Singleton&);
	const Singleton& operator=(const Singleton&);
protected:
	static std::auto_ptr<T> m_Singleton;
	Singleton( void ){}
	~Singleton( void ){}
	static TMutex access;
public:
	static T& getMe(){
		if (m_Singleton.get() == NULL) {
			access.lock();
			if (m_Singleton.get() == NULL) { 
				m_Singleton.reset(new T);
			}
			access.unlock();
		}
		return *m_Singleton.get();
	}
};

template<typename T,typename TMutex>
std::auto_ptr<T> Singleton<T,TMutex>::m_Singleton;

template<typename T,typename TMutex>
TMutex Singleton<T,TMutex>::access;

#endif  //_SINGLETON_H__
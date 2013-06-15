#ifndef __IOBJECTPTRMANAGER_H__
#define __IOBJECTPTRMANAGER_H__

#include "global_types.h"
#include <boost/shared_ptr.hpp>
#include <map>

namespace ShareLib{namespace Data{

template<typename V,typename K1>
class IObjectPtrManager
{
public:
	typedef typename boost::shared_ptr<V> VALUEPTR;
private:
	typedef typename std::map<K1,VALUEPTR> MAINKEYMAP;

	MAINKEYMAP m_map1;
protected:
	//CLockerRW m_locker;

	typedef typename IObjectPtrManager<V,K1> THISTYPE;
public:

	typedef typename MAINKEYMAP::const_iterator ITERATOR;

	VALUEPTR operator[] (const K1& k1)
	{
		//AutoLocker_R(m_locker);

		VALUEPTR v;
		VALUEPTR *kk = find_in_map(m_map1,k1);
		if (kk) 
			v = *kk;

		return v;
	}

	inline ITERATOR get_iterator() {return m_map1.begin();}

	inline VALUEPTR walk(ITERATOR& iter)
	{
		VALUEPTR ret;
		if (iter != m_map1.end())
		{
			ret = iter->second;
			iter++;
		}
		return ret;
	}
protected:
	inline MAINKEYMAP& map_obj(){return m_map1;}

	VALUEPTR insert(const K1& k1,const V& value)
	{
		V* pValue = new V();
		*pValue = value;
		return insert(k1,pValue);
	}

	VALUEPTR insert(const K1& k1,V* value)
	{
		VALUEPTR val(value);
		insert(k1,val);
		return val;
	}
	VALUEPTR insert(const K1& k1,VALUEPTR& value)
	{
		//AutoLocker_W(m_locker);
		m_map1[k1] = value;
		return value;
	}

	void remove(const K1& k1)
	{
		//AutoLocker_W(m_locker);
		m_map1.erase(k1);
	}

	void clear()
	{
		//AutoLocker_W(m_locker);
		m_map1.clear();
	}
};

/************************************************************************/
/*                                                                      */
/************************************************************************/


template<typename V,typename K1>
class CObjectPtrManager:protected IObjectPtrManager<V,K1>
{
public:
	typedef typename THISTYPE::ITERATOR ITERATOR;
	typedef typename THISTYPE::VALUEPTR VALUEPTR;

	inline VALUEPTR operator[](const K1& k1)
	{
		return THISTYPE::operator[](k1);
	}

	inline ITERATOR get_iterator() {
		return THISTYPE::get_iterator();
	}
	inline VALUEPTR walk(ITERATOR& iter)
	{
		return THISTYPE::walk(iter);
	}
	template<typename T>
	inline VALUEPTR insert(const K1& k1,T value)
	{
		THISTYPE::insert(k1,value);
	}

	inline void remove(const K1& k1)
	{
		THISTYPE::remove(k1);
	}

	inline size_t Size()
	{
		return map_obj().size();
	}
};

template<typename Y>
class IAutoIDObjectPtrManager:protected CObjectPtrManager<Y,u_long>
{
private:
	u_long id_start_;
public:
	typedef typename THISTYPE::VALUEPTR VALUEPTR;
	typedef typename THISTYPE::ITERATOR ITERATOR;

	IAutoIDObjectPtrManager():id_start_(0){};

	u_long insert(VALUEPTR& val)
	{
		//AutoLocker_W(m_locker);
		u_long id = id_start_;

		while (find_in_map(map_obj(),++id));

		map_obj()[id] = val;
		id_start_ = id;

		return id;
	}

	inline VALUEPTR operator[] (u_long idx)
	{
		return THISTYPE::operator[](idx);
	}

	inline ITERATOR get_iterator()
	{
		return THISTYPE::get_iterator();
	}

	inline VALUEPTR walk(ITERATOR& iter)
	{
		return THISTYPE::walk(iter);
	}

	inline size_t Size()
	{
		return CObjectPtrManager<Y,u_long>::Size();
	}
};

}}
#endif	//__IOBJECTPTRMANAGER_H__
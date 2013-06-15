#ifndef SHARE_DATA_INTERFACE_H
#define SHARE_DATA_INTERFACE_H

#include "common.h"

template<typename T,typename Y>
class IManager
{
protected:
	typedef std::map<T,Y> IManager_Map;
	IManager_Map map_;
public:
	Y* operator[] (T idx)
	{
		return find_in_map(map_,idx);
	}
	
	void insert(T idx,Y& val)
	{
		map_.insert(IManager_Map::value_type(idx,val));
	}

	void insert_always(T idx,Y& val)
	{
		//Y* _val = find_in_map(idx);
		//if (!_val)
		//{
		//	insert(idx,val);
		//}
		//else
		//{
		//	*_val = val;
		//}
		map_[idx] = val;
	}

	void remove(T idx)
	{
		map_.erase(idx);
	}

	IManager_Map& get_map(){return map_;}
};
#endif //SHARE_DATA_INTERFACE_H
#ifndef __DATA_COMMON_H
#define __DATA_COMMON_H

//#include "global_types.h"
//#include <string>
//#include <assert.h>
//#include <boost/noncopyable.hpp>
//#include <boost/enable_shared_from_this.hpp>
//#include <exception>
#include <map>
//#include "BitManager.h"
//#ifndef _DEBUG
//#define ASSERT(x,y) BOOST_ASSERT(x)
//
//#else
//#define ASSERT(x,y) if(!(x)) {throw std::exception(y);}
//#endif



template<typename T,typename Y>
Y* find_in_map(std::map<T,Y>& map,T idx)
{
	std::map<T,Y>::iterator itor = map.find(idx);
	if(itor == map.end())
		return NULL;
	return &itor->second;
}

#endif

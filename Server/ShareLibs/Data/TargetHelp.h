#ifndef _COMMON_TARGETHELP_h_
#define _COMMON_TARGETHELP_h_

#include <FWHLIB.h>
#include "../StaticConfigs/enums_define.h"
namespace game_server{ namespace messages{

	using namespace std;
	using namespace FWHLib::Data;
	using namespace game_server::enums_define;

	template<typename Tz = NullTarget>
	struct UnknownTarget:public Tz{};
	template <typename T,typename Tz>
	T& operator << (T &dataStream, const UnknownTarget<Tz>& value){
		dataStream <<value.Type
			<<(Tz&)value;
		return dataStream;
	}


	

	//////////////////////////////////////////////////////////////////////////
	template<int TypeId>
	struct TargetUuid{
		static const u_long Type=TypeId;
		boost_uuid ID;
	};
	template <int TypeId,typename T>
	T& operator << (T &dataStream, const TargetUuid<TypeId>& value){
		if (value.ID.is_nil())
		{
			return dataStream;
		}
		dataStream//<<value.Type
			<<value.ID;

		return dataStream;
	}

	template <int TypeId>
	inline CStreamData& operator >> (CStreamData& dataStream, TargetUuid<TypeId>& value){
		memset(&value.ID,0,value.ID.static_size());
		if(dataStream.eof())
			return dataStream;

		dataStream>>value.ID;
		return dataStream;
	}


}}
#endif // _COMMON_TARGETHELP_h_

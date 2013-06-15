#include "typestream.h"


namespace ShareLib {namespace Data {

#define StreamInOut(type)\
	CTypeStream& operator << (CTypeStream &dataStream, const type value)\
	{\
		dataStream.set_value<type>(value);\
		return dataStream;\
	}\
	CTypeStream& operator >> (CTypeStream &dataStream, type& value)\
	{\
		value = dataStream.get_value<type>();\
		return dataStream;\
	}

	StreamInOut(u_long);
	StreamInOut(long);
	StreamInOut(char);
	StreamInOut(u_char);
	StreamInOut(u_int);
	StreamInOut(int);
	StreamInOut(bool);
	StreamInOut(u_short);
	StreamInOut(short);
#undef StreamInOut

	CTypeStream& operator<<( CTypeStream &dataStream, const std::string& str_value )
	{
		u_short size = str_value.size();
		dataStream << size;
		
		dataStream.set_bytes(str_value.data(),size);
		return dataStream;
	}

	CTypeStream& operator>>( CTypeStream &dataStream, std::string& str_value )
	{
		u_short size;
		dataStream >> size;

		void *src = dataStream.get_bytes(size);
		str_value.resize(size);
		memcpy((void*)str_value.data(),src,size);
		return dataStream;
	}

}};
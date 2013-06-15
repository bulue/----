#ifndef __SERVER_MESSAGE_H__
#define __SERVER_MESSAGE_H__

#include "ShareLib.h"

namespace Server{namespace Messages{

	using namespace ShareLib::Data;
	typedef std::vector<u_long> ULongArray;

	struct LogIn
	{
		static const u_long MSGID = 1000;
		std::string name;
		std::string pwd;
	};

	inline CTypeStream& operator << (CTypeStream& dataStream,const LogIn &value)
	{
		dataStream << value.name
			<< value.pwd;
		return dataStream;
	}

	inline CTypeStream& operator >> (CTypeStream& dataStream,LogIn &value)
	{
		dataStream >> value.name
			>>value.pwd;
		return dataStream;
	}

	typedef CMessage<LogIn> CMsg_LogIn;

	struct LogOut
	{
		static const u_long MSGID = 1001;
		u_long Result;
	};

	inline CTypeStream& operator << (CTypeStream& dataStream,const LogOut &value)
	{
		dataStream << value.Result;
		return dataStream;
	}

	inline CTypeStream& operator >> (CTypeStream& dataStream,LogOut &value)
	{
		dataStream >> value.Result;
		return dataStream;
	}

	typedef CMessage<LogOut> CMsg_LogOut;


	struct PMmap
	{
		static const u_long MSGID = 2000;
	};

	template <typename T> 
	T& operator << (T &dataStream, const PMmap& value){
		dataStream;
		return dataStream;
	}

	inline CTypeStream& operator >> (CTypeStream& dataStream, PMmap& value){
		dataStream;
		return dataStream;
	}

	typedef CMessage<PMmap> CMsg_PMmap;

	struct PMmapReply
	{
		static const u_long MSGID = 7000;
		u_long result ;
		ULongArray Map ;
	};

	template <typename T> 
	T& operator << (T &dataStream, const PMmapReply& value){
		dataStream<<value.result
			<<value.Map
			;
		return dataStream;
	}

	inline CTypeStream& operator >> (CTypeStream& dataStream, PMmapReply& value){
		dataStream>>value.result
			>>value.Map
			;
		return dataStream;
	}

	typedef CMessage<PMmapReply> CMsg_PMmapReply;
}}

#endif	//__SERVER_MESSAGE_H__
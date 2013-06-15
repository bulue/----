#pragma once 

#include "typestream.h"
#include "../Server/connection.h"

#define SRUTIL_DELEGATE_PREFERRED_SYNTAX
#include "../srutil/delegate/delegate.hpp"

namespace ShareLib {namespace Data{

typedef ShareLib::Server::CSession CSession;


class IMsgFun
{
public:
	virtual bool dispatch(CSession&,void* buff,size_t ) = 0;
};


template<typename MsgType>
class CMsgFun:public IMsgFun
{
	typedef template<typename ObjType> srutil::delegate<bool (MsgType)> delegate;
public:
	virtual bool dispatch(CSession&,void* buff,size_t size)
	{
		using namespace ShareLib::Server;
		MsgType msg;

		CTypeStream dataStream(buff,size,0);
		dataStream >> msg;

		return m_delegate(msg);
	}

	
	delegate m_delegate;
};





}}
#ifndef __CLIENT_SESSION_H__
#define __CLIENT_SESSION_H__

#include "network/tcpsocket.h"
#include "syncobj.h"

class CClientSession:public CTcpSocketBase
{
public:
	CClientSession();

	virtual void start();

	virtual bool msgParse(char* cmd,unsigned int len);

	virtual void do_delete_self(const boost::system::error_code& error);

	void close();

	bool boCloseByHand;
};


#endif
#ifndef __SERVER_H__
#define __SERVER_H__

#include "Connection.h"
#include <map>

namespace ShareLib{namespace Server{

class CServer:public CSessionManager
{
public:
	CServer(short port,GETNEWSESSION pFunGetSsn):
		CSessionManager(pFunGetSsn),
		acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	{
		session_ptr new_session(new CSession(static_cast<CSessionManager&>(*this)));
		acceptor_.async_accept(new_session->socket(),
			boost::bind(&CServer::handle_accept, this, new_session,
			boost::asio::placeholders::error));
	}

	void handle_accept(session_ptr new_session,
		const boost::system::error_code& error)
	{
			if (!error)
			{
				//LogInst.DebugInfo("New session {%s} connected...\n",new_session->remote_address().c_str());
				ISessionManager::insert(new_session);
				new_session->start();
				new_session.reset(new CSession(static_cast<CSessionManager&>(*this)));
				acceptor_.async_accept(new_session->socket(),
					boost::bind(&CServer::handle_accept, this, new_session,
					boost::asio::placeholders::error));
			}
	}

	template<class Tx,class Ty,class Tz,class Tu>
	inline void add_message_map(Ty* Obj,u_long (Ty::*exec)(Tx&,const CMessage<Tz,Tu>&))
	{
		IMsgMapManager::insert(Tz::MSGID,CMSGFUN(Obj,exec));
	}

private:
	boost::asio::ip::tcp::acceptor acceptor_;
};

}}

#endif//__SERVER_H__
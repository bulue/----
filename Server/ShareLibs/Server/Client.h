#include "connection.h"

namespace ShareLib{namespace Server {



class CClient : public CSessionManager
{
public:
	CClient():CSessionManager(&CClient::getSession) {}
	explicit CClient(GETNEWSESSION creator):CSessionManager(creator){}

	template<class Tx,class Ty,class Tz,class Tu>
	inline void add_message_map(Ty* Obj,u_long (Ty::*exec)(Tx&,const CMessage<Tz,Tu>&))
	{
		IMsgMapManager::insert(Tz::MSGID,CMSGFUN(Obj,exec));
	}

	void add_client(std::string ip,unsigned short port)
	{
		using namespace boost::asio::ip;

		//session_ptr session = session_ptr(new CSession(static_cast<CSessionManager&>(*this)));
		session_ptr session = session_ptr((*SsnCreator)(static_cast<CSessionManager&>(*this)));
		session->async_connect(ip,port);
		ISessionManager::insert(session);
	}

	void send_msg(IMessage* msg)
	{
		ISessionManager::ITERATOR iter = ISessionManager::get_iterator();
		for (ISessionManager::VALUEPTR value = ISessionManager::walk(iter);value;value = walk(iter))
		{
			value->send_msg(msg);
		}
	}

	static CSession* getSession(CSessionManager& ssnMgr) 
	{
		return new CSession(ssnMgr);
	}
};




}}
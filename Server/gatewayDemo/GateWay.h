#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "network/packet.h"
#include "Log.h"
#include "syncobj.h"
#include "Singleton.h"
#include "network/packet.h"
#include "network/tcpsocket.h"
#include <set>

using namespace boost::asio::ip;
using namespace boost::asio;

extern io_service g_ioservice;
extern CMutex g_logmutex;

class CEngine;
class CServerConnection;
class CGateWayConnection;
class CGateWayServer;


class CServerConnection:public CTcpSocketBase{
	bool msgParse(char* basemsg,unsigned int len);
	void do_delete_self(const boost::system::error_code& error);
public:
	CServerConnection(io_service& ios):CTcpSocketBase(ios) {}
};

class CGateWayConnection:public CTcpSocketBase {
public:
	bool msgParse(char* cmd,unsigned int len);
	void do_delete_self(const boost::system::error_code& error);
	void start();
	CGateWayConnection(io_service& ios):CTcpSocketBase(ios){}
};

class CGateWayServer{
	tcp::acceptor m_acceptor;
public:
	void start_accept() {
		CGateWayConnection* pConnection =  new CGateWayConnection(g_ioservice);
		m_acceptor.async_accept(pConnection->socket(),
			boost::bind(&CGateWayServer::handle_accept,this,pConnection,_1));
	}
	void handle_accept(CGateWayConnection* pconn,const boost::system::error_code& error){
		if (!error){
			pconn->start();
			start_accept();
		}else{
			delete pconn;
		}
	}

	CGateWayServer(io_service& ios,short port):m_acceptor(ios,tcp::endpoint(tcp::v4(),port)){
		start_accept();
	}

	std::set<CGateWayConnection*> m_connections;
};



class CEngine:public Singleton<CEngine> {
public:
	CGateWayServer m_gatewaysvr;
	CServerConnection *m_pSvrConn;

	CEngine():m_pSvrConn(0),m_gatewaysvr(g_ioservice,27000){

	}

	void StartUp() {
		m_gatewaysvr.start_accept();
		ConnectSvr();
	}
	void ShutDown() {

	}

	void ConnectSvr() {
		m_pSvrConn = new CServerConnection(g_ioservice);
		m_pSvrConn->async_connect("127.0.0.1",27015);
	}
};
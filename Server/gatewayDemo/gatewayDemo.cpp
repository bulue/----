#include "GateWay.h"
#include "syncobj.h"

io_service g_ioservice;
CMutex g_logmutex;

void __stdcall ShowLogFunc(CLogger::Level& level,const char* logtime,const char* pszMsg){
	AILOCKT(g_logmutex);
	//printf_s("%s %s\n",logtime,pszMsg);
	printf_s("%s\n",pszMsg);
}


int main() {
	try
	{
		//CGateWayServer gatewayserver(g_ioservice,27000);
		CEngine::getMe().StartUp();
		g_ioservice.run();
	}
	catch (std::exception& e)
	{
		g_logger.error("exception %s",e.what());
	}
	catch(...) {
		g_logger.error("unkown exception");
	}
	return EXIT_SUCCESS;
}

bool CServerConnection::msgParse( char* basemsg,unsigned int len )
{
	g_logger.info("Svr Conn recv [%d]",len);
	stProxyMessage* proxymsg = (stProxyMessage*)basemsg;
	CGateWayConnection* pcon = (CGateWayConnection*)proxymsg->iproxid;
	if (CEngine::getMe().m_gatewaysvr.m_connections.find(pcon) != CEngine::getMe().m_gatewaysvr.m_connections.end()){
		BUFFER_CMD(stBaseMessage,sendmsg,stBaseMessage::_MAX_MSG_SIZE);
		sendmsg->pushbuffer(proxymsg->buffer(),proxymsg->bufferlen());
		pcon->write((char*)sendmsg,sendmsg->alllen());
	}
	return true;
}

void CServerConnection::do_delete_self(const boost::system::error_code& error)
{
	delete this;
	CEngine::getMe().m_pSvrConn = NULL;
	g_logger.info("server connection disconnect...try to connect again...");
	CEngine::getMe().ConnectSvr();
}

void CGateWayConnection::start()
{
	CEngine::getMe().m_gatewaysvr.m_connections.insert(this);
	CTcpSocketBase::start();
}

bool CGateWayConnection::msgParse( char* basemsg,unsigned int len )
{
	g_logger.info("Gtw Conn recv [%d]",len);
	stBaseMessage* pMsg= (stBaseMessage*)basemsg;
	BUFFER_CMD(stProxyMessage,proxymsg,stBaseMessage::_MAX_MSG_SIZE);
	proxymsg->iproxid = (int)this;
	proxymsg->pushbuffer(pMsg->body(),pMsg->bodylen());
	if (CEngine::getMe().m_pSvrConn) 
		CEngine::getMe().m_pSvrConn->write((char*)proxymsg,proxymsg->getallsize());
	return true;
}

void CGateWayConnection::do_delete_self(const boost::system::error_code& error)
{
	g_logger.debug("error_code  = [%s]",error.message().c_str());
	CEngine::getMe().m_gatewaysvr.m_connections.erase(this);
	delete this;
}

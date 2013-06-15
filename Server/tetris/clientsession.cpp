#include "tetrisFrame.h"
#include "clientsession.h"
#include "tetris_global.h"
#include "CltTetrisEngine.h"

CClientSession::CClientSession()
	:CTcpSocketBase(CltTetrisEngine::getMe().io_s)
{
	boCloseByHand = false;
}

void CClientSession::start()
{
	__super::start();
	CltTetrisEngine::getMe().m_ClientSession = this;
}

bool CClientSession::msgParse( char* cmd,unsigned int len )
{
	AILOCKT(CltTetrisEngine::getMe().m_SyncMsgList);
	stBaseMessage* bmsg = (stBaseMessage*)cmd;
	CltTetrisEngine::getMe().m_SyncMsgList.push_back(bmsg->clone());
	return true;
}

void CClientSession::do_delete_self( const boost::system::error_code& error )
{
	if (!boCloseByHand) {
		printf("����������ӶϿ�....\n");
		wxMessageBox(wxT("����������ӶϿ�"),wxT("����˹����"));
	}
	CltTetrisEngine::getMe().m_ClientSession = NULL;
	SF_DELETE this;
}

void CClientSession::close()
{

	boCloseByHand = true;
	__super::close();
}



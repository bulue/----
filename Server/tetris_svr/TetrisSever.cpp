#include "TetrisSever.h"
#include "TetrisEngine.h"
#include "Shape.h"
#include "tetris_global.h"



session::session( io_service& ios ):CTcpSocketBase(ios)
{
	m_player = NULL;
}

void session::start()
{
	__super::start();
	g_logger.debug("新链接来自:%s",__super::socket().remote_endpoint().address().to_string().c_str());
}

bool session::msgParse( char* msg,unsigned int len )
{
	stBaseMessage *pmsg = (stBaseMessage *)msg;
	stBaseCmd* pcmd = (stBaseCmd*)pmsg->body();
	switch (pcmd->cmd) {
	case _SYS_LOGIN_CMD:
		{
			switch (pcmd->value) {
			case stUserRegist::_value:
				{
					stUserRegist* pDstCmd = (stUserRegist*)pcmd;

				}break;
			case stUserLogin::_value:
				{
					stUserLoginRet retcmd;

					stUserLogin* pDstCmd = (stUserLogin*)pcmd;
					CPlayer* pUser = TetrisEngine::getMe().m_PlayerManage.FindByName(pDstCmd->szAccount);
					if (!pUser) {
						pUser = new CPlayer(TetrisEngine::getMe().GeneratePlayerId());
						strcpy_s(pUser->szAccount,sizeof(pUser->szAccount),pDstCmd->szAccount);
						strcpy_s(pUser->szPwd,sizeof(pUser->szPwd),pDstCmd->szPwd);
						pUser->m_session = this;
						if ( TetrisEngine::getMe().m_PlayerManage.addValue(pUser) )
							retcmd.btError = 0;
					}else {
						if ( memcmp(pUser->szPwd,pDstCmd->szPwd,sizeof(pUser->szPwd) == 0) ){
							retcmd.btError = 0;
						}else
							retcmd.btError = 1;
					}
					if (retcmd.btError == 0) {
						m_player = pUser;
						g_logger.debug("玩家[%s][%d]登陆成功",pUser->szAccount,pUser->nId);
					}
					this->write((stBaseCmd*)&retcmd,sizeof(retcmd));
				}break;
			};
		}break;
	default:
		{
			if (m_player) {
				stBaseMessage* pcopymsg =  m_player->CopyMsg(pmsg);
				if (pcopymsg) {
					AILOCKT(m_player->m_lmsg);
					m_player->m_lmsg.push_back(pcopymsg);
				}
			}
		}
	}
	return true;
}

void session::do_delete_self( const boost::system::error_code& error )
{
	if (m_player) {
		m_player->m_session = NULL;
		if (m_player->m_GameRoom)
			m_player->m_GameRoom->LeaveRoom(m_player);
		g_logger.debug("用户:[%s],来自[%s],断开.",m_player->szAccount,this->socket().remote_endpoint().address().to_string().c_str());
	}else {
		g_logger.debug("用户:[NULL]来自[%s],断开.",this->socket().remote_endpoint().address().to_string().c_str());
	}
	delete this;
}


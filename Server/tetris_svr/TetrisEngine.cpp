#include "TetrisEngine.h"
#include "tetris_global.h"
#include <boost/thread.hpp>

TetrisEngine::TetrisEngine()
{
	m_nPlayerStartId = _USER_ID_BEGIN;
	for (int i = 0; i < _GAME_ROOM_MAX; ++i) {
		CGameRoom* pGameRoom = SF_NEW CGameRoom();
		pGameRoom->nId = i;
		m_RoomManage.addValue(pGameRoom);
	}
}

TetrisEngine::~TetrisEngine()
{
	CRoomManage::iterator it;
	for (it = m_RoomManage.begin();it != m_RoomManage.end(); ++it) {
		CGameRoom* pGameRoom = it->second;
		if (pGameRoom) {
			SF_DELETE(pGameRoom);
		}
	}
	m_RoomManage.clear();
}

void TetrisEngine::Run()
{
	boost::thread thread(boost::bind(&io_service::run,&io_s));

	while (true) {
		ProcessUserMsg();
		ProcessTimer();
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));
	}

	thread.join();
}


void TetrisEngine::ProcessUserMsg()
{
	AILOCKT(m_PlayerManage);
	CPlayerManage::iterator it;
	for (it = m_PlayerManage.begin();it != m_PlayerManage.end();it++)
	{
		CPlayer * pUser = it->second;
		if (pUser ) {
			AILOCKT(pUser->m_lmsg);
			CPlayer::CSyncMsgList::iterator it;
			for (it = pUser->m_lmsg.begin();it != pUser->m_lmsg.end(); ++it){
				stBaseMessage* pMsg = *it;
				pUser->DoUserCmd((stBaseCmd*)pMsg->body(),pMsg->bodylen());	
				delete pMsg;
			}
			pUser->m_lmsg.clear();
		}
	}
}


void TetrisEngine::ProcessTimer()
{
	if (GetTickCount() - dwLastTime > 500) {
		AILOCKT(m_PlayerManage);
		CPlayerManage::iterator it;
		for (it = m_PlayerManage.begin(); it != m_PlayerManage.end(); ++it) {
			CPlayer* puser = it->second;
			if (puser) {
				puser->m_board.OnTimer();
			}
		}
		dwLastTime = GetTickCount();
	}
}

int TetrisEngine::GeneratePlayerId()
{
	return BOOST_INTERLOCKED_INCREMENT((volatile long*)&m_nPlayerStartId);
}




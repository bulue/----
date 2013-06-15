#include "CltTetrisEngine.h"
#include "tetris_global.h"


CltTetrisEngine::CltTetrisEngine()
{
	m_SyncMsgList.clear();
	m_WorkThreads.clear();
	m_LoginDialog = NULL;
	m_ClientSession = NULL;
	m_TetrisFrame = NULL;
	m_dwLastTime = 0;
	m_tLastTime =  0;
	m_GameHallFrame = NULL;
	m_boRun = false;
}

CltTetrisEngine::~CltTetrisEngine()
{
	ShutDown();
}

void CltTetrisEngine::ProcessSyncMsg()
{
	AILOCKT(m_SyncMsgList);
	for (CSyncMsgList::iterator it = m_SyncMsgList.begin(); it != m_SyncMsgList.end(); ++it)
	{
		stBaseMessage* bmsg = *it;
		stBaseCmd* pcmd = (stBaseCmd*)bmsg->body();
		int nLen = bmsg->bodylen();
		switch (pcmd->cmd) {
		case _SYS_LOGIN_CMD:
			{
				switch (pcmd->value){
				case stUserLoginRet::_value:
					{
						stUserLoginRet* pDestCmd = (stUserLoginRet*)pcmd;
						if (pDestCmd->btError == 0) {
							m_LoginDialog->EndModal(0);
						}
					}break;
				}
			}break;
		case _TRITRIS_GAME_CMD:
			{
				if (m_TetrisFrame)
				{
					switch (pcmd->value) {
					case stTetrisRefresh::_value:
						{
							stTetrisRefresh* pDstCmd = (stTetrisRefresh*)pcmd;
							if (pDstCmd->isMe)
								m_TetrisFrame->m_ClientBoard->m_LogicBoard.DoLogicBoardCmd(pcmd,nLen);
							else 
								m_TetrisFrame->m_ClientBoard1->m_LogicBoard.DoLogicBoardCmd(pcmd,nLen);
						}break;
					case stTetrisDrawText::_value:
						{
							stTetrisDrawText* pDstCmd = (stTetrisDrawText*)pcmd;
							if (pDstCmd->isMe)
								m_TetrisFrame->m_ClientBoard->m_LogicBoard.DoLogicBoardCmd(pcmd,nLen);
							else 
								m_TetrisFrame->m_ClientBoard1->m_LogicBoard.DoLogicBoardCmd(pcmd,nLen);
						}break;
					default:
						{
							m_TetrisFrame->m_ClientBoard->m_LogicBoard.DoLogicBoardCmd(pcmd,nLen);
						}
					}
				}	
			}break;
		case _GAME_ROOM_CMD:
			{
				switch (pcmd->value) {
				case stGameRoomViewRet::_value:
					{
						stGameRoomViewRet * pDstCmd = (stGameRoomViewRet*)pcmd;
						AILOCKT(m_RoomList);
						m_RoomList.clear();
						for (int i = 0;i < pDstCmd->RoomInfos.size; ++i) {
							m_RoomList.push_back(pDstCmd->RoomInfos[i]);
						}
						m_GameHallFrame->Visit(&m_RoomList);
					}break;
				case stGameRoomInter::_value:
					{
						stGameRoomInter* pDstCmd = (stGameRoomInter*)pcmd;
						if (pDstCmd->nErrorCode == 0) {
							m_GameHallFrame->OnRoomEnter();
						}
					}break;
				}
			}break;
		}
		SF_FREE(bmsg);
	}

	m_SyncMsgList.clear();
}

void CltTetrisEngine::Run()
{
	m_boRun = true;
	while (m_boRun) {
		ProcessSyncMsg();
		boost::this_thread::sleep(boost::posix_time::millisec(10));
	}
}

void CltTetrisEngine::Stop()
{
	m_boRun = false;
}

void CltTetrisEngine::ShutDown()
{
	Stop();
	do {
		AILOCKT(m_SyncMsgList);
		for (CSyncMsgList::iterator it = m_SyncMsgList.begin(); it != m_SyncMsgList.end(); ++it)
		{
			stBaseMessage* bmsg = *it;
			SF_FREE(bmsg);
		}
		m_SyncMsgList.clear();
	} while (false);

	if (m_ClientSession)
		m_ClientSession->close();

	for (size_t i  = 0;i < m_WorkThreads.size(); ++i )
	{
		m_WorkThreads[i].join();
	}
}

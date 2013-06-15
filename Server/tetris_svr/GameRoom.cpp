#include "GameRoom.h"
#include "TetrisEngine.h"

CGameRoom::CGameRoom()
{
	nId = 0;
	for (int i = 0; i < _ROOM_PLAYER_MAX; ++i) {
		m_vPlayer[i] = NULL;
	}
}

bool CGameRoom::EnterRoom( CPlayer* _player)
{
	for (int i = 0; i < _ROOM_PLAYER_MAX; ++i) {
		if (m_vPlayer[i] == NULL) {
			m_vPlayer[i] = _player;
			_player->m_GameRoom = this;
			return true;
		}
	}
	return false;
}

void CGameRoom::LeaveRoom( CPlayer* _player)
{
	for (int i = 0; i < _ROOM_PLAYER_MAX; ++i) {
		if (m_vPlayer[i] == _player){
			m_vPlayer[i] = NULL;
			_player->m_GameRoom = NULL;
		}
	}
}

bool CRoomManage::doRoomCmd( CPlayer* pUser,stBaseCmd* pcmd,int nLen )
{
	switch(pcmd->value) {
	case stGameRoomView::_value:
		{
			stGameRoomView* pDstCmd = (stGameRoomView*)pcmd;
			BUFFER_CMD(stGameRoomViewRet,retcmd,stBaseMessage::_MAX_MSG_SIZE);
			retcmd->nId = pDstCmd->nId;
			for (iterator it = this->begin(); it != this->end(); ++it) {
				const CGameRoom *pRoom = it->second;
				stRoomInfo tmp;
				tmp.nId = pRoom->nId;
				if (pRoom->m_vPlayer[0]) 
					strcpy_s(tmp.szName1,sizeof(tmp.szName1)-1,pRoom->m_vPlayer[0]->szAccount);
				if (pRoom->m_vPlayer[1]) 
					strcpy_s(tmp.szName2,sizeof(tmp.szName2)-1,pRoom->m_vPlayer[1]->szAccount);
				retcmd->RoomInfos.push_back(tmp);
			}
			pUser->SendToMe((stBaseCmd*)retcmd,sizeof(stGameRoomViewRet)+retcmd->RoomInfos.getarraysize());
		}break;
	case stGameRoomInter::_value:
		{
			stGameRoomInter* pDstCmd = (stGameRoomInter*)pcmd;
			stGameRoomInter retcmd;
			memcpy(&retcmd,pDstCmd,sizeof(retcmd));
			CGameRoom* pRoom = this->FindById(pDstCmd->nRoomId);
			if (pRoom) {
				if (pDstCmd->nSeat >= 0 && pDstCmd->nSeat < 2) {
					if (pRoom->m_vPlayer[pDstCmd->nSeat] == NULL) {
						pRoom->EnterRoom(pUser);
						retcmd.nErrorCode = 0;
					}else retcmd.nErrorCode = 1;
				}else retcmd.nErrorCode = 2;
			}else retcmd.nErrorCode = 3;
			pUser->SendToMe((stBaseCmd*)&retcmd,sizeof(stGameRoomInter));
		}break;
	case stGameRoomLeave::_value:
		{
			stGameRoomLeave* pDstCmd = (stGameRoomLeave*)pcmd; 
			
		}
	}

	return true;
}

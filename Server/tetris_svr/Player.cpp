#include "Player.h"
#include "TetrisEngine.h"



bool CPlayer::DoUserCmd( stBaseCmd* pcmd,int nLen )
{
	switch (pcmd->cmd)
	{
	case _TRITRIS_GAME_CMD:
		{
			m_board.DoLogicBoardCmd(pcmd,nLen);
		}break;
	case _GAME_ROOM_CMD:
		{
			TetrisEngine::getMe().m_RoomManage.doRoomCmd(this,pcmd,nLen);
		}
	}
	return true;
}

void CPlayer::SendToMe( stBaseCmd* pcmd,int nLen )
{
	if (m_session) m_session->write(pcmd,nLen);
}

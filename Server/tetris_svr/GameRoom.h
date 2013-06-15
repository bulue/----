#ifndef __GAME_ROOM_H_
#define __GAME_ROOM_H_


#include "HashManage.h"
#include "tetris_global.h"

#define _ROOM_PLAYER_MAX 2

class CPlayer;

class CGameRoom
{
public:
	CGameRoom();
	~CGameRoom(){}
	bool EnterRoom(CPlayer* );
	void LeaveRoom(CPlayer* );

	
	int nId;
	CPlayer* m_vPlayer[_ROOM_PLAYER_MAX];
};

struct stLimitHashRoomId:public LimitHash<int,CGameRoom*>
{
	static __inline int mhkey(CGameRoom* &e)
	{	 
		return e->nId;
	}
};

class CRoomManage:public zLHashManager3<CGameRoom*,
	stLimitHashRoomId
>
{
public:
	CGameRoom* FindById(int _nId)
	{
		return m_e1.find(_nId);
	}
	bool doRoomCmd(CPlayer* pUser,stBaseCmd* pcmd,int nLen);
};



#endif
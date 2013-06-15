#ifndef __TETRIENGINE_H__
#define __TETRIENGINE_H__

#include "TetrisSever.h"
#include "Singleton.h"
#include "SvrBoard.h"
#include "syncobj.h"
#include "Log.h"
#include "HashManage.h"
#include "mem_allocate.h"
#include "GameRoom.h"
#include "Player.h"


#include <set>
#include <map>

#define _USER_ID_BEGIN 3200
#define _GAME_ROOM_MAX 10

class TetrisEngine
	:public Singleton<TetrisEngine>
{
public:
	TetrisEngine();
	~TetrisEngine();
	io_service io_s;

	DWORD dwLastTime;
	volatile int  m_nPlayerStartId;
	CPlayerManage m_PlayerManage;
	CRoomManage   m_RoomManage;

	int GeneratePlayerId();

	void ProcessUserMsg();
	void ProcessTimer();

	void Run();

};


#endif
#ifndef _USER_H__
#define _USER_H__

#include "Singleton.h"
#include "HashManage.h"
#include "SvrBoard.h"

class CGameRoom;
class session;

class CPlayer
{
public:
	CPlayer(int _nId):nId(_nId),m_session(NULL),m_GameRoom(NULL){m_board.SetUser(this);}
	int nId;

	char szAccount[48];
	char szPwd[48];

	CLogicBoad m_board;

	session* m_session;
	CGameRoom* m_GameRoom;

	void SendToMe(stBaseCmd* pcmd,int nLen);

	typedef CSyncList<stBaseMessage*> CSyncMsgList ;
	CSyncMsgList m_lmsg;

	stBaseMessage* CopyMsg(stBaseMessage* _pmsg) {
		void* p = SF_MALLOC(_pmsg->alllen());
		memcpy(p,_pmsg,_pmsg->alllen());
		return (stBaseMessage*)p;
	}

	void ReleaseMsg(stBaseMessage* _pmsg) {
		SF_FREE((void*)_pmsg);
	}

	bool DoUserCmd(stBaseCmd* pcmd,int nLen);
};


struct stLimitHashPlayerId:public LimitHash<int,CPlayer*>
{
	static __inline int mhkey(CPlayer* &e)
	{	 
		return e->nId;
	}
};

struct stLimitHashPlayerName:public  LimitHash<std::string,CPlayer*>
{
	static __inline std::string mhkey(CPlayer* &e)
	{	 
		return e->szAccount;
	}
};

class CPlayerManage:public zLHashManager3<CPlayer*,
	stLimitHashPlayerId,
	stLimitHashPlayerName
>
{
public:
	CPlayer* FindById(int _nId)
	{
		return m_e1.find(_nId);
	}

	CPlayer* FindByName(const char* _szName)
	{
		return m_e2.find(_szName);
	}
};




#endif  //_USER_H__
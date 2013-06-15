#ifndef _CLTTETRISENGINE_H__
#define _CLTTETRISENGINE_H__

#include "LoginDialog.h"
#include "tetrisFrame.h"
#include "syncobj.h"
#include "Singleton.h"
#include "clientsession.h"
#include "RoomVector.h"
#include "GameHallUI.h"

#include <vector>
#include <boost/thread.hpp>
#include <ctime>


class CltTetrisEngine
	:public Singleton<CltTetrisEngine>
{
public:
	CltTetrisEngine();
	~CltTetrisEngine();

	typedef CSyncList<stBaseMessage* > CSyncMsgList;

	LoginDialog *	m_LoginDialog;
	TetrisFrame*	m_TetrisFrame;
	GameHallFrame*	m_GameHallFrame;

	CSyncMsgList	m_SyncMsgList;
	CRoomVector		 m_RoomList;		//��Ϸ����
	

	io_service	io_s;		
	bool		m_boAlone;	//����
	bool		m_boRun;	//����
	DWORD		m_dwLastTime;
	clock_t		m_tLastTime;

	std::vector<boost::thread> m_WorkThreads; //�����߳�
	CClientSession* m_ClientSession;

	void Run();
	void Stop();
	void ShutDown();
	void ProcessSyncMsg();
};



#endif
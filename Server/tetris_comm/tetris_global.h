#ifndef __TETRIS_GLOBAL_H__
#define __TETRIS_GLOBAL_H__

#include "network/packet.h"

#define _NAME_MAX_LEN 48

#define _TRITRIS_GAME_CMD	6
template<SUBCMD_TYPE subcmdvalue>
struct stTetrisGameCmd:public stCmdBase<_TRITRIS_GAME_CMD,subcmdvalue> {};

#define _SYS_LOGIN_CMD		  1
template<SUBCMD_TYPE subcmdvalue>
struct stSystemCmd:public stCmdBase<_SYS_LOGIN_CMD,subcmdvalue> {};

#define _GAME_ROOM_CMD 7
template<SUBCMD_TYPE subcmdvalue>
struct stGameRoomCmd:public stCmdBase<_GAME_ROOM_CMD,subcmdvalue> {};

enum { BoardWidth = 10, BoardHeight = 22 };

#define TR_START  0
struct stTetrisStart:public stTetrisGameCmd<TR_START>
{
	stTetrisStart() {
		ZEROCMD;
	}
};

#define TR_START_RET 1
struct stTetrisStartRet:public stTetrisGameCmd<TR_START_RET>
{
	int nErrorCode;
	stTetrisStartRet(){
		ZEROCMD;
	}
};

enum emTR_OPERATOR:int{
	emTR_OPERATOR_NULL,
	emTR_OPERATOR_DOWN,
	emTR_OPERATOR_LEFT,
	emTR_OPERATOR_RIGHT,
	emTR_OPERATOR_ROTATELEFT,
	emTR_OPERATOR_ROTATERIGHT,
	emTR_OPERATOR_PAUSE,
	emTR_OPERATOR_START,
	emTR_OPERATOR_ChangeOnline,
};

#define TR_OPERATOR 2
struct stTetrisOperator:stTetrisGameCmd<TR_OPERATOR>
{
	BYTE btOperator;
	stTetrisOperator(){
		ZEROCMD;
	}
};

struct stShape
{
	int pieceShape;
	int coords[4][2];
};

#define TR_REFRESH 3
struct stTetrisRefresh:public stTetrisGameCmd<TR_REFRESH>
{
	BYTE board[BoardWidth * BoardHeight];
	int nCurX;
	int nCurY;
	stShape curPiece;
	bool isMe;
	stTetrisRefresh(){
		ZEROCMD;
	}
};

#define TR_DRAW_TEXT 4
struct stTetrisDrawText:public stTetrisGameCmd<TR_DRAW_TEXT>
{
	char szDrawText[48];
	bool isMe;
	stTetrisDrawText(){
		ZEROCMD;
	}
};


//===========================================================================================
#define SYS_LOGIN 1
struct stUserLogin:public stSystemCmd<SYS_LOGIN>
{
	char szAccount[48];
	char szPwd[48];
	stUserLogin(){
		ZEROCMD;
	}
};
#define SYS_LOGINRET 2
struct stUserLoginRet:public stSystemCmd<SYS_LOGINRET>
{
	BYTE btError;
	stUserLoginRet(){
		ZEROCMD;
	}
};

#define SYS_REGEST 3
struct stUserRegist:public stSystemCmd<SYS_REGEST>
{
	char szAccount[48];
	char szPwd[48];
	stUserRegist(){
		ZEROCMD;
	}
};

#define SYS_REGEST_RET 4
struct stUserRegistRet:public stSystemCmd<SYS_REGEST_RET>
{
	BYTE btError;
	stUserRegistRet(){
		ZEROCMD;
	}
};

#define SYS_REGEST_TEST 5
struct stUserRegistTest:public stSystemCmd<SYS_REGEST_RET>
{
	char szAccount[48];
	stUserRegistTest(){
		ZEROCMD;
	}
};

#define SYS_REGEST_TEST_RET 6
struct stUserRegistTestRet:public stSystemCmd<SYS_REGEST_TEST_RET>
{
	BYTE btError;
	stUserRegistTestRet(){
		ZEROCMD;
	}
};


//=======================================================================================
#define GAME_ROOM_VIEW 1
struct stGameRoomView :public stGameRoomCmd<_GAME_ROOM_CMD>
{
	int nId;
	stGameRoomView() {
		ZEROCMD;
	}
};

struct stRoomInfo
{
	int nId;
	char szName1[_NAME_MAX_LEN];
	char szName2[_NAME_MAX_LEN];
	stRoomInfo() {
		ZEROSELF;
	}
};

#define GAME_ROOM_VIEW_RET 2
struct stGameRoomViewRet:public stGameRoomCmd<GAME_ROOM_VIEW_RET>
{
	int nId;
	stZeroArray<stRoomInfo> RoomInfos;
	stGameRoomViewRet() {
		ZEROCMD;
	}
};

#define GAME_ROOM_INTER 3
struct stGameRoomInter:public stGameRoomCmd<GAME_ROOM_INTER>
{
	int nErrorCode;
	int nRoomId;
	int nSeat;
};

#define GAME_ROOM_LEAVE 4
struct stGameRoomLeave:public stGameRoomCmd<GAME_ROOM_LEAVE>
{
	int nErrorCode;
};



#endif
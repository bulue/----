#ifndef SVRBOARD_H
#define SVRBOARD_H

#include "Shape.h"
#include "tetris_global.h"

class CPlayer;
class CLogicBoad
{
public:
	CLogicBoad();

	Tetrominoes & ShapeAt(int x, int y) { return board[(y * BoardWidth) + x]; }

	void Start();
	void Pause();
	void ClearBoard();
	void DropDown();
	void OneLineDown();
	void PieceDropped();
	void RemoveFullLines();
	void NewPiece();
	bool TryMove(const Shape& newPiece, int newX, int newY);
	void Refresh();
	void OnTimer();

	bool DoLogicBoardCmd(stBaseCmd* cmd,int nLen);

	__inline bool isPause() {return isPaused;}
	bool isStarted;
	bool isPaused;
	bool isFallingFinished;
	Shape curPiece;
	int curX;
	int curY;
	int numLinesRemoved;
	Tetrominoes board[BoardWidth * BoardHeight];

	void SetUser(CPlayer* _user){m_user = _user;}
	void SendToMe(stBaseCmd* pcmd,int nLen);
	CPlayer* m_user;
};

#endif
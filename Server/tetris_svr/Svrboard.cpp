#include "SvrBoard.h"
#include "TetrisSever.h"
#include "TetrisEngine.h"


void CLogicBoad::ClearBoard()
{
	for (int i = 0; i < BoardHeight * BoardWidth; ++i)
		board[i] = NoShape;
}

CLogicBoad::CLogicBoad()
{
	memset(this,0,sizeof(*this));
}

void CLogicBoad::Start()
{
	if (isPaused)
		return;

	isStarted = true;
	isFallingFinished = false;
	numLinesRemoved = 0;
	ClearBoard();

	stTetrisDrawText cmd;
	if (m_user && m_user->m_GameRoom) {
		CGameRoom* pRoom = m_user->m_GameRoom;
		for (int i =0 ;i  < 2; ++i) {
			CPlayer* pUser = pRoom->m_vPlayer[i];
			if (pUser) {
				if (pUser != m_user) {
					cmd.isMe = false;
					sprintf(cmd.szDrawText,"%s得分%d:",m_user->szAccount,numLinesRemoved);
				}else {
					cmd.isMe = true;
					sprintf(cmd.szDrawText,"您的得分:%d",numLinesRemoved);
				}
				pUser->SendToMe((stBaseCmd*)&cmd,sizeof(cmd));
			}
		}
	}

	NewPiece();
}

void CLogicBoad::Pause()
{
	if (!isStarted)
		return;

	isPaused = !isPaused;
	Refresh();
}


void CLogicBoad::NewPiece()
{
	curPiece.SetRandomShape();
	curX = BoardWidth / 2 + 1;
	curY = BoardHeight - 1 + curPiece.MinY();

	if (!TryMove(curPiece, curX, curY)) {
		curPiece.SetShape(NoShape);
		isStarted = false;
		
		stTetrisDrawText cmd;
		if (m_user && m_user->m_GameRoom) {
			CGameRoom* pRoom = m_user->m_GameRoom;
			for (int i =0 ;i  < 2; ++i) {
				CPlayer* pUser = pRoom->m_vPlayer[i];
				if (pUser) {
					if (pUser != m_user) {
						cmd.isMe = false;
						sprintf(cmd.szDrawText,"%s最终得分:%d",m_user->szAccount,numLinesRemoved);
					}else {
						cmd.isMe = true;
						sprintf(cmd.szDrawText,"游戏结束,您的得分:%d",numLinesRemoved);
					}
					pUser->SendToMe((stBaseCmd*)&cmd,sizeof(cmd));
				}
			}
		}
	}
}

bool CLogicBoad::TryMove( const Shape& newPiece, int newX, int newY )
{
	for (int i = 0; i < 4; ++i) {
		int x = newX + newPiece.x(i);
		int y = newY - newPiece.y(i);
		if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight)
			return false;
		if (ShapeAt(x, y) != NoShape)
			return false;
	}

	curPiece = newPiece;
	curX = newX;
	curY = newY;
	Refresh();
	return true;
}

void CLogicBoad::OneLineDown()
{
	if (!TryMove(curPiece, curX, curY - 1))
		PieceDropped();
}

void CLogicBoad::PieceDropped()
{
	for (int i = 0; i < 4; ++i) {
		int x = curX + curPiece.x(i);
		int y = curY - curPiece.y(i);
		ShapeAt(x, y) = curPiece.GetShape();
	}

	RemoveFullLines();

	if (!isFallingFinished)
		NewPiece();
}

void CLogicBoad::RemoveFullLines()
{
	int numFullLines = 0;

	for (int i = BoardHeight - 1; i >= 0; --i) {
		bool lineIsFull = true;

		for (int j = 0; j < BoardWidth; ++j) {
			if (ShapeAt(j, i) == NoShape) {
				lineIsFull = false;
				break;
			}
		}

		if (lineIsFull) {
			++numFullLines;
			for (int k = i; k < BoardHeight - 1; ++k) {
				for (int j = 0; j < BoardWidth; ++j)
					ShapeAt(j, k) = ShapeAt(j, k + 1);
			}
		}
	}

	if (numFullLines > 0) {
		numLinesRemoved += numFullLines;
		stTetrisDrawText cmd;
		if (m_user && m_user->m_GameRoom) {
			CGameRoom* pRoom = m_user->m_GameRoom;
			for (int i =0 ;i  < 2; ++i) {
				CPlayer* pUser = pRoom->m_vPlayer[i];
				if (pUser) {
					if (pUser != m_user) {
						cmd.isMe = false;
						sprintf(cmd.szDrawText,"%s的得分:%d",m_user->szAccount,numLinesRemoved);
					}else {
						cmd.isMe = true;
						sprintf(cmd.szDrawText,"您的得分:%d",numLinesRemoved);
					}
					pUser->SendToMe((stBaseCmd*)&cmd,sizeof(cmd));
				}
			}
		}

		isFallingFinished = true;
		curPiece.SetShape(NoShape);
		Refresh();
	}
}

void CLogicBoad::OnTimer()
{
	if (isPaused || !isStarted)
		return ;
	if (isFallingFinished) {
		isFallingFinished = false;
		NewPiece();
	} else {
		OneLineDown();
	}
}

void CLogicBoad::Refresh()
{
	stTetrisRefresh cmd;
	
	for (int i = 0 ;i < BoardWidth * BoardHeight; ++i) {
		cmd.board[i] = board[i];
	}
	memcpy(&cmd.curPiece,&curPiece,sizeof(cmd.curPiece));
	cmd.nCurX = curX;
	cmd.nCurY = curY;
	cmd.isMe = true;

	if (m_user && m_user->m_GameRoom) {
		CGameRoom* pRoom = m_user->m_GameRoom;
		for (int i =0 ;i  < 2; ++i) {
			CPlayer* pUser = pRoom->m_vPlayer[i];
			if (pUser) {
				pUser != m_user ? cmd.isMe = false : cmd.isMe = true;
				pUser->SendToMe((stBaseCmd*)&cmd,sizeof(cmd));
			}
		}
	}
}


bool CLogicBoad::DoLogicBoardCmd( stBaseCmd* pcmd,int nLen )
{
	switch (pcmd->value)
	{
	case stTetrisRefresh::_value:
		{

		}break;
	case stTetrisOperator::_value:
		{
			stTetrisOperator* pDstcmd = (stTetrisOperator*)pcmd;
			switch (pDstcmd->btOperator)
			{
			case emTR_OPERATOR_START:
				{
					Start();
				}break;
			case emTR_OPERATOR_RIGHT:
				{
					TryMove(curPiece,curX+1,curY);
				}break;
			case emTR_OPERATOR_LEFT:
				{
					TryMove(curPiece,curX-1,curY);
				}break;
			case emTR_OPERATOR_DOWN:
				{
					TryMove(curPiece,curX,curY-1);
				}break;
			case emTR_OPERATOR_ROTATELEFT:
				{
					TryMove(curPiece.RotateLeft(),curX,curY);
				}break;
			case emTR_OPERATOR_PAUSE:
				{
					Pause();
				}break;
			}
		}break;
	}
	return true;
}

void CLogicBoad::SendToMe(stBaseCmd* pcmd,int nLen )
{
}


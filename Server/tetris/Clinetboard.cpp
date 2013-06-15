#include "ClientBoard.h"
#include "tetrisFrame.h"
#include "tetris_global.h"
#include "CltTetrisEngine.h"

ClientBoard::ClientBoard(wxFrame *parent,bool _boMain)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition,
	wxDefaultSize, wxBORDER_NONE),m_LogicBoard(this)
{
	if (_boMain) {
		Connect(wxEVT_PAINT, wxPaintEventHandler(ClientBoard::OnPaint));
		Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ClientBoard::OnKeyDown));
	}
	Connect(wxEVT_BOARD_EVENT,wxCommandEventHandler(ClientBoard::OnCustomeCmd));
}

ClientBoard::~ClientBoard()
{
	Disconnect(wxEVT_PAINT, wxPaintEventHandler(ClientBoard::OnPaint));
	Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(ClientBoard::OnKeyDown));
}

#include <boost/thread.hpp>


void ClientBoard::OnPaint(wxPaintEvent& event)
{
	printf("this = %p,ClientBoard::OnPaint()\n",this);

	wxPaintDC dc(this);

	wxString sDraw = m_LogicBoard.szDrawText;
	dc.DrawText(sDraw,0,0);
	wxSize size = GetClientSize();
	int boardTop = size.GetHeight() - BoardHeight * SquareHeight();


	for (int i = 0; i < BoardHeight; ++i) {
		for (int j = 0; j < BoardWidth; ++j) {
			Tetrominoes shape = m_LogicBoard.ShapeAt(j, BoardHeight - i - 1);
			if (shape != NoShape)
				DrawSquare(dc, 0 + j * SquareWidth(),
				boardTop + i * SquareHeight(), shape);
		}
	}

	if (m_LogicBoard.curPiece.GetShape() != NoShape) {
		for (int i = 0; i < 4; ++i) {
			int x = m_LogicBoard.curX + m_LogicBoard.curPiece.x(i);
			int y = m_LogicBoard.curY - m_LogicBoard.curPiece.y(i);
			DrawSquare(dc, 0 + x * SquareWidth(),
				boardTop + (BoardHeight - y - 1) * SquareHeight(),
				m_LogicBoard.curPiece.GetShape());
		}
	}
}

void ClientBoard::DrawSquare(wxPaintDC& dc, int x, int y, Tetrominoes shape)
{
	static wxColour colors[] = { wxColour(0, 0, 0), wxColour(204, 102, 102), 
		wxColour(102, 204, 102), wxColour(102, 102, 204), 
		wxColour(204, 204, 102), wxColour(204, 102, 204), 
		wxColour(102, 204, 204), wxColour(218, 170, 0) };

	static wxColour light[] = { wxColour(0, 0, 0), wxColour(248, 159, 171),
		wxColour(121, 252, 121), wxColour(121, 121, 252), 
		wxColour(252, 252, 121), wxColour(252, 121, 252), 
		wxColour(121, 252, 252), wxColour(252, 198, 0) };

	static wxColour dark[] = { wxColour(0, 0, 0), wxColour(128, 59, 59), 
		wxColour(59, 128, 59), wxColour(59, 59, 128), 
		wxColour(128, 128, 59), wxColour(128, 59, 128), 
		wxColour(59, 128, 128), wxColour(128, 98, 0) };


	wxPen pen(light[int(shape)]);
	pen.SetCap(wxCAP_PROJECTING);
	dc.SetPen(pen);

	dc.DrawLine(x, y + SquareHeight() - 1, x, y);
	dc.DrawLine(x, y, x + SquareWidth() - 1, y);

	wxPen darkpen(dark[int(shape)]);
	darkpen.SetCap(wxCAP_PROJECTING);
	dc.SetPen(darkpen);

	dc.DrawLine(x + 1, y + SquareHeight() - 1,
		x + SquareWidth() - 1, y + SquareHeight() - 1);
	dc.DrawLine(x + SquareWidth() - 1, 
		y + SquareHeight() - 1, x + SquareWidth() - 1, y + 1);

	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(wxBrush(colors[int(shape)])); 
	dc.DrawRectangle(x + 1, y + 1, SquareWidth() - 2, 
		SquareHeight() - 2);
}

void ClientBoard::OnCustomeCmd( wxCommandEvent& event )
{
	Refresh();
}

stBaseMessage* __copy_msg(stBaseCmd* pcmd,int nLen)
{
	void* ptr = SF_MALLOC(nLen + sizeof(stBaseMessage));
	if (ptr) {
		PTR_CMD(stBaseMessage,bmsg,ptr);
		bmsg->pushbuffer((char*)pcmd,nLen);
		return (stBaseMessage*)bmsg;
	}
	return NULL;
}


void ClientBoard::OnKeyDown( wxKeyEvent& event )
{
	AILOCKT(CltTetrisEngine::getMe().m_SyncMsgList);

	stTetrisOperator cmd;

	int keycode = event.GetKeyCode();

	if (m_LogicBoard.isPaused)
		return;

	switch (keycode) {
	case WXK_LEFT:
		{
			cmd.btOperator = emTR_OPERATOR_LEFT;
		}break;
	case WXK_RIGHT:
		{
			cmd.btOperator = emTR_OPERATOR_RIGHT;
		}break;
	case WXK_DOWN:
		{
			cmd.btOperator = emTR_OPERATOR_DOWN;

		}break;
	case WXK_UP:
		{
			cmd.btOperator = emTR_OPERATOR_ROTATELEFT;
		}break;
	case WXK_SPACE:
		{
			cmd.btOperator = emTR_OPERATOR_START;
		}break;
	case 'd':
		{
			cmd.btOperator = emTR_OPERATOR_PAUSE;
		}break;
	case 'D':
		{
			cmd.btOperator = emTR_OPERATOR_PAUSE;
		}break;
	case 'O':
	case 'o':
		{
		}break;
	case 'p':
	case 'P':
		{
			cmd.btOperator = emTR_OPERATOR_PAUSE;
		}break;
	default:
		cmd.btOperator = emTR_OPERATOR_NULL;
		event.Skip();
	}

	if (cmd.btOperator !=emTR_OPERATOR_NULL)
	{
		if (CltTetrisEngine::getMe().m_boAlone){
			stBaseMessage* bmsg = __copy_msg((stBaseCmd*)&cmd,sizeof(stTetrisOperator));
			if (bmsg)
				CltTetrisEngine::getMe().m_SyncMsgList.push_back(bmsg);
		}else {
			if (CltTetrisEngine::getMe().m_ClientSession)
				CltTetrisEngine::getMe().m_ClientSession->write((stBaseCmd*)&cmd,sizeof(stTetrisOperator));
		}
	}
}



//===============================================================================================

void CLogicBoad::ClearBoard()
{
	for (int i = 0; i < BoardHeight * BoardWidth; ++i)
		board[i] = NoShape;
}

CLogicBoad::CLogicBoad(ClientBoard* _ClientBoard)
{
	memset(this,0,sizeof(*this));
	m_ClientBoard = _ClientBoard;
}

void CLogicBoad::Start()
{
	if (isPaused)
		return;

	isStarted = true;
	isFallingFinished = false;
	numLinesRemoved = 0;
	ClearBoard();

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

		isFallingFinished = true;
		curPiece.SetShape(NoShape);
		Refresh();
	}
}

void CLogicBoad::OnTimer()
{
	if (isFallingFinished) {
		isFallingFinished = false;
		NewPiece();
	} else {
		OneLineDown();
	}
}

void CLogicBoad::Refresh()
{
	wxCommandEvent event(wxEVT_BOARD_EVENT);
	event.SetEventObject(m_ClientBoard);
	m_ClientBoard->GetEventHandler()->AddPendingEvent(event);
}


bool CLogicBoad::DoLogicBoardCmd( stBaseCmd* pcmd,int nLen )
{
	switch (pcmd->value)
	{
	case stTetrisRefresh::_value:
		{
			stTetrisRefresh* pDstCmd = (stTetrisRefresh*)pcmd;
			for (int i = 0; i < BoardWidth * BoardHeight; ++i) {
				board[i] = Tetrominoes(pDstCmd->board[i]);
			}
			curX = pDstCmd->nCurX;
			curY = pDstCmd->nCurY;
			memcpy(&curPiece,&pDstCmd->curPiece,sizeof(curPiece));
			Refresh();
		}break;
	case stTetrisDrawText::_value:
		{
			stTetrisDrawText* pDstCmd = (stTetrisDrawText*)pcmd;
			strcpy_s(szDrawText,sizeof(szDrawText),pDstCmd->szDrawText);
			Refresh();
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
				}
			}
		}break;
	}
	return true;
}
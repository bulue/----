#ifndef BOARD_H
#define BOARD_H

#include <wx/wx.h>
#include "Shape.h"
#include "clientsession.h"
#include "tetris_global.h"

class ClientBoard;
class CLogicBoad
{
public:
	CLogicBoad(ClientBoard* m_ClientBoard);

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

	ClientBoard* m_ClientBoard;
	char szDrawText[1024];
};

class ClientBoard : public wxPanel
{

public:
	ClientBoard(wxFrame *parent,bool _boMain = true);
	~ClientBoard();
protected:
	void OnPaint(wxPaintEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnCustomeCmd(wxCommandEvent& event);
public:
	enum { BoardWidth = 10, BoardHeight = 22 };

	int SquareWidth() { return GetClientSize().GetWidth() / BoardWidth; }
	int SquareHeight() { return GetClientSize().GetHeight() / BoardHeight; }

	void DrawSquare(wxPaintDC &dc, int x, int y, Tetrominoes shape);

	CLogicBoad m_LogicBoard;

};


#endif
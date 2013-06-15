#ifndef __TETRIS_H__
#define __TETRIS_H__

#include <wx/wx.h>
#include "Singleton.h"
#include "ClientBoard.h"
#include "clientsession.h"
#include <boost/thread.hpp>
#include <vector>

class TetrisFrame
	: public wxFrame
{
public:
	TetrisFrame();
	~TetrisFrame();

	ClientBoard* m_ClientBoard;
	ClientBoard* m_ClientBoard1;

	virtual void OnMenuSelectBegin( wxCommandEvent& event );
	virtual void OnMenuOperatorSelectionSummary( wxCommandEvent& event );
};



#endif
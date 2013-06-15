///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GAMEHALL_H__
#define __GAMEHALL_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/frame.h>

#include <vector>

using namespace std;
///////////////////////////////////////////////////////////////////////////
#include "RoomVector.h"

typedef CRoomVector::stVisitor stRoomVistor;
///////////////////////////////////////////////////////////////////////////////
/// Class GameHall
///////////////////////////////////////////////////////////////////////////////
class GameHallFrame : public wxFrame ,public stRoomVistor
{
private:

protected:
	wxScrolledWindow* m_scrolledWindow1;
	wxBitmapButton* m_bpButton3;

	vector<wxButton*> m_vSeatBtn;

	void OnSeatBtnClick( wxCommandEvent& event );
	void OnCustomeCmd(wxCommandEvent& event);
public:
	virtual bool Show(bool show = true);
	virtual void Update(CRoomVector* ,int nId);
	virtual void Visit(CRoomVector* );
	void OnRoomEnter();

	GameHallFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ÓÎÏ·´óÌü"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 781,526 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

	~GameHallFrame();

};

#endif //__NONAME_H__

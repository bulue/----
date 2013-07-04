///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __SQLGUI_H__
#define __SQLGUI_H__
#include "DBDialog.h"
#include <wx/wx.h>

///////////////////////////////////////////////////////////////////////////
#include <sqltask.h>

///////////////////////////////////////////////////////////////////////////////
/// Class SqlGuiFrame
///////////////////////////////////////////////////////////////////////////////
class SqlGuiFrame : public DBDialog 
{
private:
	ODConnection m_odconn;
private:
	// Virtual event handlers, overide them in your derived class
	virtual void OnSizeChange( wxSizeEvent& event ) { event.Skip(); }
	virtual void OnConnectButtonClick( wxCommandEvent& event );
	virtual void OnExecuteButtonClick( wxCommandEvent& event );
	virtual void OnChoiceDbName( wxCommandEvent& event );
	virtual void OnChoiseTableName( wxCommandEvent& event );
	virtual void OnGridCellChanged( wxGridEvent& event );

	void OnCopy(wxCommandEvent &WXUNUSED(event));
	void OnPaste(wxCommandEvent &WXUNUSED(event));
	void OnRightMouseDown(wxGridEvent &event); 
public:

	SqlGuiFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 832,573 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

	~SqlGuiFrame();

};

#endif //__SQLGUI_H__

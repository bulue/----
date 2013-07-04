///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DBDIALOG_H__
#define __DBDIALOG_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/combobox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/grid.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DBDialog
///////////////////////////////////////////////////////////////////////////////
class DBDialog : public wxFrame 
{
private:

protected:
	wxPanel* m_panel;
	wxComboBox* m_comboBox1;
	wxButton* m_button2;
	wxStaticText* m_staticText3;
	wxChoice* m_ccDbName;
	wxStaticText* m_staticText4;
	wxChoice* m_ccTableName;
	wxTextCtrl* m_textCtrl1;
	wxButton* m_button1;
	wxPanel* m_panel5;
	wxGrid* m_grid2;
	wxStatusBar* m_statusBar;

	// Virtual event handlers, overide them in your derived class
	virtual void OnSizeChange( wxSizeEvent& event ) { event.Skip(); }
	virtual void OnConnectButtonClick( wxCommandEvent& event ) { event.Skip(); }
	virtual void OnChoiceDbName( wxCommandEvent& event ) { event.Skip(); }
	virtual void OnChoiseTableName( wxCommandEvent& event ) { event.Skip(); }
	virtual void OnExecuteButtonClick( wxCommandEvent& event ) { event.Skip(); }
	virtual void OnGridCellChanged( wxGridEvent& event ) { event.Skip(); }
	virtual void OnRightMouseDown( wxMouseEvent& event ) { event.Skip(); }


public:

	DBDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 832,573 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

	~DBDialog();

};

#endif //__DBDIALOG_H__

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WXTMPFRAME_H__
#define __WXTMPFRAME_H__

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
#include <wx/treectrl.h>
#include <wx/panel.h>
#include <wx/aui/auibook.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/grid.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class wxTmpFrame
///////////////////////////////////////////////////////////////////////////////
class wxTmpFrame : public wxFrame 
{
private:

protected:
	wxPanel* m_panel35;
	wxComboBox* m_comboBox1;
	wxButton* m_ConnBtn;
	wxPanel* m_panel5;
	wxPanel* m_panel36;
	wxTreeCtrl* m_DBTreeCtrl;
	wxPanel* m_panel37;
	wxAuiNotebook* m_DBTablebook;
	wxStatusBar* m_statusBar1;

	// Virtual event handlers, overide them in your derived class
	virtual void OnConnectBtnClk( wxCommandEvent& event ) { event.Skip(); }
	virtual void OnDBTreeItemActivate( wxTreeEvent& event ) { event.Skip(); }
	virtual void OnDBTreeItemExpanding( wxTreeEvent& event ) { event.Skip(); }


public:

	wxTmpFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 840,608 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

	~wxTmpFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DBTableBasePanel
///////////////////////////////////////////////////////////////////////////////
#include <wx/stc/stc.h>
class DBTableBasePanel : public wxPanel 
{
private:

public:
	wxStyledTextCtrl* m_DBStyledTextCtrl;
	wxButton* m_ExcuteBtn;
	wxGrid* m_grid;

	// Virtual event handlers, overide them in your derived class
	virtual void OnExcuteBtnClk( wxCommandEvent& event ) { event.Skip(); }


public:

	DBTableBasePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 573,508 ), long style = wxTAB_TRAVERSAL ); 
	~DBTableBasePanel();

};

#endif //__WXTMPFRAME_H__

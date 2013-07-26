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
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/aui/aui.h>
#include <wx/aui/auibar.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/aui/auibook.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/grid.h>

///////////////////////////////////////////////////////////////////////////

#define Menu_NewPage 1000
#define Menu_ConnectView 1001
#define Menu_ExcuteBar 1002
#define Menu_ConnectObj 1003
#define Menu_AddNewPage 1004

///////////////////////////////////////////////////////////////////////////////
/// Class wxTmpFrame
///////////////////////////////////////////////////////////////////////////////
class wxTmpFrame : public wxFrame 
{
private:

protected:
	wxMenuBar* m_menubar1;
	wxMenu* m_menu3;
	wxMenu* m_ViewMenu;
	wxPanel* m_panel5;
	wxAuiToolBar* m_auiToolBar3;
	wxComboBox* m_comboBox1;
	wxButton* m_ConnBtn;
	wxAuiToolBar* m_excutetoolbar;
	wxComboBox* m_DBNameCB;
	wxButton* m_ExcuteBtn;
	wxPanel* m_DBTreePane;
	wxTreeCtrl* m_DBTreeCtrl;
	wxPanel* m_panel37;
	wxAuiNotebook* m_DBTablebook;
	wxStatusBar* m_statusBar1;

	// Virtual event handlers, overide them in your derived class
	virtual void OnNewPageAddClk( wxCommandEvent& event ) { event.Skip(); }
	virtual void OnMenuConnectBarSel( wxCommandEvent& event ) { event.Skip(); }
	virtual void OnMenuExcuteBarSel( wxCommandEvent& event ) { event.Skip(); }
	virtual void OnShowDBTreePane( wxCommandEvent& event ) { event.Skip(); }
	virtual void OnConnectBtnClk( wxCommandEvent& event ) { event.Skip(); }
	virtual void OnDBComBoxChanged( wxCommandEvent& event ) { event.Skip(); }
	virtual void OnExcuteBtnClk( wxCommandEvent& event ) { event.Skip(); }
	virtual void OnDBTreeItemActivate( wxTreeEvent& event ) { event.Skip(); }
	virtual void OnDBTreeItemExpanding( wxTreeEvent& event ) { event.Skip(); }
	virtual void OnDBBookPageChanged( wxAuiNotebookEvent& event ) { event.Skip(); }


public:

	wxTmpFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 840,608 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

	~wxTmpFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class DBTableBasePanel
///////////////////////////////////////////////////////////////////////////////
class DBTableBasePanel : public wxPanel 
{
private:

protected:
	wxGrid* m_grid;

public:

	DBTableBasePanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 573,508 ), long style = wxTAB_TRAVERSAL ); 
	~DBTableBasePanel();

};

#endif //__WXTMPFRAME_H__

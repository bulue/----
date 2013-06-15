///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __REGISTDLG_H__
#define __REGISTDLG_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class RegistDialog
///////////////////////////////////////////////////////////////////////////////
class RegistDialog : public wxDialog 
{
private:

protected:
	wxPanel* m_panel2;
	wxStaticText* m_staticText2;
	wxTextCtrl* m_textCtrl2;
	wxStaticText* m_staticText3;
	wxTextCtrl* m_textCtrl3;
	wxStaticText* m_staticText4;
	wxTextCtrl* m_textCtrl4;
	wxButton* m_RegistBtn;

	// Virtual event handlers, overide them in your derived class
	virtual void OnRegistButtonClick( wxCommandEvent& event ) { event.Skip(); }


public:

	RegistDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 411,230 ), long style = wxDEFAULT_DIALOG_STYLE ); 
	~RegistDialog();

};

#endif //__NONAME_H__

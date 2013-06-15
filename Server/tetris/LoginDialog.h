///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __LOGINDIALOG_H__
#define __LOGINDIALOG_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/dialog.h>

#include "tetris_custome_event.h"


///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class LoginDialog
///////////////////////////////////////////////////////////////////////////////
class LoginDialog : public wxDialog 
{
private:

protected:
	wxPanel* m_panel1;
	wxStaticBitmap* m_bitmap2;
	wxStaticText* m_staticText1;
	wxTextCtrl* m_textCtrl2;
	wxStaticText* m_staticText2;
	wxTextCtrl* m_textCtrl3;
	wxButton* m_LoginBtn;
	wxButton* m_RegistBtn;

	void OnLoginButtonClick( wxCommandEvent& event );
	void OnRegistButtonClick( wxCommandEvent& event );

public:
	void EndModal_Thread_Safe(int retCode);

	LoginDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 297,143 ), long style = wxDEFAULT_DIALOG_STYLE ); 
	~LoginDialog();

};


#endif //__NONAME_H__

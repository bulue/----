///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "LoginDialog.h"
#include "CltTetrisEngine.h"

///////////////////////////////////////////////////////////////////////////

LoginDialog::LoginDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	SetIcon(wxICON(tretris));
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	//m_bitmap2 = new wxStaticBitmap( m_panel1, wxID_ANY, wxBITMAP(bmp), wxDefaultPosition, wxDefaultSize, 0 );
	//bSizer2->Add( m_bitmap2, 0, wxALL, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1 = new wxStaticText( m_panel1, wxID_ANY, wxT("ÕË»§"), wxDefaultPosition, wxSize( 50,-1 ), wxALIGN_CENTRE );
	m_staticText1->Wrap( -1 );
	bSizer3->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl2 = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	bSizer3->Add( m_textCtrl2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText2 = new wxStaticText( m_panel1, wxID_ANY, wxT("ÃÜÂë"), wxDefaultPosition, wxSize( 50,-1 ), wxALIGN_CENTRE );
	m_staticText2->Wrap( -1 );
	bSizer4->Add( m_staticText2, 0, wxALL, 5 );

	m_textCtrl3 = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), wxTE_PASSWORD );
	bSizer4->Add( m_textCtrl3, 0, wxALL, 5 );


	bSizer2->Add( bSizer4, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );


	bSizer5->Add( bSizer6, 1, wxEXPAND, 5 );

	m_LoginBtn = new wxButton( m_panel1, wxID_ANY, wxT("µÇÂ½"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_LoginBtn, 0, wxALL, 5 );

	m_RegistBtn = new wxButton( m_panel1, wxID_ANY, wxT("×¢²á"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_RegistBtn, 0, wxALL, 5 );


	bSizer2->Add( bSizer5, 1, wxEXPAND, 5 );


	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );


	SetTitle(wxT("¶íÂŞË¹·½¿é"));


	// Connect Events
	m_LoginBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LoginDialog::OnLoginButtonClick ), NULL, this );
	m_RegistBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LoginDialog::OnRegistButtonClick ), NULL, this );
}

LoginDialog::~LoginDialog()
{
	m_LoginBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LoginDialog::OnLoginButtonClick ), NULL, this );
	m_RegistBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( LoginDialog::OnRegistButtonClick ), NULL, this );

}

#include "tetris_global.h"
#include "tetrisFrame.h"

void LoginDialog::OnLoginButtonClick( wxCommandEvent& event )
{
	wxString strAccount = m_textCtrl2->GetValue();
	wxString strPwd = m_textCtrl3->GetValue();
	
	stUserLogin cmd;

	strcpy_s(cmd.szAccount,sizeof(cmd.szAccount),strAccount.ToStdString().c_str());
	strcpy_s(cmd.szPwd,sizeof(cmd.szPwd),strPwd.ToStdString().c_str());

	if (CltTetrisEngine::getMe().m_ClientSession)
		CltTetrisEngine::getMe().m_ClientSession->write((stBaseCmd*)&cmd,sizeof(cmd));
	else 
		wxMessageBox(wxT("Î´Á¬½ÓÉÏ·şÎñÆ÷£¬Çë¼ì²éÍøÂç"),wxT("ÍøÂç´íÎó"),wxOK|wxICON_ERROR);
	
	event.Skip();
}

#include "RegistDialog.h"

void LoginDialog::OnRegistButtonClick( wxCommandEvent& event )
{
	RegistDialog *registDlg = new RegistDialog(NULL);
	registDlg->ShowModal();
	registDlg->Destroy();
	event.Skip();
}


void LoginDialog::EndModal_Thread_Safe( int retCode )
{
	wxCommandEvent event(wxEVT_TETRIS_EVENT,GetId());
	event.SetEventObject(this);
	GetEventHandler()->AddPendingEvent(event);
}


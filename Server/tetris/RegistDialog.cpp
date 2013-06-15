///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "RegistDialog.h"

///////////////////////////////////////////////////////////////////////////

RegistDialog::RegistDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	SetIcon(wxICON(tretris));
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText2 = new wxStaticText( m_panel2, wxID_ANY, wxT("ÕËºÅ"), wxDefaultPosition, wxSize( 50,-1 ), wxALIGN_CENTRE );
	m_staticText2->Wrap( -1 );
	bSizer7->Add( m_staticText2, 0, wxALL, 5 );

	m_textCtrl2 = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	bSizer7->Add( m_textCtrl2, 0, wxALL, 5 );


	bSizer6->Add( bSizer7, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText3 = new wxStaticText( m_panel2, wxID_ANY, wxT("ÃÜÂë"), wxDefaultPosition, wxSize( 50,-1 ), wxALIGN_CENTRE );
	m_staticText3->Wrap( -1 );
	bSizer8->Add( m_staticText3, 0, wxALL, 5 );

	m_textCtrl3 = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	bSizer8->Add( m_textCtrl3, 0, wxALL, 5 );


	bSizer6->Add( bSizer8, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText4 = new wxStaticText( m_panel2, wxID_ANY, wxT("È·ÈÏÃÜÂë"), wxDefaultPosition, wxSize( 50,-1 ), wxALIGN_CENTRE );
	m_staticText4->Wrap( -1 );
	bSizer9->Add( m_staticText4, 0, wxALL, 5 );

	m_textCtrl4 = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	bSizer9->Add( m_textCtrl4, 0, wxALL, 5 );


	bSizer6->Add( bSizer9, 1, wxEXPAND, 5 );

	m_RegistBtn = new wxButton( m_panel2, wxID_ANY, wxT("×¢²á"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_RegistBtn, 0, wxALL|wxALIGN_RIGHT, 5 );


	m_panel2->SetSizer( bSizer6 );
	m_panel2->Layout();
	bSizer6->Fit( m_panel2 );
	bSizer5->Add( m_panel2, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer5 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_RegistBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( RegistDialog::OnRegistButtonClick ), NULL, this );
}

RegistDialog::~RegistDialog()
{
	// Disconnect Events
	m_RegistBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( RegistDialog::OnRegistButtonClick ), NULL, this );

}

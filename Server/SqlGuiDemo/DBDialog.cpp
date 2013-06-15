///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "DBDialog.h"

///////////////////////////////////////////////////////////////////////////

DBDialog::DBDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_comboBox1 = new wxComboBox( m_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_comboBox1->SetSelection( 0 );
	bSizer5->Add( m_comboBox1, 1, wxALL, 5 );

	m_button2 = new wxButton( m_panel, wxID_ANY, wxT("连接"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_button2, 0, wxALL, 5 );


	bSizer3->Add( bSizer5, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText3 = new wxStaticText( m_panel, wxID_ANY, wxT("数据库"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer11->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString m_ccDbNameChoices;
	m_ccDbName = new wxChoice( m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_ccDbNameChoices, 0 );
	m_ccDbName->SetSelection( 0 );
	bSizer11->Add( m_ccDbName, 1, wxALL, 5 );

	m_staticText4 = new wxStaticText( m_panel, wxID_ANY, wxT("表"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer11->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString m_ccTableNameChoices;
	m_ccTableName = new wxChoice( m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_ccTableNameChoices, 0 );
	m_ccTableName->SetSelection( 0 );
	bSizer11->Add( m_ccTableName, 1, wxALL, 5 );


	bSizer3->Add( bSizer11, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_textCtrl1 = new wxTextCtrl( m_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	bSizer4->Add( m_textCtrl1, 1, wxALL, 5 );

	m_button1 = new wxButton( m_panel, wxID_ANY, wxT("执行"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button1, 0, wxALL, 5 );


	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );


	m_panel->SetSizer( bSizer3 );
	m_panel->Layout();
	bSizer3->Fit( m_panel );
	bSizer2->Add( m_panel, 0, wxEXPAND, 5 );

	m_panel5 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxVERTICAL );

	m_grid2 = new wxGrid( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid2->CreateGrid( 0, 0 );
	m_grid2->EnableEditing( true );
	m_grid2->EnableGridLines( true );
	m_grid2->SetGridLineColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_grid2->EnableDragGridSize( true );
	m_grid2->SetMargins( 10, 10 );

	// Columns
	m_grid2->EnableDragColMove( false );
	m_grid2->EnableDragColSize( true );
	m_grid2->SetColLabelSize( 30 );
	m_grid2->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	m_grid2->AutoSizeRows();
	m_grid2->EnableDragRowSize( false );
	m_grid2->SetRowLabelSize( 80 );
	m_grid2->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance

	// Cell Defaults
	m_grid2->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer41->Add( m_grid2, 1, wxALL|wxEXPAND, 5 );


	m_panel5->SetSizer( bSizer41 );
	m_panel5->Layout();
	bSizer41->Fit( m_panel5 );
	bSizer2->Add( m_panel5, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer2 );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 4, wxST_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( DBDialog::OnSizeChange ) );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DBDialog::OnConnectButtonClick ), NULL, this );
	m_ccDbName->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DBDialog::OnChoiceDbName ), NULL, this );
	m_ccTableName->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DBDialog::OnChoiseTableName ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DBDialog::OnExecuteButtonClick ), NULL, this );
	m_grid2->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( DBDialog::OnRightMouseDown ), NULL, this );
}

DBDialog::~DBDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( DBDialog::OnSizeChange ) );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DBDialog::OnConnectButtonClick ), NULL, this );
	m_ccDbName->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DBDialog::OnChoiceDbName ), NULL, this );
	m_ccTableName->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DBDialog::OnChoiseTableName ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DBDialog::OnExecuteButtonClick ), NULL, this );
	m_grid2->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( DBDialog::OnRightMouseDown ), NULL, this );

}

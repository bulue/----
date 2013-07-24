///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxTmpFrame.h"

///////////////////////////////////////////////////////////////////////////

wxTmpFrame::wxTmpFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxVERTICAL );

	m_panel35 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );

	m_comboBox1 = new wxComboBox( m_panel35, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer6->Add( m_comboBox1, 1, wxALL, 5 );

	m_ConnBtn = new wxButton( m_panel35, wxID_ANY, wxT("Á¬½Ó"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_ConnBtn, 0, wxALL, 5 );


	bSizer39->Add( bSizer6, 0, wxEXPAND, 5 );

	m_panel5 = new wxPanel( m_panel35, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	m_panel36 = new wxPanel( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer40;
	bSizer40 = new wxBoxSizer( wxVERTICAL );

	m_DBTreeCtrl = new wxTreeCtrl( m_panel36, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer40->Add( m_DBTreeCtrl, 1, wxALL|wxEXPAND, 5 );


	m_panel36->SetSizer( bSizer40 );
	m_panel36->Layout();
	bSizer40->Fit( m_panel36 );
	bSizer8->Add( m_panel36, 1, wxEXPAND | wxALL, 5 );

	m_panel37 = new wxPanel( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxVERTICAL );

	m_DBTablebook = new wxAuiNotebook( m_panel37, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE );

	bSizer41->Add( m_DBTablebook, 1, wxEXPAND | wxALL, 5 );


	m_panel37->SetSizer( bSizer41 );
	m_panel37->Layout();
	bSizer41->Fit( m_panel37 );
	bSizer8->Add( m_panel37, 1, wxEXPAND | wxALL, 5 );


	m_panel5->SetSizer( bSizer8 );
	m_panel5->Layout();
	bSizer8->Fit( m_panel5 );
	bSizer39->Add( m_panel5, 1, wxEXPAND | wxALL, 5 );


	m_panel35->SetSizer( bSizer39 );
	m_panel35->Layout();
	bSizer39->Fit( m_panel35 );
	bSizer32->Add( m_panel35, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer32 );
	this->Layout();
	m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	m_ConnBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxTmpFrame::OnConnectBtnClk ), NULL, this );
	m_DBTreeCtrl->Connect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( wxTmpFrame::OnDBTreeItemActivate ), NULL, this );
	m_DBTreeCtrl->Connect( wxEVT_COMMAND_TREE_ITEM_EXPANDING, wxTreeEventHandler( wxTmpFrame::OnDBTreeItemExpanding ), NULL, this );
}

wxTmpFrame::~wxTmpFrame()
{
	// Disconnect Events
	m_ConnBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxTmpFrame::OnConnectBtnClk ), NULL, this );
	m_DBTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( wxTmpFrame::OnDBTreeItemActivate ), NULL, this );
	m_DBTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_ITEM_EXPANDING, wxTreeEventHandler( wxTmpFrame::OnDBTreeItemExpanding ), NULL, this );

}

DBTableBasePanel::DBTableBasePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	m_DBStyledTextCtrl = new wxStyledTextCtrl( this, wxID_ANY,  wxDefaultPosition);
	bSizer8->Add( m_DBStyledTextCtrl, 1, wxALL, 5 );

	m_ExcuteBtn = new wxButton( this, wxID_ANY, wxT("Ö´ÐÐ"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_ExcuteBtn, 0, wxALL, 5 );


	bSizer7->Add( bSizer8, 0, wxEXPAND, 5 );

	m_grid = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid->CreateGrid( 0, 0 );
	m_grid->EnableEditing( true );
	m_grid->EnableGridLines( true );
	m_grid->EnableDragGridSize( false );
	m_grid->SetMargins( 0, 0 );

	// Columns
	m_grid->EnableDragColMove( false );
	m_grid->EnableDragColSize( true );
	m_grid->SetColLabelSize( 30 );
	m_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	m_grid->EnableDragRowSize( true );
	m_grid->SetRowLabelSize( 80 );
	m_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance

	// Cell Defaults
	m_grid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer7->Add( m_grid, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer7 );
	this->Layout();

	// Connect Events
	m_ExcuteBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DBTableBasePanel::OnExcuteBtnClk ), NULL, this );
}

DBTableBasePanel::~DBTableBasePanel()
{
	// Disconnect Events
	m_ExcuteBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DBTableBasePanel::OnExcuteBtnClk ), NULL, this );

}

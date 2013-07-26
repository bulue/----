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
	this->SetSizeHints( wxSize( 300,400 ), wxDefaultSize );

	m_menubar1 = new wxMenuBar( 0 );
	m_menu3 = new wxMenu();
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_menu3, Menu_NewPage, wxString( wxT("新建查询") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem4 );

	m_menubar1->Append( m_menu3, wxT("文件") ); 

	m_ViewMenu = new wxMenu();
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_ViewMenu, Menu_ConnectView, wxString( wxT("查询工具条") ) , wxEmptyString, wxITEM_NORMAL );
	m_ViewMenu->Append( m_menuItem3 );

	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( m_ViewMenu, Menu_ExcuteBar, wxString( wxT("执行工具条") ) , wxEmptyString, wxITEM_NORMAL );
	m_ViewMenu->Append( m_menuItem6 );

	m_ViewMenu->AppendSeparator();

	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_ViewMenu, Menu_ConnectObj, wxString( wxT("连接对象管理器") ) , wxEmptyString, wxITEM_NORMAL );
	m_ViewMenu->Append( m_menuItem5 );

	m_menubar1->Append( m_ViewMenu, wxT("视图") ); 

	this->SetMenuBar( m_menubar1 );

	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxVERTICAL );

	m_panel5 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	m_auiToolBar3 = new wxAuiToolBar( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT|wxAUI_TB_OVERFLOW ); 
	m_comboBox1 = new wxComboBox( m_auiToolBar3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_comboBox1->SetMinSize( wxSize( 500,-1 ) );

	m_auiToolBar3->AddControl( m_comboBox1 );
	m_ConnBtn = new wxButton( m_auiToolBar3, wxID_ANY, wxT("连接"), wxDefaultPosition, wxDefaultSize, 0 );
	m_auiToolBar3->AddControl( m_ConnBtn );
	m_auiToolBar3->AddSeparator(); 

	m_auiToolBar3->AddTool( wxID_ANY, wxT("tool"), wxArtProvider::GetBitmap( wxART_CDROM, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 

	m_auiToolBar3->AddTool( wxID_ANY, wxT("tool"), wxArtProvider::GetBitmap( wxART_COPY, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxT("拷贝"), wxEmptyString, NULL ); 

	m_auiToolBar3->AddTool( wxID_ANY, wxT("tool"), wxArtProvider::GetBitmap( wxART_PASTE, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxT("黏贴"), wxEmptyString, NULL ); 

	m_auiToolBar3->AddTool( wxID_ANY, wxT("tool"), wxArtProvider::GetBitmap( wxART_TIP, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxT("提示消息"), wxEmptyString, NULL ); 

	m_auiToolBar3->Realize(); 

	bSizer8->Add( m_auiToolBar3, 0, wxALL, 5 );

	m_excutetoolbar = new wxAuiToolBar( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_HORZ_LAYOUT|wxAUI_TB_OVERFLOW ); 
	m_DBNameCB = new wxComboBox( m_excutetoolbar, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_DBNameCB->SetMinSize( wxSize( 200,-1 ) );

	m_excutetoolbar->AddControl( m_DBNameCB );
	m_ExcuteBtn = new wxButton( m_excutetoolbar, wxID_ANY, wxT("执行"), wxDefaultPosition, wxDefaultSize, 0 );
	m_excutetoolbar->AddControl( m_ExcuteBtn );
	m_excutetoolbar->AddSeparator(); 

	m_excutetoolbar->AddTool( Menu_AddNewPage, wxT("tool"), wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_TOOLBAR ), wxNullBitmap, wxITEM_NORMAL, wxT("新建查询"), wxEmptyString, NULL ); 

	m_excutetoolbar->Realize(); 

	bSizer8->Add( m_excutetoolbar, 0, wxALL, 5 );

	m_DBTreePane = new wxPanel( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer40;
	bSizer40 = new wxBoxSizer( wxVERTICAL );

	m_DBTreeCtrl = new wxTreeCtrl( m_DBTreePane, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer40->Add( m_DBTreeCtrl, 1, wxALL|wxEXPAND, 5 );


	m_DBTreePane->SetSizer( bSizer40 );
	m_DBTreePane->Layout();
	bSizer40->Fit( m_DBTreePane );
	bSizer8->Add( m_DBTreePane, 1, wxEXPAND | wxALL, 5 );

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
	bSizer32->Add( m_panel5, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer32 );
	this->Layout();
	m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( m_menuItem4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxTmpFrame::OnNewPageAddClk ) );
	this->Connect( m_menuItem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxTmpFrame::OnMenuConnectBarSel ) );
	this->Connect( m_menuItem6->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxTmpFrame::OnMenuExcuteBarSel ) );
	this->Connect( m_menuItem5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxTmpFrame::OnShowDBTreePane ) );
	m_ConnBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxTmpFrame::OnConnectBtnClk ), NULL, this );
	m_DBNameCB->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( wxTmpFrame::OnDBComBoxChanged ), NULL, this );
	m_ExcuteBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxTmpFrame::OnExcuteBtnClk ), NULL, this );
	this->Connect( Menu_AddNewPage, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxTmpFrame::OnNewPageAddClk ) );
	m_DBTreeCtrl->Connect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( wxTmpFrame::OnDBTreeItemActivate ), NULL, this );
	m_DBTreeCtrl->Connect( wxEVT_COMMAND_TREE_ITEM_EXPANDING, wxTreeEventHandler( wxTmpFrame::OnDBTreeItemExpanding ), NULL, this );
	m_DBTablebook->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( wxTmpFrame::OnDBBookPageChanged ), NULL, this );
}

wxTmpFrame::~wxTmpFrame()
{
	// Disconnect Events
	this->Disconnect( Menu_NewPage, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxTmpFrame::OnNewPageAddClk ) );
	this->Disconnect( Menu_ConnectView, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxTmpFrame::OnMenuConnectBarSel ) );
	this->Disconnect( Menu_ExcuteBar, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxTmpFrame::OnMenuExcuteBarSel ) );
	this->Disconnect( Menu_ConnectObj, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxTmpFrame::OnShowDBTreePane ) );
	m_ConnBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxTmpFrame::OnConnectBtnClk ), NULL, this );
	m_DBNameCB->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( wxTmpFrame::OnDBComBoxChanged ), NULL, this );
	m_ExcuteBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxTmpFrame::OnExcuteBtnClk ), NULL, this );
	this->Disconnect( Menu_AddNewPage, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( wxTmpFrame::OnNewPageAddClk ) );
	m_DBTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( wxTmpFrame::OnDBTreeItemActivate ), NULL, this );
	m_DBTreeCtrl->Disconnect( wxEVT_COMMAND_TREE_ITEM_EXPANDING, wxTreeEventHandler( wxTmpFrame::OnDBTreeItemExpanding ), NULL, this );
	m_DBTablebook->Disconnect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( wxTmpFrame::OnDBBookPageChanged ), NULL, this );

}

DBTableBasePanel::DBTableBasePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );


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
}

DBTableBasePanel::~DBTableBasePanel()
{
}

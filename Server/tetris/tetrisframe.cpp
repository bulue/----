#include "tetrisFrame.h"
#include "ClientBoard.h"
#include "CltTetrisEngine.h"

#define _MENU_BEGIN 1
#define _MENU_OPERATOR_SUMMARY  2

TetrisFrame::TetrisFrame()
	: wxFrame(NULL, wxID_ANY, wxT("Tetris"), wxDefaultPosition, wxSize(490, 625))
{
	wxMenuBar* m_menubar1;
	wxMenu* m_menu1;
	wxMenu* m_menu2;

	SetIcon(wxICON(tretris));
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_ClientBoard = new ClientBoard(this);
	bSizer2->Add( m_ClientBoard, 1, wxEXPAND | wxALL, 5 );


	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_ClientBoard1 = new ClientBoard(this);
	bSizer3->Add( m_ClientBoard1, 1, wxEXPAND | wxALL, 5 );


	bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );

	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( wxT("开始") ) , wxT("开始游戏"), wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );

	m_menubar1->Append( m_menu1, wxT("菜单") ); 

	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_menu2, wxID_ANY, wxString( wxT("操作") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu2->Append( m_menuItem3 );

	m_menubar1->Append( m_menu2, wxT("帮助") ); 

	this->SetMenuBar( m_menubar1 );

	this->SetSizer( bSizer1 );
	this->Layout();
	wxStatusBar * m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( m_menuItem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TetrisFrame::OnMenuSelectBegin ) );
	this->Connect( m_menuItem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TetrisFrame::OnMenuOperatorSelectionSummary ) );
}


TetrisFrame::~TetrisFrame()
{
	CltTetrisEngine::getMe().m_TetrisFrame = NULL;
}

void TetrisFrame::OnMenuSelectBegin( wxCommandEvent& WXUNUSED(event) )
{
	stTetrisOperator cmd;
	cmd.btOperator = emTR_OPERATOR_START;
	if (CltTetrisEngine::getMe().m_ClientSession)
		CltTetrisEngine::getMe().m_ClientSession->write((stBaseCmd*)&cmd,sizeof(cmd));
}

void TetrisFrame::OnMenuOperatorSelectionSummary( wxCommandEvent& WXUNUSED(event) )
{
	//stTetrisOperator cmd;
	//cmd.btOperator = emTR_OPERATOR_START;
	//if (CltTetrisEngine::getMe().m_ClientSession)
		//CltTetrisEngine::getMe().m_ClientSession->write((stBaseCmd*)&cmd,sizeof(cmd));
}



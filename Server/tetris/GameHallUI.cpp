///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GameHallUI.h"
#include "CltTetrisEngine.h"
#include "tetris_custome_event.h"

///////////////////////////////////////////////////////////////////////////


int __make_btn_id(int _nRoomId,int _nSeat) {
	return _nRoomId*10 + _nSeat;
}

int __get_btn_room_id(int _nBitId) {
	return _nBitId/10;
}

int __get_btn_seat_id(int _nBitId) {
	return _nBitId%10;
}

GameHallFrame::GameHallFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	SetIcon(wxICON(tretris));
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_scrolledWindow1 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );

	bSizer5->Add( m_scrolledWindow1, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( bSizer5 );
	this->Layout();

	this->Centre( wxBOTH );

	Connect(wxEVT_GAMEHALL_EVENT,wxCommandEventHandler( GameHallFrame::OnCustomeCmd ), NULL, this);
}

GameHallFrame::~GameHallFrame()
{
	printf("GameHallFrame Îö¹¹ÁË\n");
}

void GameHallFrame::OnSeatBtnClick( wxCommandEvent& event )
{
	stGameRoomInter cmd;
	int nBtnId = event.GetId(); 
	cmd.nRoomId = __get_btn_room_id(nBtnId);
	cmd.nSeat = __get_btn_seat_id(nBtnId);

	if (CltTetrisEngine::getMe().m_ClientSession) 
		CltTetrisEngine::getMe().m_ClientSession->write((stBaseCmd*)&cmd,sizeof(stGameRoomInter));
	event.Skip();
}

void GameHallFrame::Update( CRoomVector* pRoomVector,int nId )
{

}

void GameHallFrame::Visit( CRoomVector* pRoomVector)
{
	wxCommandEvent event(wxEVT_GAMEHALL_EVENT);
	event.SetEventObject(this);
	event.SetClientData(pRoomVector);
	event.SetId(1);
	this->GetEventHandler()->AddPendingEvent(event);
}

void GameHallFrame::OnRoomEnter()
{
	wxCommandEvent event(wxEVT_GAMEHALL_EVENT);
	event.SetEventObject(this);
	event.SetId(2);
	this->GetEventHandler()->AddPendingEvent(event);
}

bool GameHallFrame::Show( bool show /*= true*/ )
{
	if (wxFrame::Show(show) && show) {
		stGameRoomView cmd;
		if (CltTetrisEngine::getMe().m_ClientSession) {
			CltTetrisEngine::getMe().m_ClientSession->write((stBaseCmd*)&cmd,sizeof(cmd));
		}
		return true;
	}
	return false;
}

void GameHallFrame::OnCustomeCmd( wxCommandEvent& event )
{
	switch(event.GetId())
	{
	case 1:
		{
			CRoomVector* pRoomVector = (CRoomVector*)event.GetClientData();
			CRoomVector& vRoom = *pRoomVector;
			AILOCKP(pRoomVector);

			wxGridSizer* gSizer2;
			gSizer2 = new wxGridSizer( 5, 5, 30, 30 );

			for (int i  = 0; i < vRoom.size(); ++i)
			{
				int nRoomId = vRoom[i].nId;
				wxString sName1 = vRoom[i].szName1;
				wxString sName2 = vRoom[i].szName2;

				wxBoxSizer* bSizer11;
				bSizer11 = new wxBoxSizer( wxHORIZONTAL );


				wxButton *btn1 = new wxButton( m_scrolledWindow1, __make_btn_id(nRoomId,0), sName1, wxDefaultPosition, wxDefaultSize, 0 );
				btn1->SetMinSize( wxSize( 80,80 ) );
				bSizer11->Add( btn1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
				m_vSeatBtn.push_back(btn1);

				m_bpButton3 = new wxBitmapButton( m_scrolledWindow1, wxID_ANY, wxBitmap(wxBITMAP(tretris_bmp)), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
				bSizer11->Add( m_bpButton3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

				wxButton *btn2 = new wxButton( m_scrolledWindow1, __make_btn_id(nRoomId,1), sName2, wxDefaultPosition, wxSize( 80,80 ), 0 );
				bSizer11->Add( btn2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
				m_vSeatBtn.push_back(btn2);


				gSizer2->Add( bSizer11, 1, wxEXPAND, 5 );
			}


			m_scrolledWindow1->SetSizer( gSizer2 );
			m_scrolledWindow1->Layout();
			gSizer2->Fit( m_scrolledWindow1 );

			for (int i = 0;i < m_vSeatBtn.size(); ++i) {
				m_vSeatBtn[i]->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GameHallFrame::OnSeatBtnClick ), NULL, this );
			}

			Maximize();
		}break;
	case 2:
		{
			CltTetrisEngine::getMe().m_TetrisFrame = new TetrisFrame();
			CltTetrisEngine::getMe().m_boAlone = false;
			CltTetrisEngine::getMe().m_TetrisFrame->Show();
			this->Close(true);
		}break;
	}
	
}



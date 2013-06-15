#include <wx/wx.h>

class MyApp : public wxApp
{
public:
	virtual bool OnInit();

};

#include "CltTetrisEngine.h"
#include <boost/thread.hpp>

IMPLEMENT_APP(MyApp)

	bool MyApp::OnInit()
{
#ifdef _DEBUG1
	if (!AllocConsole()){
		wxMessageBox("Failed to create the console!");
	}else {
		AttachConsole(ATTACH_PARENT_PROCESS);
		freopen("CONIN$", "r+t", stdin); // 重定向 STDIN
		freopen("CONOUT$", "w+t", stdout); // 重定向STDOUT
		wxPuts(wxT("A wxWidgets console application"));
	}
#endif
	//srand(time(NULL));

	CClientSession* clientsession = new CClientSession();
	if(clientsession){
		clientsession->async_connect("127.0.0.1",6112);
	}
	CltTetrisEngine::getMe().m_WorkThreads.push_back(boost::thread(boost::bind(&io_service::run,&CltTetrisEngine::getMe().io_s)));
	CltTetrisEngine::getMe().m_WorkThreads.push_back(boost::thread(boost::bind(&CltTetrisEngine::Run,&CltTetrisEngine::getMe())));

	CltTetrisEngine::getMe().m_LoginDialog = new LoginDialog(NULL);
	if (CltTetrisEngine::getMe().m_LoginDialog)
	{
		int ret = CltTetrisEngine::getMe().m_LoginDialog->ShowModal();
		if (ret == 0) {
			CltTetrisEngine::getMe().m_GameHallFrame = new GameHallFrame(NULL);
			CltTetrisEngine::getMe().m_GameHallFrame->Show();
		}
		CltTetrisEngine::getMe().m_LoginDialog->Destroy();
	}

	return true;
}
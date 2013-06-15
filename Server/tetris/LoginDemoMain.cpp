#include "LoginDialog.h"
#include <wx/wx.h>

class MyApp:public wxApp
{
public:
	virtual bool OnInit();
};


IMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	wxInitAllImageHandlers();
	LoginDialog* frame = new LoginDialog(NULL);
	frame->ShowModal();
	frame->Destroy();
#ifdef _DEBUG1
	if (!AllocConsole())
		wxMessageBox("Failed to create the console!");
	AttachConsole(ATTACH_PARENT_PROCESS);
	freopen("CONIN$", "r+t", stdin); // 重定向 STDIN
	freopen("CONOUT$", "w+t", stdout); // 重定向STDOUT
	wxPuts(wxT("A wxWidgets console application"));
#endif
	return true;
}
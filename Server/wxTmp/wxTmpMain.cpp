#include "SqlToolFrame.h"
#include <wx/wx.h>

class MyApp:public wxApp
{
public:
	virtual bool OnInit();
};


IMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
#ifdef _DEBUG1
	if (!AllocConsole())
		wxMessageBox("Failed to create the console!");
	AttachConsole(ATTACH_PARENT_PROCESS);
	freopen("CONIN$", "r+t", stdin); // 重定向 STDIN
	freopen("CONOUT$", "w+t", stdout); // 重定向STDOUT
	wxPuts(wxT("A wxWidgets console application"));
	wxPuts(wxT("这几个字是测试中文的\n"));
#endif
	SqlToolFrame* frame = new SqlToolFrame(NULL);
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}

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
	freopen("CONIN$", "r+t", stdin); // �ض��� STDIN
	freopen("CONOUT$", "w+t", stdout); // �ض���STDOUT
	wxPuts(wxT("A wxWidgets console application"));
	wxPuts(wxT("�⼸�����ǲ������ĵ�\n"));
#endif
	SqlToolFrame* frame = new SqlToolFrame(NULL);
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}

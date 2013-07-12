#ifndef __SQLTOOLFRAME_H__ 
#define __SQLTOOLFRAME_H__


#include "wxTmpFrame.h"
#include <sqltask.h>
#include <wx/stc/stc.h>

class SqlToolFrame:public wxTmpFrame
{
public:
	SqlToolFrame(wxWindow* parent);

	virtual void OnConnectBtnClk( wxCommandEvent& event );
	virtual void OnDBTreeItemActivate( wxTreeEvent& event );
	virtual void OnDBTreeItemExpanding( wxTreeEvent& event );
};

#define _DBDATA_DB_		1
#define _DBDATA_TABLE_  2

class DBTreeData:public wxTreeItemData
{
public:
	DBTreeData(int _nType) {nType = _nType;}
	int nType;
};

enum
{
	MARGIN_LINE_NUMBERS
};

class DBTablePanel:public DBTableBasePanel
{
public:
	wxString m_DBName;
	virtual void OnExcuteBtnClk( wxCommandEvent& event );

	DBTablePanel( wxWindow* parent, 
		wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL );

};




#endif	//__SQLTOOLFRAME_H__


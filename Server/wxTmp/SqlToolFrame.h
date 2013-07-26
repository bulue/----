#ifndef __SQLTOOLFRAME_H__ 
#define __SQLTOOLFRAME_H__

class SqlToolFrame;

#include "wxTmpFrame.h"
#include <sqltask.h>
#include <wx/stc/stc.h>

class SqlToolFrame:public wxTmpFrame
{
public:
	SqlToolFrame(wxWindow* parent);
	~SqlToolFrame(){m_mgr.UnInit();}

	virtual void OnConnectBtnClk( wxCommandEvent& event );
	virtual void OnDBTreeItemActivate( wxTreeEvent& event );
	virtual void OnDBTreeItemExpanding( wxTreeEvent& event );
	virtual void OnExcuteBtnClk( wxCommandEvent& event );
	virtual void OnMenuViewConnectPane( wxCommandEvent& event );
	virtual void OnNewPageAddClk( wxCommandEvent& event );
	virtual void OnShowDBTreePane( wxCommandEvent& event );
	virtual void OnDBBookPageChanged( wxAuiNotebookEvent& event );
	virtual void OnDBComBoxChanged( wxCommandEvent& event );
	virtual void OnMenuExcuteBarSel( wxCommandEvent& event );
	virtual void OnMenuConnectBarSel( wxCommandEvent& event );
private:
	wxAuiManager m_mgr;
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
	wxStyledTextCtrl* m_DBStyledTextCtrl;
	vector<wxString> m_AutoCompTips;	//×Ô¶¯²¹Æë´úÂë
	virtual void OnHotspotClk(wxStyledTextEvent& event);
	virtual void OnCallTipClk(wxStyledTextEvent& event);
	virtual void OnAutoCompSelection(wxStyledTextEvent& event);
	virtual void OnCharAdded(wxStyledTextEvent& event);

	void OnExcuteBtnClk();

	DBTablePanel( wxWindow* parent, 
		wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL );
	wxAuiManager m_mgr;

	~DBTablePanel() {
		m_mgr.UnInit();
	}
};




#endif	//__SQLTOOLFRAME_H__


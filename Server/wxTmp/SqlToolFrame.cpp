#include "SqlToolFrame.h"
#include "ToolEngine.h"
#include <wx/imaglist.h>
#include <wx/textfile.h>
#include <wx/msgdlg.h>
#include <wx/app.h>

std::vector<wxString> __get_lines_from_file(const wxString& _strFileName)
{
	std::vector<wxString> vLines;
	wxTextFile tFile;
	if (tFile.Open(_strFileName)) {
		wxString tmp1 = tFile.GetFirstLine();
		vLines.push_back(tmp1);
		while (!tFile.Eof()) {
			wxString tmp2 = tFile.GetNextLine();
			vLines.push_back(tmp2);
		}
		tFile.Close();
	}
	return vLines;
}

SqlToolFrame::SqlToolFrame( wxWindow* parent ):wxTmpFrame(parent)
{
	SetIcon(wxIcon(wxICON(sqlicon)));
	SetTitle(wxT("数据库工具-2"));
	wxImageList* pImageList = new wxImageList(20,20);
	//pImageList->Add(wxBITMAP(folder_bmp1));
	//pImageList->Add(wxBITMAP(folder_bmp2));
	//pImageList->Add(wxBITMAP(table_bmp));
	pImageList->Add(wxArtProvider::GetBitmap( wxART_FOLDER, wxART_OTHER ));
	pImageList->Add(wxArtProvider::GetBitmap( wxART_FOLDER_OPEN, wxART_OTHER ));
	pImageList->Add(wxBITMAP(table_bmp));
	pImageList->Add(wxArtProvider::GetBitmap( wxART_GO_HOME, wxART_OTHER ));

	wxTreeItemId treeroot1 = m_DBTreeCtrl->AddRoot(wxT("DataManager"),3);
	m_DBTreeCtrl->AssignImageList(pImageList);

	std::vector<wxString> vLine = __get_lines_from_file(wxT("配置文件.ini"));
	if (!vLine.empty()) {
		for (int i = 0; i < (int)vLine.size(); ++i) {
			m_comboBox1->Append(vLine[i]);
		}
		m_comboBox1->SetValue(vLine[0]);
	}

	//wxAuiSimpleTabArt * pTabArt = new wxAuiSimpleTabArt();
	//if (pTabArt) {
	//	pTabArt->SetActiveColour(wxColour(RGB(0,0,255)));
	//}
	//m_DBTablebook->SetArtProvider(pTabArt);

	m_mgr.SetManagedWindow(m_panel5);
	m_mgr.AddPane(m_DBTreePane, wxAuiPaneInfo().
		Name(wxT("tb4")).Caption(wxT("对象资源管理器")).Left());
	m_mgr.AddPane(m_panel37,wxAuiPaneInfo().CenterPane());
	m_mgr.AddPane(m_auiToolBar3,wxAuiPaneInfo().
		Name(wxT("m_auiToolBar3")).ToolbarPane().Top().
		LeftDockable(false).
		RightDockable(false).
		BottomDockable(false));
	m_mgr.AddPane(m_excutetoolbar,wxAuiPaneInfo().
		Name(wxT("m_excutetoolbar")).ToolbarPane().Top().
		LeftDockable(false).
		RightDockable(false).
		BottomDockable(false));
	m_mgr.Update();
}

void SqlToolFrame::OnConnectBtnClk( wxCommandEvent& event )
{
	ToolEngine::getMe().m_DBConn.disconnect();

	wxString wsConn = m_comboBox1->GetValue();
	if (ToolEngine::getMe().m_DBConn.connect(wsConn)) {
		_vstring vDbName = ToolEngine::getMe().m_DBConn.GetDataBaseName();
		wxTreeItemId root = m_DBTreeCtrl->GetRootItem();
		if (root.IsOk()) {
			m_DBTreeCtrl->DeleteChildren(root);
		}
		for (int i = 0; i < (int)vDbName.size(); ++i) {
			m_DBTreeCtrl->AppendItem(root,vDbName[i],0,1,new DBTreeData(_DBDATA_DB_));
			m_DBNameCB->Append(vDbName[i]);
		}
		m_DBTreeCtrl->ExpandAllChildren(root);
		if (!vDbName.empty()) m_DBNameCB->SetValue(vDbName[0]);
	}
	event.Skip();
}

void SqlToolFrame::OnDBTreeItemActivate( wxTreeEvent& event )
{
	wxTreeItemId itemid = event.GetItem();
	wxString wsItemName =  m_DBTreeCtrl->GetItemText(itemid);
	DBTreeData* pDbData = (DBTreeData*)m_DBTreeCtrl->GetItemData(itemid);
	if (!pDbData) return;
	if (pDbData->nType == _DBDATA_DB_) {
		_vstring vTbName = ToolEngine::getMe().m_DBConn.GetDataTableName(wsItemName.t_str());

		m_DBTreeCtrl->DeleteChildren(itemid);
		for (int i = 0;i < (int)vTbName.size(); ++i) {
			m_DBTreeCtrl->AppendItem(itemid,vTbName[i],2,2,new DBTreeData(_DBDATA_TABLE_));
		}
	}

	if (pDbData->nType == _DBDATA_TABLE_) {
		DBTablePanel* newPanel = new DBTablePanel(m_DBTablebook,wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

		wxTreeItemId parent =  m_DBTreeCtrl->GetItemParent(itemid);
		wxString wsDbName = m_DBTreeCtrl->GetItemText(parent);
		ODStatement stmt(ToolEngine::getMe().m_DBConn.get_native_dbc_handle());
		if (!stmt.Execute(wxString::Format("USE %s",wsDbName.t_str()))) {
			wxMessageBox(wxString::Format("LastState:%s\nLastError:%s",stmt.LastStateError().c_str(),stmt.LastError().c_str()),wxT("ODBC ERROR"),wxOK|wxICON_ERROR);
		}
		_vstring vColName = ToolEngine::getMe().m_DBConn.GetColumnName(wsItemName.t_str());
		//newPanel->m_DBName = wxString::Format("USE %s\n",wsDbName.t_str());
		newPanel->m_DBName = wsDbName;
		m_DBNameCB->SetValue(wsDbName);
		for (size_t i = 0; i < vColName.size(); ++i) {
			newPanel->m_AutoCompTips.push_back(vColName[i]);
		}

		wxString wsQuery;
		wsQuery += wxT("/****** Script for SelectTopNRows command  ******/ \n");
		wsQuery += (wxT("SELECT TOP 1000 "));
		for (int i = 0; i < (int)vColName.size() ;++i ) {
			wsQuery+=(wxString::Format(wxT("[%s]"),vColName[i].c_str()));
			if (i+1 < (int)vColName.size())
				wsQuery+=(wxT(","));
		}
		wsQuery += (wxString::Format(" FROM [%s]",wsItemName.t_str()));

		newPanel->m_DBStyledTextCtrl->SetText(wsQuery);

		m_DBTablebook->AddPage( newPanel, wsItemName, false, wxNullBitmap );
		m_DBTablebook->SetSelection(m_DBTablebook->GetPageCount()-1);
		newPanel->Layout();
	}

	event.Skip();
}

void SqlToolFrame::OnDBTreeItemExpanding( wxTreeEvent& event )
{
	event.Skip();
}

void SqlToolFrame::OnExcuteBtnClk( wxCommandEvent& event )
{
	DBTablePanel* curPanel = (DBTablePanel*)m_DBTablebook->GetCurrentPage();
	if (curPanel) {
		curPanel->OnExcuteBtnClk();
	}
	event.Skip();
}

void SqlToolFrame::OnMenuViewConnectPane( wxCommandEvent& event )
{
	m_mgr.GetPane(m_auiToolBar3).Show(true);
	m_mgr.Update();
	event.Skip();
}

void SqlToolFrame::OnNewPageAddClk( wxCommandEvent& event )
{
	static int index = 1;
	DBTablePanel* newPanel = new DBTablePanel(m_DBTablebook,wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_DBTablebook->AddPage(newPanel,wxString::Format("新查询%d",index++));
	event.Skip();
}

void SqlToolFrame::OnShowDBTreePane( wxCommandEvent& event )
{
	m_mgr.GetPane(m_DBTreePane).Show(true);
	m_mgr.Update();
	event.Skip();
}

void SqlToolFrame::OnDBBookPageChanged( wxAuiNotebookEvent& event )
{
	DBTablePanel* pane = (DBTablePanel*)m_DBTablebook->GetCurrentPage();
	if (pane) {
		m_DBNameCB->SetValue(pane->m_DBName);	
	}else
		m_DBNameCB->SetValue(wxT(""));
	event.Skip();
}

void SqlToolFrame::OnDBComBoxChanged( wxCommandEvent& event )
{
	DBTablePanel* pane = (DBTablePanel*)m_DBTablebook->GetCurrentPage();
	if (pane) {
		pane->m_DBName = m_DBNameCB->GetValue();
	}
	event.Skip();
}

void SqlToolFrame::OnMenuExcuteBarSel( wxCommandEvent& event )
{
	m_mgr.GetPane(m_excutetoolbar).Show(true);
	m_mgr.Update();
	event.Skip();
}

void SqlToolFrame::OnMenuConnectBarSel( wxCommandEvent& event )
{
	m_mgr.GetPane(m_auiToolBar3).Show(true);
	m_mgr.Update();
	event.Skip();
}


//=============================DBTablePanel==================================

void DBTablePanel::OnExcuteBtnClk()
{
	wxAuiPaneInfo& paneinfo = m_mgr.GetPane(m_grid);
	if (paneinfo.IsOk()&&!paneinfo.IsShown()) {
		paneinfo.Show(true);
		m_mgr.Update();
	}
	int nGridCols = m_grid->GetNumberCols();
	int nGridRows = m_grid->GetNumberRows();
	if (nGridCols) m_grid->DeleteCols(0,nGridCols);
	if (nGridRows) m_grid->DeleteRows(0,nGridRows);

	if (ToolEngine::getMe().m_DBConn.isopen()) {
		ODStatement stmt(ToolEngine::getMe().m_DBConn.get_native_dbc_handle());

		wxString ws = m_DBStyledTextCtrl->GetStringSelection();
		if (ws.empty()) ws = m_DBStyledTextCtrl->GetValue();
		wxString statement =m_DBName.empty()? wxT(""):wxString::Format("USE %s",m_DBName);
		
		if ( (statement.empty() || stmt.Execute(statement)) && stmt.Execute(ws)) {
			ODResultSet set = stmt.GetResultSet();
			int nSetCols = set.CountCols();
			m_grid->InsertCols(0,nSetCols);
			for (int i = 1; i <= nSetCols; ++i) {
				od_desccol col = set.GetDscCol(i);
				m_grid->SetColLabelValue(i-1,col.Name());
			}
			int row = 1;
			m_grid->BeginBatch();
			while (set.FetchNext()) {
				m_grid->InsertRows(m_grid->GetNumberRows());
				for (int col = 1; col <= nSetCols; ++col) {
					od_fieldimpl field = set.GetField(col);
					_tstring strTmp = field.as_string();
					if (strTmp == wxT("")) strTmp = wxT("NULL");
					m_grid->SetCellValue(strTmp,row-1,col-1);
				}
				row++;
			}
			m_grid->EndBatch();
			DWORD dwAutoSizeTime = GetTickCount();
			m_grid->AutoSizeColumns();
		}else {
			wxMessageBox(wxString::Format("LastState:%s\nLastError:%s",stmt.LastStateError().c_str(),stmt.LastError().c_str()),wxT("ODBC ERROR"),wxOK|wxICON_ERROR);
		}
	}
}

DBTablePanel::DBTablePanel( wxWindow* parent, wxWindowID id /*= wxID_ANY*/, const wxPoint& pos /*= wxDefaultPosition*/, const wxSize& size /*= wxDefaultSize*/, long style /*= wxTAB_TRAVERSAL */ ) :DBTableBasePanel(parent,id,pos,size,style)
{
	//m_DBStyledTextCtrl->SetMarginWidth (MARGIN_LINE_NUMBERS, 10);
	//m_DBStyledTextCtrl->StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColour (75, 75, 75) );
	//m_DBStyledTextCtrl->StyleSetBackground (wxSTC_STYLE_LINENUMBER, wxColour (220, 220, 220));
	//m_DBStyledTextCtrl->SetMarginType (MARGIN_LINE_NUMBERS, wxSTC_MARGIN_RTEXT);
	m_DBStyledTextCtrl = new wxStyledTextCtrl( this, wxID_ANY,  wxDefaultPosition);
	m_DBStyledTextCtrl->SetWrapMode (wxSTC_WRAP_WORD);

	m_DBStyledTextCtrl->SetLexer(wxSTC_LEX_SQL);

	m_DBStyledTextCtrl->SetKeyWords(0,wxT("add except percent all exec plan alter execute precision and exists primary any exit print as fetch proc asc file procedure authorization fillfactor public backup for raiserror begin foreign read between freetext readtext break freetexttable reconfigure browse from references bulk full replication by function restore cascade goto restrict case grant return check group revoke checkpoint having right close holdlock rollback clustered identity rowcount coalesce identity_insert rowguidcol collate identitycol rule column if save commit in schema compute index select constraint inner session_user contains insert set containstable intersect setuser continue into shutdown convert is some create join statistics cross key system_user current kill table current_date left textsize current_time like then current_timestamp lineno to current_user load top cursor national tran database nocheck transaction dbcc nonclustered trigger deallocate not truncate declare null tsequal default nullif union delete of unique deny off update desc offsets updatetext disk on use distinct open user distributed opendatasource values double openquery varying drop openrowset view dummy openxml waitfor dump option when else or where end order while errlvl outer with escape over writetext "));
	m_DBStyledTextCtrl->StyleClearAll();

	
	//wxFont font(12,wxSTC_STYLE_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false);
	//m_DBStyledTextCtrl->SetFont(font);
	for (int i = wxSTC_SQL_DEFAULT; i < wxSTC_SQL_USER4; i++)
		m_DBStyledTextCtrl->StyleSetSize(i,10);
	// taken from sql.properties in scite
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_DEFAULT, _T("fore:#007f7f")); //中文字符，日文字符
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_COMMENT, _T("fore:#007f00")); // --注释符号后的内容
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_COMMENTLINE, _T("fore:#007f00")); //  /**/释符号中间的内容
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_COMMENTDOC, _T("fore:#007f00"));// /********/符号中间的内容
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_NUMBER, _T("fore:#007f7f"));		//数字
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_WORD, _T("fore:#00007F,bold")); //保留关键字
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_STRING, _T("fore:#7f007f")); // 双引号中的内容
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_CHARACTER, _T("fore:#7f007f")); // 单引号内的内容
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_SQLPLUS, _T("fore:#7F7F00")); // colour.preproc
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_SQLPLUS_PROMPT, _T("fore:#007F00,back:#E0FFE0,eolfilled")); // + font.monospace
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_OPERATOR, _T("fore:#808080,bold"));
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_IDENTIFIER, _T("fore:#007f7f"));//英文字符
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_COMMENTLINEDOC, _T("fore:#007f00")); // + font.comment
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_WORD2, _T("fore:#b00040"));
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_COMMENTDOCKEYWORD, _T("fore:#3060a0")); // + font.code.comment.doc
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_COMMENTDOCKEYWORDERROR, _T("fore:#804020")); // + font.code.comment.doc
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_USER1, _T("fore:#4b0082"));
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_USER2, _T("fore:#b00040"));
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_USER3, _T("fore:#8b0000"));
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_USER4, _T("fore:#800080"));
	//m_DBStyledTextCtrl->SetMouseDownCaptures(true);
	//m_DBStyledTextCtrl->Connect( wxEVT_STC_HOTSPOT_CLICK, wxStyledTextEventHandler( DBTablePanel::OnHotspotClk ), NULL, this );
	//m_DBStyledTextCtrl->Connect( wxEVT_STC_INDICATOR_CLICK, wxStyledTextEventHandler( DBTablePanel::OnHotspotClk ), NULL, this );
	//m_DBStyledTextCtrl->Connect( wxEVT_STC_MARGINCLICK, wxStyledTextEventHandler( DBTablePanel::OnHotspotClk ), NULL, this );
	//m_DBStyledTextCtrl->Connect( wxEVT_STC_DOUBLECLICK, wxStyledTextEventHandler( DBTablePanel::OnHotspotClk ), NULL, this );
	//m_DBStyledTextCtrl->Connect( wxEVT_STC_CHANGE, wxStyledTextEventHandler( DBTablePanel::OnHotspotClk ), NULL, this );
	m_DBStyledTextCtrl->Connect( wxEVT_STC_CALLTIP_CLICK, wxStyledTextEventHandler( DBTablePanel::OnCallTipClk ), NULL, this );
	m_DBStyledTextCtrl->Connect( wxEVT_STC_AUTOCOMP_SELECTION, wxStyledTextEventHandler( DBTablePanel::OnAutoCompSelection ), NULL, this );
	m_DBStyledTextCtrl->Connect( wxEVT_STC_CHARADDED, wxStyledTextEventHandler( DBTablePanel::OnCharAdded ), NULL, this );

	//
	const TCHAR* tips[] = {wxT("add"),wxT("except"),wxT("percent"),wxT("all"),wxT("exec"),wxT("plan"),wxT("alter"),wxT("execute"),wxT("precision"),wxT("and"),wxT("exists"),wxT("primary"),wxT("any"),wxT("exit"),wxT("print"),wxT("as"),wxT("fetch"),wxT("proc"),wxT("asc"),wxT("file"),wxT("procedure"),wxT("authorization"),wxT("fillfactor"),wxT("public"),wxT("backup"),wxT("for"),wxT("raiserror"),wxT("begin"),wxT("foreign"),wxT("read"),wxT("between"),wxT("freetext"),wxT("readtext"),wxT("break"),wxT("freetexttable"),wxT("reconfigure"),wxT("browse"),wxT("from"),wxT("references"),wxT("bulk"),wxT("full"),wxT("replication"),wxT("by"),wxT("function"),wxT("restore"),wxT("cascade"),wxT("goto"),wxT("restrict"),wxT("case"),wxT("grant"),wxT("return"),wxT("check"),wxT("group"),wxT("revoke"),wxT("checkpoint"),wxT("having"),wxT("right"),wxT("close"),wxT("holdlock"),wxT("rollback"),wxT("clustered"),wxT("identity"),wxT("rowcount"),wxT("coalesce"),wxT("identity"),wxT("insert"),wxT("rowguidcol"),wxT("collate"),wxT("identitycol"),wxT("rule"),wxT("column"),wxT("if"),wxT("save"),wxT("commit"),wxT("in"),wxT("schema"),wxT("compute"),wxT("index"),wxT("select"),wxT("constraint"),wxT("inner"),wxT("session"),wxT("user"),wxT("contains"),wxT("insert"),wxT("set"),wxT("containstable"),wxT("intersect"),wxT("setuser"),wxT("continue"),wxT("into"),wxT("shutdown"),wxT("convert"),wxT("is"),wxT("some"),wxT("create"),wxT("join"),wxT("statistics"),wxT("cross"),wxT("key"),wxT("system"),wxT("user"),wxT("current"),wxT("kill"),wxT("table"),wxT("current"),wxT("date"),wxT("left"),wxT("textsize"),wxT("current"),wxT("time"),wxT("like"),wxT("then"),wxT("current"),wxT("timestamp"),wxT("lineno"),wxT("to"),wxT("current"),wxT("user"),wxT("load"),wxT("top"),wxT("cursor"),wxT("national"),wxT("tran"),wxT("database"),wxT("nocheck"),wxT("transaction"),wxT("dbcc"),wxT("nonclustered"),wxT("trigger"),wxT("deallocate"),wxT("not"),wxT("truncate"),wxT("declare"),wxT("null"),wxT("tsequal"),wxT("default"),wxT("nullif"),wxT("union"),wxT("delete"),wxT("of"),wxT("unique"),wxT("deny"),wxT("off"),wxT("update"),wxT("desc"),wxT("offsets"),wxT("updatetext"),wxT("disk"),wxT("on"),wxT("use"),wxT("distinct"),wxT("open"),wxT("user"),wxT("distributed"),wxT("opendatasource"),wxT("values"),wxT("double"),wxT("openquery"),wxT("varying"),wxT("drop"),wxT("openrowset"),wxT("view"),wxT("dummy"),wxT("openxml"),wxT("waitfor"),wxT("dump"),wxT("option"),wxT("when"),wxT("else"),wxT("or"),wxT("where"),wxT("end"),wxT("order"),wxT("while"),wxT("errlvl"),wxT("outer"),wxT("with"),wxT("escape"),wxT("over"),wxT("writetext")};
	m_AutoCompTips.clear();
	for (int i = 0; i < sizeof(tips)/sizeof(tips[0]); ++i) {
		m_AutoCompTips.push_back(tips[i]);
	}

	m_mgr.SetManagedWindow(this);
	m_mgr.AddPane(m_DBStyledTextCtrl,wxAuiPaneInfo().
		Name(wxT("m_DBStyledTextCtrl")).Center().Floatable(false).CloseButton(false));
	m_mgr.AddPane(m_grid,wxAuiPaneInfo().
		Name(wxT("m_grid")).Center());
	m_mgr.Update();
}

void DBTablePanel::OnHotspotClk( wxStyledTextEvent& event )
{
	//m_DBStyledTextCtrl->CallTipShow(m_DBStyledTextCtrl->GetCurrentPos(),wxString::Format("提示：当前位置%d,当前事件编号%d,事件字符串%s",m_DBStyledTextCtrl->GetCurrentPos(),event.GetWParam(),event.GetText()));
	wxString wsComp = wxT("add except percent all exec plan alter execute precision and exists primary any exit print as fetch proc asc file procedure authorization fillfactor public backup for raiserror begin foreign read between freetext readtext break freetexttable reconfigure browse from references bulk full replication by function restore cascade goto restrict case grant return check group revoke checkpoint having right close holdlock rollback clustered identity rowcount coalesce identity_insert rowguidcol collate identitycol rule column if save commit in schema compute index select constraint inner session_user contains insert set containstable intersect setuser continue into shutdown convert is some create join statistics cross key system_user current kill table current_date left textsize current_time like then current_timestamp lineno to current_user load top cursor national tran database nocheck transaction dbcc nonclustered trigger deallocate not truncate declare null tsequal default nullif union delete of unique deny off update desc offsets updatetext disk on use distinct open user distributed opendatasource values double openquery varying drop openrowset view dummy openxml waitfor dump option when else or where end order while errlvl outer with escape over writetext ");
	m_DBStyledTextCtrl->AutoCompShow(0,wsComp);
	event.Skip();
}

void DBTablePanel::OnCallTipClk( wxStyledTextEvent& event )
{
	wxString tmp = event.GetText();
	event.Skip();
}

void DBTablePanel::OnAutoCompSelection( wxStyledTextEvent& event )
{
	wxString tmp = event.GetText();
	m_DBStyledTextCtrl->AddText(tmp);
	event.Skip();
}

void DBTablePanel::OnCharAdded( wxStyledTextEvent& event )
{
	int nCurPos = m_DBStyledTextCtrl->GetCurrentPos();
	int nStart  = m_DBStyledTextCtrl->WordStartPosition(nCurPos-1,true);
	int nEnd  = m_DBStyledTextCtrl->WordEndPosition(nCurPos-1,true);
	wxString wsWord = m_DBStyledTextCtrl->GetTextRange(nStart,nEnd);

	if (!wsWord.empty()) {
		wxString wsAutoComp;
		for (int i = 0; i < (int)m_AutoCompTips.size(); ++i) {
			wxString& tip = m_AutoCompTips[i];
			if (tip.compare(0,wsWord.size(),wsWord) == 0){
				wsAutoComp += tip;
				wsAutoComp += wxT(" ");
			}
		}
		if (!wsAutoComp.empty()) {
			m_DBStyledTextCtrl->AutoCompShow(nEnd - nStart,wsAutoComp);
		}
	}
	event.Skip();
}

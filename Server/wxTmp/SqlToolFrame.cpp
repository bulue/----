#include "SqlToolFrame.h"
#include "ToolEngine.h"
#include <wx/imaglist.h>
#include <wx/textfile.h>
#include <wx/msgdlg.h>

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
	pImageList->Add(wxBITMAP(folder_bmp1));
	pImageList->Add(wxBITMAP(folder_bmp2));
	pImageList->Add(wxBITMAP(table_bmp));

	wxTreeItemId treeroot1 = m_DBTreeCtrl->AddRoot(wxT(""));
	m_DBTreeCtrl->AssignImageList(pImageList);

	std::vector<wxString> vLine = __get_lines_from_file(wxT("配置文件.ini"));
	if (!vLine.empty()) {
		for (int i = 0; i < (int)vLine.size(); ++i) {
			m_comboBox1->Append(vLine[i]);
		}
		m_comboBox1->SetValue(vLine[0]);
	}
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
		}
		m_DBTreeCtrl->ExpandAllChildren(root);
	}
	event.Skip();
}

void SqlToolFrame::OnDBTreeItemActivate( wxTreeEvent& event )
{
	wxTreeItemId itemid = event.GetItem();
	wxString wsItemName =  m_DBTreeCtrl->GetItemText(itemid);
	DBTreeData* pDbData = (DBTreeData*)m_DBTreeCtrl->GetItemData(itemid);
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
		newPanel->m_DBName = wxString::Format("USE %s\n",wsDbName.t_str());

		wxString wsQuery;
		wxString wsColKeyWord;

		wsQuery.Append(wxT("SELECT TOP 1000 "));
		for (int i = 0; i < (int)vColName.size() ;++i ) {
			wsQuery.Append(wxString::Format(wxT("[%s]"),vColName[i].c_str()));
			wsColKeyWord.Append(wxString::Format(wxT("%s "),vColName[i].c_str()));
			if (i+1 < (int)vColName.size())
				wsQuery.Append(wxT(","));
		}
		wsQuery.Append(wxString::Format(" FROM %s",wsItemName.t_str()));

		//newPanel->m_DBStyledTextCtrl->SetKeyWords(wxSTC_SQL_WORD,wsColKeyWord);
		newPanel->m_DBStyledTextCtrl->SetText(wsQuery);

		m_DBTablebook->AddPage( newPanel, wsItemName, true, wxNullBitmap );
		newPanel->Layout();
	}

	event.Skip();
}

void SqlToolFrame::OnDBTreeItemExpanding( wxTreeEvent& event )
{
	event.Skip();
}

//=============================DBTablePanel==================================

void DBTablePanel::OnExcuteBtnClk( wxCommandEvent& event )
{
	int nGridCols = m_grid->GetNumberCols();
	int nGridRows = m_grid->GetNumberRows();
	if (nGridCols) m_grid->DeleteCols(0,nGridCols);
	if (nGridRows) m_grid->DeleteRows(0,nGridRows);

	if (ToolEngine::getMe().m_DBConn.isopen()) {
		ODStatement stmt(ToolEngine::getMe().m_DBConn.get_native_dbc_handle());

		wxString ws = m_DBStyledTextCtrl->GetStringSelection();
		if (ws.empty()) ws = m_DBStyledTextCtrl->GetValue();
		if (stmt.Execute(m_DBName) && stmt.Execute(ws)) {

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
	event.Skip();
}

DBTablePanel::DBTablePanel( wxWindow* parent, wxWindowID id /*= wxID_ANY*/, const wxPoint& pos /*= wxDefaultPosition*/, const wxSize& size /*= wxDefaultSize*/, long style /*= wxTAB_TRAVERSAL */ ) :DBTableBasePanel(parent,id,pos,size,style)
{
	m_DBStyledTextCtrl->SetMarginWidth (MARGIN_LINE_NUMBERS, 50);
	m_DBStyledTextCtrl->StyleSetForeground (wxSTC_STYLE_LINENUMBER, wxColour (75, 75, 75) );
	m_DBStyledTextCtrl->StyleSetBackground (wxSTC_STYLE_LINENUMBER, wxColour (220, 220, 220));
	m_DBStyledTextCtrl->SetMarginType (MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

	m_DBStyledTextCtrl->SetWrapMode (wxSTC_WRAP_WORD);

	m_DBStyledTextCtrl->SetLexer(wxSTC_LEX_SQL);

	m_DBStyledTextCtrl->SetKeyWords(0,wxT("add except percent all exec plan alter execute precision and exists primary any exit print as fetch proc asc file procedure authorization fillfactor public backup for raiserror begin foreign read between freetext readtext break freetexttable reconfigure browse from references bulk full replication by function restore cascade goto restrict case grant return check group revoke checkpoint having right close holdlock rollback clustered identity rowcount coalesce identity_insert rowguidcol collate identitycol rule column if save commit in schema compute index select constraint inner session_user contains insert set containstable intersect setuser continue into shutdown convert is some create join statistics cross key system_user current kill table current_date left textsize current_time like then current_timestamp lineno to current_user load top cursor national tran database nocheck transaction dbcc nonclustered trigger deallocate not truncate declare null tsequal default nullif union delete of unique deny off update desc offsets updatetext disk on use distinct open user distributed opendatasource values double openquery varying drop openrowset view dummy openxml waitfor dump option when else or where end order while errlvl outer with escape over writetext "));
	m_DBStyledTextCtrl->StyleClearAll();


	// taken from sql.properties in scite
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_DEFAULT, _T("fore:#808080")); //中文字符，日文字符
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_COMMENT, _T("fore:#007f00")); // --注释符号后的内容
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_COMMENTLINE, _T("fore:#007f00")); //  /**/释符号中间的内容
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_COMMENTDOC, _T("fore:#7f7f7f"));
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_NUMBER, _T("fore:#007f7f"));		//数字
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_WORD, _T("fore:#00007F,bold")); //保留关键字
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_STRING, _T("fore:#7f007f")); // 双引号中的内容
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_CHARACTER, _T("fore:#7f007f")); // 单引号内的内容
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_SQLPLUS, _T("fore:#7F7F00")); // colour.preproc
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_SQLPLUS_PROMPT, _T("fore:#007F00,back:#E0FFE0,eolfilled")); // + font.monospace
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_OPERATOR, _T("bold"));
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_IDENTIFIER, _T(""));//英文字符
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_COMMENTLINEDOC, _T("fore:#007f00")); // + font.comment
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_WORD2, _T("fore:#b00040"));
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_COMMENTDOCKEYWORD, _T("fore:#3060a0")); // + font.code.comment.doc
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_COMMENTDOCKEYWORDERROR, _T("fore:#804020")); // + font.code.comment.doc
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_USER1, _T("fore:#4b0082"));
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_USER2, _T("fore:#b00040"));
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_USER3, _T("fore:#8b0000"));
	m_DBStyledTextCtrl->StyleSetSpec(wxSTC_SQL_USER4, _T("fore:#800080"));

	wxFont font(15,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false);
	for (int i = wxSTC_SQL_DEFAULT; i < wxSTC_SQL_USER4; i++)
		m_DBStyledTextCtrl->StyleSetFont(i,font);
}

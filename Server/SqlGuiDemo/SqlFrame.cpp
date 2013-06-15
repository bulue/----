///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////


#include "SqlFrame.h"
#include <wx/textfile.h>

///////////////////////////////////////////////////////////////////////////

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

SqlGuiFrame::SqlGuiFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : DBDialog( parent, id, title, pos, size, style )
{

	std::vector<wxString> vLine = __get_lines_from_file(wxT("配置文件.ini"));
	if (!vLine.empty()) {
		for (int i = 0; i < vLine.size(); ++i) {
			m_comboBox1->Append(vLine[i]);
		}
		m_comboBox1->SetValue(vLine[0]);
	}
}

SqlGuiFrame::~SqlGuiFrame()
{

}

#include <wx/msgdlg.h>

void SqlGuiFrame::OnConnectButtonClick( wxCommandEvent& event )
{
	m_odconn.disconnect();
	wxString wsString = m_comboBox1->GetValue();
	if (m_odconn.connect(wsString)) {
		_vstring vDbName = m_odconn.GetDataBaseName();
		m_ccDbName->Clear();
		for (int i = 0; i < vDbName.size(); ++i) {
			m_ccDbName->Append(vDbName[i]);
		}
		m_ccDbName->SetSelection(0);
		OnChoiceDbName(event);
	}else{
		wxMessageBox(wxString::Format("LastState:%s  \nLastError:%s",
			m_odconn.LastStateError().c_str(),m_odconn.LastError().c_str()),
			wxT("ODBC CONNECT ERROR"),
			wxOK | wxICON_ERROR);
	}

	event.Skip();
}

void SqlGuiFrame::OnChoiceDbName( wxCommandEvent& event )
{
	m_ccTableName->Clear();
	int nSel = m_ccDbName->GetSelection();
	if (nSel != wxNOT_FOUND) {
		_vstring vTbName = m_odconn.GetDataTableName(m_ccDbName->GetString(nSel).t_str());
		for (int i = 0; i < vTbName.size(); ++i) {
			m_ccTableName->Append(vTbName[i]);
		}
		m_ccTableName->SetSelection(0);
		ODStatement stmt(m_odconn.get_native_dbc_handle());
		stmt.Execute(wxString::Format("USE %s",m_ccDbName->GetString(nSel).t_str()));
		OnChoiseTableName(event);
	}
	event.Skip();
}

void SqlGuiFrame::OnChoiseTableName( wxCommandEvent& event )
{
	int nSel = m_ccTableName->GetSelection();
	if (nSel != wxNOT_FOUND) {
		wxString wsTbName =  m_ccTableName->GetString(nSel);
		_vstring vColName = m_odconn.GetColumnName(wsTbName.t_str());
		wxString wsQuery(wxT("SELECT TOP 1000 "));
		for (int i = 0; i < vColName.size() ;++i ) {
			wsQuery.Append(wxString::Format(wxT("[%s]"),vColName[i].c_str()));
			if (i+1 < vColName.size())
				wsQuery.Append(wxT(","));
		}
		wsQuery.Append(wxString::Format(" FROM %s",wsTbName.t_str()));
		m_textCtrl1->Clear();
		m_textCtrl1->SetValue(wsQuery);
	}
	event.Skip();
}

#include <list>
using namespace std;

void SqlGuiFrame::OnExecuteButtonClick( wxCommandEvent& event )
{
	int nGridCols = m_grid2->GetNumberCols();
	int nGridRows = m_grid2->GetNumberRows();
	if (nGridCols) m_grid2->DeleteCols(0,nGridCols);
	if (nGridRows) m_grid2->DeleteRows(0,nGridRows);

	DWORD dwBeginTicks = GetTickCount();
	if (m_odconn.isopen()) {
		ODStatement stmt(m_odconn.get_native_dbc_handle());
		wxString wsQuery = m_textCtrl1->GetValue();
		DWORD dwBegExcTime = GetTickCount();
		bool bExe = stmt.Execute(wsQuery);
		DWORD dwEndExcTime = GetTickCount();
		m_statusBar->SetStatusText(wxString::Format("ODBC计算时间%lf秒",float(dwEndExcTime - dwBegExcTime)/1000.0f),1);
		if (bExe) {
			ODResultSet set = stmt.GetResultSet();
			int nSetCols = set.CountCols();
			m_grid2->InsertCols(0,nSetCols);
			for (int i = 1; i <= nSetCols; ++i) {
				od_desccol col = set.GetDscCol(i);
				m_grid2->SetColLabelValue(i-1,col.Name());
			}
			int row = 1;
			m_grid2->BeginBatch();
			while (set.FetchNext()) {
				m_grid2->InsertRows(m_grid2->GetNumberRows());
				for (int col = 1; col <= nSetCols; ++col) {
					od_fieldimpl field = set.GetField(col);
					_tstring strTmp = field.as_string();
					if (strTmp == wxT("")) strTmp = wxT("NULL");
					m_grid2->SetCellValue(strTmp,row-1,col-1);
				}
				row++;
			}
			m_grid2->EndBatch();
			DWORD dwAutoSizeTime = GetTickCount();
			m_grid2->AutoSizeColumns();
			m_statusBar->SetStatusText(wxString::Format("AutoSizeColumns计算时间%lf秒",float(GetTickCount() - dwAutoSizeTime)/1000.0f),3);
		}else {
			wxMessageBox(wxString::Format("LastState:%s\nLastError:%s",stmt.LastStateError().c_str(),stmt.LastError().c_str()),
				wxT("ODBC ERROR"),wxOK|wxICON_ERROR);
		}
		
	}
	DWORD dwEndTicks = GetTickCount();
	m_statusBar->SetStatusText(wxString::Format("本次执行总时间时间%lf秒",float(dwEndTicks - dwBeginTicks)/1000.0f),0);
	event.Skip();
}

#include <wx/clipbrd.h>
void SqlGuiFrame::OnCopy(wxCommandEvent &event)
{
	wxString copy_data;
	bool something_in_this_line;

	copy_data.Clear();

	for (int i=0; i<m_grid2->GetRows();i++)
	{     
		something_in_this_line = false;    
		for (int j=0; j< m_grid2->GetCols(); j++)
		{  
			if (m_grid2->IsInSelection(i,j))
			{ 
				if (something_in_this_line == false)
				{  
					if (copy_data.IsEmpty() == false) 
					{   
						copy_data.Append(wxT("\n"));  
					}
					something_in_this_line = true;
				} 
				else
				{                                
					copy_data.Append(wxT("\t"));  
				}
				copy_data = copy_data + m_grid2->GetCellValue(i,j);    
			}
		}
	}

	if (wxTheClipboard->Open()) {
		wxTheClipboard->Clear();
		wxTheClipboard->SetData(new wxTextDataObject(copy_data));
		wxTheClipboard->Close();
	}

}

void SqlGuiFrame::OnPaste(wxCommandEvent &event)
{
	wxString copy_data;
	wxString cur_field;
	wxString cur_line;

	if (wxTheClipboard->Open()) {
		if (wxTheClipboard->IsSupported(wxDF_TEXT)) {
			wxTextDataObject wxdata;
			wxTheClipboard->GetData(wxdata);
			copy_data = wxdata.GetText();
		}
		wxTheClipboard->Close();
	}

	int i = m_grid2->GetGridCursorRow();
	int j = m_grid2->GetGridCursorCol();
	int k = j;

	while(!copy_data.IsEmpty()) 
	{
		cur_line = copy_data.BeforeFirst(wxT('\n'));
		while(!cur_line.IsEmpty())
		{
			cur_field = cur_line.BeforeFirst(wxT('\t'));
			m_grid2->SetCellValue(i,j,cur_field);
			j++; 
			cur_line  = cur_line.AfterFirst (wxT('\t'));
		} 
		i++;
		j=k;
		copy_data = copy_data.AfterFirst(wxT('\n'));
	} 

}

void SqlGuiFrame::OnRightMouseDown( wxGridEvent &event )
{
	wxMenu *pmenuPopUp = new wxMenu;
	wxMenuItem* pItem;
	pItem = new wxMenuItem(pmenuPopUp,wxID_COPY, wxT("Copy"));
	pmenuPopUp->Append(pItem);

	pItem = new wxMenuItem(pmenuPopUp,wxID_PASTE, wxT("Paste"));
	pmenuPopUp->Append(pItem);
	if(!m_grid2->CanEnableCellControl()) 
		pItem->Enable(false);   

	pmenuPopUp->Connect(wxID_COPY,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(SqlGuiFrame::OnCopy),NULL,this);
	pmenuPopUp->Connect(wxID_PASTE,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(SqlGuiFrame::OnPaste),NULL,this);
	PopupMenu(pmenuPopUp);
	delete pmenuPopUp;
}

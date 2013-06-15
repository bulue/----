#include "..\sqltask.h"



int __getcountcols(SQLHSTMT _hstmt)
{
	int nColCount = 0;
	SQLNumResultCols(_hstmt,(SQLSMALLINT*)&nColCount);
	return nColCount;
}

od_desccol __getdscribecolumn(SQLHSTMT _hstmt,int _ncolnum)
{
	return od_desccol(_hstmt,_ncolnum);
}

int ODStatement::Select(const TCHAR* szQuery,const col_define* define[],TCHAR* bindata)
{
	SQLRETURN retcode;
	int nTotalRows = 0;
	retcode = SQLExecDirect(m_hstmt,(SQLCHAR*)szQuery,SQL_NTS);
	if (ODBC_SUCCESS_CODE(retcode)) {
		retcode = SQLFetch(m_hstmt);
		while (ODBC_SUCCESS_CODE(retcode)) {
			int nColCount = __getcountcols(m_hstmt);
			for (int nColNum = 1; nColNum <= nColCount; ++nColNum) {
				TCHAR szColName[256];
				od_desccol dc = od_desccol(m_hstmt,nColNum);
				if (dc.Name(szColName,sizeof(szColName))) {
					for (int i = 0; define[i] != NULL ; i++) {
						if (strcmp(szColName,define[i]->szName) == 0) {
							SQLINTEGER cbtran = 0;
							retcode = SQLGetData(m_hstmt,
								(SQLSMALLINT)nColNum,
								define[i]->ctype,
								bindata+define[i]->offset,
								define[i]->bufferlenth,&cbtran);

							if (ODBC_SUCCESS_CODE(retcode) == false){
								return ODBC_ERROR;
							}
						}
					}
				}
			}
			bindata = bindata+define[0]->structsize;
			retcode = SQLFetch(m_hstmt);
			nTotalRows ++;
		}
	}else return ODBC_ERROR;
	return nTotalRows;
}

int ODStatement::Insert(const TCHAR* sztable,const col_define* define[],const TCHAR* bindata) {
	char szInsertCmd[STATEMENT_MAX_LEN];
	int icolsize = 0;
	sprintf_s(szInsertCmd,sizeof(szInsertCmd),"INSERT INTO %s(",sztable);
	for (int i = 0; define[i]!= NULL; ++i) {
		strcat_s(szInsertCmd,define[i]->szName);
		if (define[i+1]) {
			strcat_s(szInsertCmd,",");
		}
		icolsize = i+1;
	}
	strcat_s(szInsertCmd,") VALUES (");
	for (int i =0; i < icolsize; ++i) {
		strcat_s(szInsertCmd,"?");
		if (i + 1 < icolsize) {
			strcat_s(szInsertCmd,",");
		}
	}
	strcat_s(szInsertCmd,")");

	SQLRETURN retcode;
	int nCountRows;
	retcode = SQLPrepare(m_hstmt,(SQLCHAR*)szInsertCmd,SQL_NTS);
	if (ODBC_SUCCESS_CODE(retcode)){
		SQLINTEGER cb[256];
		for (SQLUSMALLINT i = 0; i < icolsize; ++i) {
			SQLSMALLINT sqltype =0;
			cb[i] = define[i]->bufferlenth;
			retcode = SQLBindParameter(m_hstmt,
				(SQLSMALLINT)i + 1,
				SQL_PARAM_INPUT, 
				define[i]->ctype,
				define[i]->ctype,
				define[i]->bufferlenth,
				0,
				(SQLPOINTER)(bindata+define[i]->offset),
				define[i]->bufferlenth, 
				&cb[i]);
		}
		retcode = SQLExecute(m_hstmt);
		if (ODBC_SUCCESS_CODE(retcode)){
			SQLRowCount(m_hstmt,(SQLINTEGER*)&nCountRows);
			return nCountRows;
		}
	}
	return ODBC_ERROR;
}


int ODStatement::Update(const TCHAR* tablename,const col_define* define[],TCHAR* pInBinData,const TCHAR* where)
{
	char szUpdateCmd[STATEMENT_MAX_LEN];
	sprintf_s(szUpdateCmd,sizeof(szUpdateCmd),"update %s set ",tablename);

	for (int i=0;define[i] != NULL; ++i) {
		strcat_s(szUpdateCmd,define[i]->szName);
		strcat_s(szUpdateCmd,"=?");
		if (define[i+1] != NULL)
			strcat_s(szUpdateCmd,",");
	}
	strcat_s(szUpdateCmd," ");
	strcat_s(szUpdateCmd,where);

	SQLRETURN retcode;
	int rowcount;
	retcode = SQLPrepare(m_hstmt,(SQLCHAR*)szUpdateCmd,SQL_NTS);
	SQLINTEGER cb[256];
	for (int i =0;define[i] != NULL;++i){
		cb[i] = define[i]->bufferlenth;
		retcode = SQLBindParameter(m_hstmt,(SQLSMALLINT)i + 1,SQL_PARAM_INPUT, define[i]->ctype,define[i]->ctype,define[i]->bufferlenth,0,(SQLPOINTER)(pInBinData+define[i]->offset),define[i]->bufferlenth, &cb[i]);
	}
	retcode = SQLExecute(m_hstmt);
	if (ODBC_SUCCESS_CODE(retcode)){
		SQLRowCount(m_hstmt,(SQLINTEGER*)&rowcount);
		return rowcount;
	}
	return ODBC_ERROR;
}
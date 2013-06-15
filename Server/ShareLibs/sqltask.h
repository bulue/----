#ifndef __SQLTASK_H__
#define __SQLTASK_H__

#include <Windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "stringext.h"

using namespace std;


struct col_define {
	char szName[48];			//列名
	unsigned short offset;		//偏移量
	unsigned short bufferlenth;	//成员变量长度
	unsigned short ctype;		//C类型
	unsigned short structsize;	//结构长度

	col_define(char* _szName,
			unsigned short _offset,
			unsigned short _bufferlenth,
			unsigned short _ctype,
			unsigned short _structsize)
	{
		strcpy_s(szName,sizeof(szName),_szName);
		offset = _offset;
		bufferlenth = _bufferlenth;
		ctype = _ctype;
		structsize = _structsize;
	}
};

#define MakeCol(colname,ctype,stname,membername) (new col_define(colname,offsetof(stname,membername),sizeof(##((stname *)0)->##membername##),ctype,sizeof(stname)))
#define NullCol() (0)

#define ODBC_SUCCESS_CODE(retcode) ((retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) 
#define STATEMENT_MAX_LEN 1024


#define ODBC_ERROR  -1


#ifdef _UNICODE
	typedef wstring _tstring;
#else
	typedef string _tstring;
#endif

typedef vector<_tstring> _vstring;

class od_desccol 
{
	SQLHSTMT  m_hStmt;
	int		  m_nColNum;
public:
	od_desccol(SQLHSTMT _hstmt,int _nColNum):m_hStmt(_hstmt),m_nColNum(_nColNum){}

	bool Name(TCHAR* szName,int nMaxLen = 65535)
	{
		SQLSMALLINT iColNameLen;
		SQLRETURN retcode;
		retcode = SQLDescribeCol(m_hStmt,
			m_nColNum,
			(SQLTCHAR*)szName,
			nMaxLen,
			&iColNameLen,
			NULL, //datatype
			NULL,//columnsize
			NULL,//decimaldigits
			NULL
			);
		if (ODBC_SUCCESS_CODE(retcode))
			return true;
		return false;
	}

	_tstring Name()
	{
		TCHAR szTmpName[256];
		SQLSMALLINT iColNameLen;
		SQLRETURN retcode;

		retcode = SQLDescribeCol(m_hStmt,
			m_nColNum,
			(SQLTCHAR*)szTmpName,
			sizeof(szTmpName),
			&iColNameLen,
			NULL, //datatype
			NULL,//columnsize
			NULL,//decimaldigits
			NULL
			);
		return szTmpName;
	}

	SQLSMALLINT DataType()
	{
		SQLRETURN retcode;
		SQLSMALLINT dataType;

		retcode = SQLDescribeCol(m_hStmt,
			m_nColNum,
			NULL,
			NULL,
			NULL,
			&dataType, //datatype
			NULL,//columnsize
			NULL,//decimaldigits
			NULL
			);
		return dataType;
	}

	SQLULEN ColumnSize()
	{
		SQLRETURN retcode;
		SQLULEN columnsize;

		retcode = SQLDescribeCol(m_hStmt,
			m_nColNum,
			NULL,
			NULL,
			NULL,
			NULL, //datatype
			&columnsize,//columnsize
			NULL,//decimaldigits
			NULL
			);
		return columnsize;
	}

	od_desccol& operator=(const od_desccol& r)
	{
		m_hStmt = r.m_hStmt;
		m_nColNum = r.m_nColNum;
		return *this;
	}
};

class od_fieldimpl
{
	SQLHSTMT  m_hStmt;
	int		  m_nColNum;
public:
	od_fieldimpl(SQLHSTMT _hstmt,int _nColNum):m_hStmt(_hstmt),m_nColNum(_nColNum){}

	od_fieldimpl& operator=(const od_fieldimpl& r)
	{
		m_hStmt = r.m_hStmt;
		m_nColNum = r.m_nColNum;
		return *this;
	}

	short as_short()
	{
		short tmpret = __getdata<short>(m_hStmt,(SQLSMALLINT)m_nColNum,SQL_C_SHORT,0);
		return tmpret;
	}

	long as_long()
	{
		long tmpret = __getdata<long>(m_hStmt,(SQLSMALLINT)m_nColNum,SQL_C_LONG,0);
		return tmpret;
	}

	float as_float()
	{
		float tmpret = __getdata<float>(m_hStmt,(SQLSMALLINT)m_nColNum,SQL_C_FLOAT,0);
		return tmpret;
	}

	double as_double()
	{
		double tmpret = __getdata<double>(m_hStmt,(SQLSMALLINT)m_nColNum,SQL_C_DOUBLE,0);
		return tmpret;
	}

	_tstring as_string()
	{
		_tstring strRet;
		TCHAR chTmpStr[256];
		SQLINTEGER cbTmpTran;
		SQLRETURN retcode;
		retcode = SQLGetData(m_hStmt,(SQLSMALLINT)m_nColNum,SQL_C_TCHAR,&chTmpStr,sizeof(chTmpStr),&cbTmpTran);
		if (ODBC_SUCCESS_CODE(retcode)){
			if (cbTmpTran != -1)
				strRet = chTmpStr;
		}else if(cbTmpTran > 0){
			strRet.resize(cbTmpTran);
			SQLGetData(m_hStmt,(SQLSMALLINT)m_nColNum,SQL_C_TCHAR,&chTmpStr,sizeof(chTmpStr),&cbTmpTran);
			return strRet;
		}
		return strRet;
	}
	
private:
	template <typename T>
	T __getdata(SQLHSTMT _hstmt,int _nColNum,SQLSMALLINT _ctype,T error_ret) {
		T tmpret;
		SQLINTEGER cbTmpTran;
		SQLRETURN retcode = SQLGetData(_hstmt,(SQLSMALLINT)_nColNum,_ctype,&tmpret,sizeof(tmpret),&cbTmpTran);
		if (ODBC_SUCCESS_CODE(retcode))
			return tmpret;
		return error_ret;
	}
};

class ODResultSet 
{
	HSTMT m_hstmt;
public:
	ODResultSet(HSTMT _hstmt):m_hstmt(_hstmt){}
	
	bool FetchNext()
	{
		SQLRETURN retcode;
		retcode = SQLFetchScroll(m_hstmt,SQL_FETCH_NEXT,0);
		if (ODBC_SUCCESS_CODE(retcode)){
			return true;
		}
		return false;
	}

	od_fieldimpl GetField(SQLSMALLINT _nColNum)
	{
		return od_fieldimpl(m_hstmt,_nColNum);
	}

	od_desccol GetDscCol(SQLSMALLINT _nColNum)
	{
		return od_desccol(m_hstmt,_nColNum);
	}

	long CountCols()
	{
		long nCount = 0;
		SQLNumResultCols(m_hstmt,(SQLSMALLINT*)&nCount);
		return nCount;
	}
};

class ODStatement
{
public:
	ODStatement(){
		m_hstmt = NULL;
	}

	ODStatement(HDBC _hdbc)
	{
		SQLRETURN retcode = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &m_hstmt);
	}

	~ODStatement()
	{
		SQLCloseCursor(m_hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT,m_hstmt);
	}

	SQLHSTMT get_native_statement_handle()
	{
		return m_hstmt;
	}

	//SELECT [param1,[param2..]] FROM table WHERE [condition]
	int Select(const TCHAR* szQuery,const col_define* define[],TCHAR* inbindata);

	//INSERT INTO TABLE(V1,V2,V3) VALUES (?,?,?)
	int Insert(const TCHAR* sztable,const col_define* define[],const TCHAR* outbindata);

	//UPDATE TABLE SET COLUMN1 = ?,COLUMN2 = ?, COLUMN3 = ? WHERE [condition]
	int Update(const TCHAR* szTableName,const col_define* define[],TCHAR* pInBinData,const TCHAR* szWhere);

	bool Prepare(TCHAR* szQuery) 
	{
		SQLRETURN retcode = SQLPrepare(m_hstmt,(SQLTCHAR*)szQuery,SQL_NTS);
		if (ODBC_SUCCESS_CODE(retcode))
			return true;
		return false;
	}

	bool Execute(const TCHAR* szQuery) 
	{
		SQLRETURN retcode;
		retcode = SQLExecDirect(m_hstmt,(SQLTCHAR*)szQuery,SQL_NTS);
		if (ODBC_SUCCESS_CODE(retcode)) 
			return true;
		return false;
	}

	_tstring ClearResult()
	{
		SQLCloseCursor(m_hstmt);
	}

	ODResultSet GetResultSet()
	{
		return ODResultSet(m_hstmt);
	}

	_tstring LastError() 
	{
		TCHAR txtState[1024];
		TCHAR txtErrorMsg[1024];
		SQLRETURN retcode;

		retcode = SQLGetDiagRec(SQL_HANDLE_STMT, m_hstmt, 1, (SQLTCHAR*)txtState, NULL, (SQLTCHAR*)txtErrorMsg, 1024, NULL);
		if (ODBC_SUCCESS_CODE(retcode)) {
			return txtErrorMsg;
		}
		return _tstring();
	}

	_tstring LastStateError()
	{
		TCHAR txtState[1024];
		TCHAR txtErrorMsg[1024];
		SQLRETURN retcode;

		retcode = SQLGetDiagRec(SQL_HANDLE_STMT, m_hstmt, 1, (SQLTCHAR*)txtState, NULL, (SQLTCHAR*)txtErrorMsg, 1024, NULL);
		if (ODBC_SUCCESS_CODE(retcode)) {
			return txtState;
		}
		return _tstring();
	}

	SQLHSTMT m_hstmt;
};


class ODConnection{
	SQLHENV  m_hev;
	SQLHDBC  m_hdbc;
	bool	 m_bInit;
	bool	 m_bOpen;
public:
	ODConnection():m_hev(NULL),m_hdbc(NULL),m_bInit(false),m_bOpen(false){}
	//DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=dbname;UID=sa;PWD=123456;
	bool connect(const TCHAR* szDBInfo,TCHAR * szConnOut = 0,unsigned int nOutLen = 0) {
		SQLRETURN retcode;
		TCHAR chConnStrOut[256];

		if (initialize()){
			retcode = SQLDriverConnect (m_hdbc, NULL, (SQLTCHAR*)szDBInfo, SQL_NTS, (SQLTCHAR*)chConnStrOut, sizeof(chConnStrOut), NULL,SQL_DRIVER_NOPROMPT);
			if (ODBC_SUCCESS_CODE(retcode)){
				if (szConnOut && nOutLen ){
					_stprintf_s(szConnOut,nOutLen,chConnStrOut);
				}
				m_bOpen = true;
			}
		}
		return m_bOpen;
	}

	bool isopen()
	{
		return m_bOpen;
	}

	HDBC get_native_dbc_handle()
	{
		return m_hdbc;
	}

	HENV get_native_env_handle()
	{
		return m_hev;
	}

	void disconnect() {
		SQLDisconnect(m_hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC,m_hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV,m_hev);
	}

	~ODConnection() {
		disconnect();
	}

	

	_tstring LastError() {
		TCHAR txtState[1024];
		TCHAR txtErrorMsg[1024];
		SQLRETURN retcode;

		retcode = SQLGetDiagRec(SQL_HANDLE_DBC, m_hdbc, 1, (SQLTCHAR*)txtState, NULL, (SQLTCHAR*)txtErrorMsg, 1024, NULL);
		if (ODBC_SUCCESS_CODE(retcode)) {
			return txtErrorMsg;
		}
		return _tstring();
	}

	_tstring LastStateError() {
		TCHAR txtState[1024];
		TCHAR txtErrorMsg[1024];
		SQLRETURN retcode;

		retcode = SQLGetDiagRec(SQL_HANDLE_DBC, m_hdbc, 1, (SQLTCHAR*)txtState, NULL, (SQLTCHAR*)txtErrorMsg, 1024, NULL);
		if (ODBC_SUCCESS_CODE(retcode)) {
			return txtState;
		}
		return _tstring();
	}

	_vstring GetDataBaseName(){
		_vstring vDBName;
		if (isopen()) {
			ODStatement stmt(this->get_native_dbc_handle());
			bool boExcute = stmt.Execute(_T("Select Name FROM Master..SysDatabases order by Name"));
			if (boExcute) {
				ODResultSet odset = stmt.GetResultSet();
				if (odset.CountCols()>=1) {
					while (odset.FetchNext()) {
						vDBName.push_back(odset.GetField(1).as_string());
					}
				}
			}
		}
		return vDBName;
	}

	_vstring GetDataTableName(const _tstring& dbname) {
		_vstring vDTName;
		if (isopen()) {
			ODStatement stmt(this->get_native_dbc_handle());
			bool boExcute = stmt.Execute(vformat(_T("Select name FROM %s.sys.SysObjects Where XType='U' orDER BY Name "),dbname.c_str()));
			if (boExcute) {
				ODResultSet odset = stmt.GetResultSet();
				if (odset.CountCols() >= 1) {
					while (odset.FetchNext()) {
						vDTName.push_back(odset.GetField(1).as_string());
					}
				}
			}
		}
		return vDTName;
	}

	_vstring GetColumnName(const _tstring& tbname) {
		_vstring vColName;
		if (isopen()) {
			ODStatement stmt(this->get_native_dbc_handle());
			bool boExcute = stmt.Execute(vformat(_T("select name from syscolumns where id = (select id from sysobjects where xtype='u' and name='%s') "),tbname.c_str()));
			if (boExcute) {
				ODResultSet odset = stmt.GetResultSet();
				if (odset.CountCols() >= 1) {
					while (odset.FetchNext()) {
						vColName.push_back(odset.GetField(1).as_string());
					}
				}
			}
		}
		return vColName;
	}

private:
	bool initialize() {
		do 
		{
			if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hev))
				break;
			if(SQL_SUCCESS!=SQLSetEnvAttr(m_hev,SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3_80, 0)) 
				break;
			if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_DBC, m_hev, &m_hdbc))
				break;
			return true;
			m_bInit = true;
		} while (0);
		return false;
	}
};


class odbc_statement
{
public:
	odbc_statement(HDBC _hdbc)
	{
		SQLRETURN retcode = SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &m_hstmt);
	}

	bool ExcuteDirect(const _tstring& strQuery)
	{
		SQLRETURN retcode;
		retcode = SQLExecDirect(m_hstmt,
			(SQLTCHAR*)strQuery.c_str(),
			strQuery.size()
			);
		if (ODBC_SUCCESS_CODE(retcode)){
			return true;	
		}
		return false;
	}
	
private:
	HSTMT m_hstmt;
	_tstring m_strStament;
};


#endif
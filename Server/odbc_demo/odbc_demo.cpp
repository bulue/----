#include "sqltask.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <time.h>
#include <stringext.h>
using namespace std;

struct stAutoRelease {
	void* pRelease;
	stAutoRelease(void* p):pRelease(p){}
	~stAutoRelease(){if (pRelease) {free(pRelease);pRelease = NULL;}}
};

#define  SETAUTOFREE(p) stAutoRelease __auto_ad79e2##p(p)
#define MALLOC_AUTO_RELEASE_MEM(p,n) char* p = (char*)malloc(n);if(p)ZeroMemory(pData,memory_size);SETAUTOFREE(p); 

struct stDBData
{
	char szString[48];
	float f;
	int   i;
	short s;
	char  chText[1024];
	char  vBinData[1024];
	stDBData()
	{
		ZeroMemory(this,sizeof(*this));
	}
	friend bool operator==(const stDBData& l,const stDBData& r);
};

bool operator_e(const stDBData& l,const stDBData& r)
{
	do 
	{
		if (l.s != r.s){
			printf("短整数不相等 [%d] != [%d]",l.s,r.s);
			break;
		}
		if (l.f != r.f){
			printf("浮点数不相等 [%f] != [%f] \n",l.f,r.f);
			break;
		}
		if (l.i != r.i){
			cout << "整数不相等 " <<  endl;
			break;
		}
		if (strcmp(l.chText ,r.chText) != 0 ){
			cout << "文本不相等" <<  endl;
			break;
		}
		if (strcmp(l.szString ,r.szString) != 0 ){
			cout << "字符串不相等" <<  endl;
			break;
		}
		if (memcmp(l.vBinData,r.vBinData,sizeof(l.vBinData)) != 0 ){
			cout << "二进制" <<  endl;
			break;
		}
		return true;
	} while (0);

	return false;
}

static const col_define* test_define[] = {
	MakeCol("字符串",SQL_C_CHAR,stDBData,szString),
	//MakePCol("二进制数据",SQL_C_BINARY,stDBData,vBinData),
	MakeCol("浮点数",SQL_C_FLOAT,stDBData,f),
	MakeCol("整数",SQL_C_LONG,stDBData,i),
	MakeCol("短整数",SQL_C_SHORT,stDBData,s),
	MakeCol("文本",SQL_C_CHAR,stDBData,chText),
	MakeCol("二进制数据",SQL_C_BINARY,stDBData,vBinData),
	NullCol(),
};

void __randtestdbdata(stDBData* p)
{
	static short static_i = 0;

	p->s = static_i++;
	p->i = rand();
	p->f =(float)rand()/(float)RAND_MAX;
	sprintf(p->szString,"字符串%x",rand());
	sprintf(p->chText,"文本内容:%06f + %07f = %08f",p->f ,p->f/100.0 ,p->f+p->f/100.0);
	sprintf(p->vBinData,"二进制内容:[%d][%x][%f][%s][%s]",p->s,p->s,p->f,p->szString,p->chText);
}

void __randtestdbdata_v2(stDBData* p)
{	
	p->i = rand();
	p->f =(float)rand()/(float)RAND_MAX;
	sprintf(p->szString,"字符串v2%x",rand());
	sprintf(p->chText,"文本内容v2:%06f + %07f = %08f",p->f ,p->f/100.0 ,p->f+p->f/100.0);
	sprintf(p->vBinData,"二进制内容v2:[%d][%x][%f][%s][%s]",p->s,p->s,p->f,p->szString,p->chText);
}

ostream& operator<< (ostream& stream,stDBData & data)
{
	stream << data.szString << " "
		<< data.f << " "
		<< data.i << " "
		<< data.s << " "
		<< data.chText << " "
		<< data.vBinData << " ";
	return stream;
}

#include <boost/timer/timer.hpp>

int main()
{
	do
	{
		ODConnection csqlconn;
		if (!csqlconn.connect("DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=bulue_db;UID=sa;PWD=19881226;")) {
			cout << "LastError:"<<csqlconn.LastError() << endl;
			cout << "LastStateError:" <<csqlconn.LastStateError() << endl;
			return -1;
		}
		do 
		{
			SQLRETURN retcode = 0;
			SQLTCHAR ServerName[256]; 
			SQLTCHAR Description[256];
			SQLSMALLINT len1 = 0;
			SQLSMALLINT len2 = 0;
			ODStatement odstmt(csqlconn.get_native_dbc_handle());
			//retcode = odstmt.Execute(wxT("Select * from Odbc_test"));
			while(ODBC_SUCCESS_CODE(retcode)) {
				retcode = SQLDataSources(csqlconn.get_native_env_handle(),
					SQL_FETCH_NEXT,
					ServerName,256,&len1,
					Description,256,&len2
					);
				cout << "ServerName: " << ServerName << endl;
				cout << "Description: "<< Description << endl;
			}
		} while (0);

		do 
		{
			ODStatement statement(csqlconn.get_native_dbc_handle());
			statement.Execute("delete from Odbc_test");
		} while (0);

		const int test_count = 1000;	//测试数据量
		std::map<short,stDBData> mapDbData;
		srand (time(NULL));
		do{
			ODStatement statement(csqlconn.get_native_dbc_handle());
			for (int i = 0;i < test_count; ++i)
			{
				stDBData data;
				__randtestdbdata(&data);
				int count = statement.Insert("Odbc_test",test_define,(char*)&data);
				if (count  == ODBC_ERROR)
				{
					cout << statement.LastStateError() << endl;
					cout << statement.LastError() << endl;
				}
				mapDbData.insert(std::map<short,stDBData>::value_type(data.s,data));
			}
		}while (0);

		do 
		{
			boost::timer::auto_cpu_timer t;
			int memory_size = test_count* sizeof(stDBData);
			MALLOC_AUTO_RELEASE_MEM(pData,memory_size);
			ODStatement statement(csqlconn.get_native_dbc_handle());
			int rows = statement.Select("Select * from Odbc_test",test_define,pData);

			int nColCount = 0;
			SQLNumResultCols(statement.get_native_statement_handle(),(SQLSMALLINT*)&nColCount);
			for (int i = 1; i < nColCount; ++i)
			{
				od_desccol dc = od_desccol(statement.get_native_statement_handle(),i);
				printf("列名:%s;列大小%d\n",dc.Name().c_str(),dc.ColumnSize());
			}
			if (rows == ODBC_ERROR) {
				cout << statement.LastStateError() << endl;
				cout << statement.LastError() << endl;
			}else {
				printf("一共读入%d行数据\n",rows);
				stDBData* arrDbData = (stDBData*)pData;
				int correct_count = 0;
				for (int i = 0;i  < rows; ++i ){
					if (operator_e(arrDbData[i],mapDbData[arrDbData[i].s])){
						correct_count++;
					}
					else{
						printf("第%d条数据不匹配\n",i);
					}
				}
				printf("正确匹配条数 %d\n",correct_count);
			}		
		} while (0);

		do 
		{
			ODStatement statement(csqlconn.get_native_dbc_handle());
			std::map<short,stDBData>::iterator itdb;
			for (itdb = mapDbData.begin(); itdb != mapDbData.end(); ++itdb)
			{
				__randtestdbdata_v2(&itdb->second);

				int rows = statement.Update("Odbc_test",test_define,(char*)&itdb->second,vformat("WHERE 短整数=%d",itdb->second.s));
				if (rows == ODBC_ERROR)
				{
					cout << statement.LastStateError() << endl;
					cout << statement.LastError() << endl; 
				}
			}
		} while (0);

		do 
		{
			int memory_size = test_count* sizeof(stDBData);
			MALLOC_AUTO_RELEASE_MEM(pData,memory_size);
			ODStatement statement(csqlconn.get_native_dbc_handle());
			int rows = statement.Select("Select * from Odbc_test",test_define,pData);

			int nColCount = 0;
			SQLNumResultCols(statement.get_native_statement_handle(),(SQLSMALLINT*)&nColCount);
			for (int i = 1; i < nColCount; ++i)
			{
				od_desccol dc = od_desccol(statement.get_native_statement_handle(),i);
				printf("列名:%s;列大小%d\n",dc.Name().c_str(),dc.ColumnSize());
			}
			if (rows == ODBC_ERROR) {
				cout << statement.LastStateError() << endl;
				cout << statement.LastError() << endl;
			}else {
				printf("一共读入%d行数据\n",rows);
				stDBData* arrDbData = (stDBData*)pData;
				int correct_count = 0;
				for (int i = 0;i  < rows; ++i ){
					if (operator_e(arrDbData[i],mapDbData[arrDbData[i].s])){
						correct_count++;
					}
					else{
						printf("第%d条数据不匹配\n",i);
					}
				}
				printf("正确匹配条数 %d\n",correct_count);
			}		
		} while (0);


	}while(false);
	return 0;
}
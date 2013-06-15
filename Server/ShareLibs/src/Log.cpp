#include<windows.h>

enum concol
{
	black=0,
	dark_blue=1,
	dark_green=2,
	dark_aqua,dark_cyan=3,
	dark_red=4,
	dark_purple=5,dark_pink=5,dark_magenta=5,
	dark_yellow=6,
	dark_white=7,
	gray=8,
	blue=9,
	green=10,
	aqua=11,cyan=11,
	red=12,
	purple=13,pink=13,magenta=13,
	yellow=14,
	white=15
};


#define std_con_out GetStdHandle(STD_OUTPUT_HANDLE)
inline void setcolor(int textcol,int backcol)
{
	{
		if((textcol%16)==(backcol%16))
		textcol++;
	}
	textcol%=16;backcol%=16;
	unsigned short wAttributes= ((unsigned)backcol<<4)|(unsigned)textcol;
	SetConsoleTextAttribute(std_con_out, wAttributes);
}

inline void setcolor(concol textcol,concol backcol)
{
	setcolor(int(textcol),int(backcol));
}
#undef std_con_out

#include "..\Log.h"
#include <stdio.h>
#include <stdarg.h>

CLog CLog::m_obj;

CLog::CLog()
{
}

CLog::~CLog()
{

}

void CLog::DebugInfo( const char*format,... )
{
	char buffer[1024];
	va_list args;
	va_start(args,format);
	vsprintf_s(buffer,format,args);
	va_end(args);
	setcolor(blue,black);
	printf("DBG_INFO:%s\n",buffer);
}

void CLog::WarningInfo(const char*format,...){
	char buffer[1024];
	va_list args;
	va_start (args, format);
	vsprintf_s (buffer,format, args);
	va_end (args);
	setcolor(yellow,black);
	printf("WAR_INFO:%s\n",buffer);
}
void CLog::ErrorInfo(const char*format,...){
	char buffer[1024];
	va_list args;
	va_start (args, format);
	vsprintf_s (buffer,format, args);
	va_end (args);
	setcolor(red,black);
	printf("ERR_INFO:%s\n",buffer);
}

void CLog::ShowInfo( const char*format,... )
{
	char buffer[1024];
	va_list args;
	va_start (args, format);
	vsprintf_s (buffer,format, args);
	SetConsoleTitle(buffer);
	va_end(args);
}

void CLog::printf(const char* format,...)
{
	m_lock.lock();
	char buffer[1024];
	va_list args;
	va_start (args, format);
	vsprintf_s (buffer,format, args);
	va_end (args);
	::printf("%s",buffer);
	m_lock.unlock();
}

//////////////////////////////////////////////////////////////////////////
#include <time.h>
#include <stdio.h>
#include <stringext.h>

bool CLogger::debug( const char * pattern, ... )
{
	static Level lvl(__FUNCTION__,eERROR,eERROR,false);
	static char szMsg[1024];

	va_list args;
	va_start( args, pattern );
	vsnprintf_s(szMsg,1024,pattern,args);
	va_end( args );

	doWriteLog(lvl,szMsg);
	return true;
}

bool CLogger::error( const char * pattern, ... )
{
	static Level lvl(__FUNCTION__,eERROR,eERROR,false);
	static char szMsg[1024];

	va_list args;
	va_start( args, pattern );
	vsnprintf_s(szMsg,1024,pattern,args);
	va_end( args );

	doWriteLog(lvl,szMsg);
	return true;
}

bool CLogger::info( const char * pattern, ... )
{
	static Level lvl(__FUNCTION__,eERROR,eERROR,false);
	static char szMsg[1024];

	va_list args;
	va_start( args, pattern );
	vsnprintf_s(szMsg,1024,pattern,args);
	va_end( args );

	doWriteLog(lvl,szMsg);
	return true;
}

void CLogger::doWriteLog( Level& level,const char* pszMsg )
{
	time_t t = time(NULL);
	tm* timeinfo;

	timeinfo = localtime(&t);
	char * szTimeInfo = vformat("%d-%d-%d %d:%d:%d",timeinfo->tm_year+1900,timeinfo->tm_mon+1,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec); 
	printf_s("%s:%s\n",szTimeInfo,pszMsg);	
#ifdef _DEBUG
	if (level.writelevel < eALL) 
#else
	if (level.writelevel < eDEBUG) 
#endif
	{
		fprintf(pFile,"%s:%s\n",szTimeInfo,pszMsg);
		fflush(pFile);
	}
}

CLogger::CLogger()
{
	time_t t = time(NULL);
	tm* timeinfo;
	
	timeinfo = localtime(&t);
	const char* szFileName = vformat("%d-%d-%d %d %d.txt",timeinfo->tm_year+1900,timeinfo->tm_mon+1,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min);
	pFile = fopen (szFileName,"w");
}

CLogger::~CLogger()
{
	fclose(pFile);
}


CLogger g_logger;

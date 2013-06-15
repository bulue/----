#ifndef __LOG_H__
#define __LOG_H__

#include <boost/thread/mutex.hpp>

class CLog
{
	static CLog m_obj;
	boost::mutex m_lock;
	virtual void printf(const char* format,...);
public:
	CLog();
	~CLog();

	static CLog& instance(){return m_obj;}

	void DebugInfo(const char*format,...);
	void WarningInfo(const char*format,...);
	void ErrorInfo(const char*format,...);

	void ShowInfo(const char*format,...);
};

#define LogInst (CLog::instance())

class CLogger
{
	enum eLoggerLevel
	{
		eOFF=0,
		eFORCE=0,
		eFATAL=1,
		eERROR=2,
		eALARM=2,
		eWARN=3,
		eIFFY=3,
		eINFO=4,
		eTRACE=4,
		eDEBUG=5,
		eGBUG=5,
		eALL=250,
	};
	FILE * pFile;
public:
	CLogger();
	~CLogger();
	bool debug(const char * pattern, ...);
	bool error(const char * pattern, ...);
	bool error_out(const char * pattern,...);
	bool info(const char * pattern, ...);
	bool fatal(const char * pattern, ...);
	bool warn(const char * pattern, ...);
	bool alarm(const char * pattern, ...);
	bool iffy(const char * pattern, ...);
	bool trace(const char * pattern, ...);
	bool gbug(const char * pattern, ...);

	struct Level
	{
		const char* name;
		const BYTE writelevel;
		const BYTE showlevel;
		const bool realtimewrite;

		Level(const char* sname,BYTE btwritelevel,BYTE btshowlevel,bool borealtimewrite)
			:name(sname),writelevel(btwritelevel),
			showlevel(btshowlevel),realtimewrite(borealtimewrite){
		}
	};

protected:
	virtual void doWriteLog(Level& level,const char* pszMsg);
};

extern CLogger g_logger;

#endif	//__LOG_H__
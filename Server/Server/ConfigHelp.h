#ifndef __CONFIGHELP__H_
#define __CONFIGHELP__H_
#include "Help/XmlHelp.h"

struct Config
{
	std::string ip;
	unsigned short port;
};

class CConfig:public Config
{
public:
	CConfig()
	{
		CXmlHelp<CConfig> m_load;
		m_load.Load(this,"config.xml");
	}
};

TiXmlElement & operator >> (TiXmlElement & dataStream, CConfig& value);

extern CConfig g_config;

#endif //__CONFIGHELP__H_
#include "ConfigHelp.h"

CConfig g_config;

TiXmlElement & operator >> (TiXmlElement & dataStream, CConfig& value)
{
	dataStream.QueryValueAttribute("ip",&value.ip);
	dataStream.QueryValueAttribute("port",&value.port);
	return dataStream;
}
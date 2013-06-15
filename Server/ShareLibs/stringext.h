#ifndef __VFORMAT_H_
#define __VFORMAT_H_

#include <stdarg.h>
#include <tchar.h>

static TCHAR* vformat(const TCHAR* format,...)
{
	static TCHAR string[1024];
	va_list vl;
	va_start(vl,format);
#ifdef _UNICODE
	_vsnwprintf(string,sizeof(string),format,vl);
#else
	_vsnprintf(string,sizeof(string),format,vl);
#endif
	va_end(vl);

	return string;
}


#endif	//__VFORMAT_H_
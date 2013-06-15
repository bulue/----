#ifndef __CLDEFINE_H__
#define __CLDEFINE_H__

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef signed short SWORD;
#ifndef _WIN32
typedef unsigned int DWORD;
#endif
typedef signed int SDWORD;
typedef unsigned long long QWORD;
typedef signed long long SQWORD;


#define TOSTR(x) #x
#define TOSTR1(x) TOSTR(x)
#define __FF_LINE__ __FILE__"("TOSTR1(__LINE__)")["__TIMESTAMP__"]:"__FUNCTION__


#define _CHECK_PACKAGE_LEN(st,cmdlen){\
	if (sizeof(st)> cmdlen){\
	g_logger.debug("收到包 %s 接收长度 %d < 定义长度 %d ", #st,cmdlen,sizeof(st));\
	return false;}\
}

template<class T> 
inline	void constructInPlace(T  *_Ptr)
{	
	new ((void  *)_Ptr) T();
}

#endif
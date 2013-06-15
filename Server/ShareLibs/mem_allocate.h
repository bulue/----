#ifndef _MEM_ALLOCATE_H__
#define _MEM_ALLOCATE_H__

#include <new.h>
#include <map>
#include <fstream>
#include <stdio.h>

#define SF_MALLOC malloc
#define SF_FREE   free

#define SF_NEW new
#define SF_DELETE delete

struct stAutoRelease {
	void* pRelease;
	stAutoRelease(void* p):pRelease(p){}
	~stAutoRelease(){if (pRelease) {SF_FREE(pRelease);pRelease = NULL;}}
};

#define  SETAUTOFREE(p) stAutoRelease __auto_ad79e2##p(p)
#define MALLOC_AUTO_RELEASE_MEM(p,n) char* p = (char*)SF_MALLOC(n);if(p)ZeroMemory(pData,memory_size);SETAUTOFREE(p); 




#endif
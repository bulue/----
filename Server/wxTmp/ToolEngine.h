#ifndef __TOOL_ENGINE_
#define __TOOL_ENGINE_

#include "Singleton.h"
#include "sqltask.h"

class ToolEngine:public Singleton<ToolEngine> 
{
public:
	ODConnection m_DBConn;
};

#endif	//__TOOL_ENGINE_
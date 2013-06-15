#ifndef __USER_H__
#define __USER_H__

#include "ShareLib.h"
#include "../StaticConfigs/Server_Messages.h"

struct UserData
{
	std::string name;
	std::string pwd;
};


class CUser:public IData,public UserData
{
public:
	std::string getName() {return name;}
	std::string getPwd() {return pwd;}
};

extern CUser g_user;

#endif	//__USER_H__
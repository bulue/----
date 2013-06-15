#ifndef __USERMANAGER_H__
#define __USERMANAGER_H__

#include "User.h"

class CUserManager:public IDataPtrManager<IDataPtr,std::string>
{
	static CUserManager m_obj;

	int m_count;
public:
	CUserManager():m_count(0) {}
	static CUserManager& instance(){return CUserManager::m_obj;}
	IDataPtr CreateUser(std::string& name,std::string& pwd)
	{
		UserData data;
		data.name = name;
		data.pwd = pwd;
		//return THISTYPE::insert(name,&data);
		return IDataPtr();
	}
	//using namespace KllLib;
	//CSession session;
	u_long Login(ShareLib::Server::CSession& session,const Server::Messages::CMsg_LogIn& msg);
	u_long LogOut(ShareLib::Server::CSession& session,const Server::Messages::CMsg_LogOut& msg);
};

#define USERS (CUserManager::instance())
#endif	//__USERMANAGER_H__
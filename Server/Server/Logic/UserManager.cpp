#include "UserManager.h"

CUserManager CUserManager::m_obj;

u_long CUserManager::Login(ShareLib::Server::CSession& session,const Server::Messages::CMsg_LogIn& msg)
{
	//printf("login success...\n");
	//printf("User:%s\n",msg->name.c_str());
	//printf("Password:%s\n",msg->pwd.c_str());

	//std::string str("Login success");

	//if (CreateUser(msg->name,msg->pwd))
	//{
	//	//printf("Create User Success.");
	//}
	LogInst.DebugInfo("%d messages.",m_count);
	Server::Messages::CMsg_LogOut reply;
	reply->Result = 100;
	session.send_msg(&reply);
	//session.async_write((void*)str.data(),str.size());
	return 0;
}

u_long CUserManager::LogOut( ShareLib::Server::CSession& session,const Server::Messages::CMsg_LogOut& msg )
{
	LogInst.DebugInfo("LogOut Success \n");
	return 0;
}
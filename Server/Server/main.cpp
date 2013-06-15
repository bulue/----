
#include "ShareLib.h"
#include "Logic/UserManager.h"
#include "ConfigHelp.h"

using namespace ShareLib::Server;

class CEchoSession:public ShareLib::Server::CSession
{
public:
	void handle_read(void* buffer,const boost::system::error_code& error,std::size_t bytes_transferred)
	{
		LogInst.DebugInfo("Recieved Msg");
	}
};

CSession * getsession(CSessionManager& mana)
{
	return new CSession(mana);
}

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: server <port>\n";
			return 1;
		}

		//CClient client;
		//for (size_t i=0 ;i < 100; i++)
		//{
		//	client.add_client("127.0.0.1",27015);
		//}
		//client.run();
		CServer s(g_config.port,getsession);
		s.add_message_map(&USERS,&CUserManager::Login);
		s.add_message_map(&USERS,&CUserManager::LogOut);
		boost::thread(&CServer::run,&s);
		boost::thread::sleep(boost::get_system_time()+boost::posix_time::seconds(1000));
		LogInst.DebugInfo("服务器要关闭了");
		boost::thread::sleep(boost::get_system_time()+boost::posix_time::seconds(5));
		//s.stop();
		boost::mutex mutex;
		mutex.lock();
		mutex.lock();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <list>
#include <set>
#include "Log.h"
#include "MainWnd.h"
#include "Singleton.h"


using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std;

typedef boost::system::error_code errcode;

using boost::asio::ip::tcp;

class session
{
public:
	session(boost::asio::io_service& io_service)
		: socket_(io_service)
	{
	}

	tcp::socket& socket()
	{
		return socket_;
	}

	void start()
	{
		socket_.async_read_some(boost::asio::buffer(data_, max_length),
			boost::bind(&session::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

private:
	void handle_read(const boost::system::error_code& error,
		size_t bytes_transferred)
	{
		if (!error)
		{
			boost::asio::async_write(socket_,
				boost::asio::buffer(data_, bytes_transferred),
				boost::bind(&session::handle_write, this,
				boost::asio::placeholders::error));
		}
		else
		{
			delete this;
		}
	}

	void handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			socket_.async_read_some(boost::asio::buffer(data_, max_length),
				boost::bind(&session::handle_read, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			delete this;
		}
	}

	tcp::socket socket_;
	enum { max_length = 1024 };
	char data_[max_length];
};

class server
{
public:
	server(boost::asio::io_service& io_service, short port)
		: io_service_(io_service),
		acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
	{
		start_accept();
	}

private:
	void start_accept()
	{
		session* new_session = new session(io_service_);
		acceptor_.async_accept(new_session->socket(),
			boost::bind(&server::handle_accept, this, new_session,
			boost::asio::placeholders::error));
	}

	void handle_accept(session* new_session,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			new_session->start();
		}
		else
		{
			delete new_session;
		}

		start_accept();
	}

	boost::asio::io_service& io_service_;
	tcp::acceptor acceptor_;
};

boost::asio::io_service g_io_service;

void OnInput(LPSTR szString) {
	if (szString[0] == '@') {
		if (strcmp(&szString[1],"close") == 0) {
			g_io_service.stop();
			g_logger.error("=======Echo is closing...=====");
		}
	}
}

void __stdcall ShowLogFunc(CLogger::Level& level,const char* logtime,const char* pszMsg)
{
	stSetListTimeText *cmd = new stSetListTimeText();
	cmd->dwColor = level.showcolor;
	strcpy_s(cmd->szMsg,sizeof(cmd->szMsg),pszMsg);
	strcpy_s(cmd->szTime,sizeof(cmd->szTime),logtime);
	CWndCtl::getMe().push_back((stBaseCmd*)cmd);
}

void OnWndClose() {
	g_io_service.stop();
	CWndCtl::getMe().SetListViewText("=====IO_SERVICE IS CLOSING...======",RGB(255,0,0));
}

int _mmain()
{
	try
	{
		CWndCtl::getMe().m_input = OnInput;
		CWndCtl::getMe().m_OnCloseWndFunc = OnWndClose;
		g_logger.SetShowLogFunc(ShowLogFunc);

		server s(g_io_service, 27015);
		g_io_service.run();
	}
	catch (std::exception& e)
	{
		g_logger.error("exception:%s",e.what());
	}

	return 0;
}
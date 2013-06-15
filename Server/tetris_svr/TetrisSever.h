#ifndef __TETRIS_SERVER_H__
#define __TETRIS_SERVER_H__

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "network/tcpsocket.h"
#include "SvrBoard.h"


class CPlayer;


class session:
	public CTcpSocketBase 
{
public:
	session(io_service& ios);

	virtual void start();

	virtual bool msgParse(char* cmd,unsigned int len);

	virtual void do_delete_self(const boost::system::error_code& error);

	CPlayer* m_player;
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


#endif
#ifndef __TCPSOCKET_H__
#define __TCPSOCKET_H__

#include "packet.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace boost::asio::ip;
using namespace boost::asio;

/************************************************************************
* class name:      CTcpSocketBase										*
* description:	   Receiving a packet in a fixed format:packet head(len) and body(context)*
* virtual funcion: start,do_delete_self,msgParse						*
************************************************************************/

class CTcpSocketBase{
public:
	CTcpSocketBase(io_service& ios):m_socket(ios),m_deleted(false){}
	virtual ~CTcpSocketBase(){}
	tcp::socket& socket() {return  m_socket;}

	virtual void start();

	void raw_write(char* msg,unsigned int len);

	void write(void *pcmd , unsigned int len);

	void async_connect(const char* szAddress,short port);

	void connect(const char* szAddress,short port);

	void close();

protected:
	virtual bool msgParse(char* cmd,unsigned int len){
		return true;
	}
private:
	void connect_handler(const boost::system::error_code& error);

	void read_head(int offset);

	void read_body(int offset);

	void handler_read_body(const boost::system::error_code& error, std::size_t bytes_transferred,int offset);

	void handler_read_head(const boost::system::error_code& error, std::size_t bytes_transferred,int offset);

	void handler_write(const boost::system::error_code& error, std::size_t bytes_transferred);

	void delete_self(const boost::system::error_code& error) ;

	virtual void do_delete_self(const boost::system::error_code& error) {
		delete this;
	}

	union {
		char szPacketBuffer[stBaseMessage::_MAX_MSG_SIZE];
		struct {
			stBaseMessage msgRecv;
		};
	};

	bool m_deleted;
	tcp::socket  m_socket;
};



#endif 
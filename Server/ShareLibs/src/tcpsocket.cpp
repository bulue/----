#include "network/tcpsocket.h"


void CTcpSocketBase::start(){
	read_head(0);
}


void CTcpSocketBase::raw_write(char* msg,unsigned int len){
	m_socket.async_write_some(buffer(msg,len),
		boost::bind(&CTcpSocketBase::handler_write,this,_1,_2));
}


void CTcpSocketBase::write(void *pcmd , unsigned int len) {
	char cmdbuffer[stBaseMessage::_MAX_MSG_SIZE];
	PTR_CMD(stBaseMessage,cmd,cmdbuffer);
	cmd->size = len;
	memcpy(cmd->body(),pcmd,len);
	raw_write(cmd->head(),cmd->alllen());
}

void CTcpSocketBase::async_connect(const char* szAddress,short port){
	m_socket.async_connect(tcp::endpoint(address::from_string(szAddress),port),
		boost::bind(&CTcpSocketBase::connect_handler,this,_1));
}

void CTcpSocketBase::connect(const char* szAddress,short port) {
	m_socket.connect(tcp::endpoint(address::from_string(szAddress),port));
}

void CTcpSocketBase::close(){
	m_socket.close();
}

void CTcpSocketBase::connect_handler(const boost::system::error_code& error){
	if (!error){
		start();
	}
	else 
		delete_self(error);
}

void CTcpSocketBase::read_head(int offset) {
	m_socket.async_read_some(buffer(msgRecv.head()+offset,msgRecv.headlen()-offset),
		boost::bind(&CTcpSocketBase::handler_read_head,this,_1,_2,offset));
}

void CTcpSocketBase::read_body(int offset) {
	m_socket.async_read_some(buffer(msgRecv.body()+offset,msgRecv.bodylen()-offset),
		boost::bind(&CTcpSocketBase::handler_read_body,this,_1,_2,offset));
}


void CTcpSocketBase::handler_read_body(const boost::system::error_code& error, std::size_t bytes_transferred,int offset) {
	if (!error) {
		int curr_offset = offset + (int)bytes_transferred;
		if (curr_offset < (int)msgRecv.bodylen()) {
			read_body(curr_offset);
		}else {
			msgParse(msgRecv.head(),msgRecv.alllen());
			read_head(0);
		}
	}else {
		delete_self(error);
	}
}


void CTcpSocketBase::handler_read_head(const boost::system::error_code& error, std::size_t bytes_transferred,int offset){
	if (!error) {
		int curr_offset = offset + (int)bytes_transferred; 
		if (curr_offset < (int)msgRecv.headlen()) {
			read_head(curr_offset);
		}else {
			read_body(0);
		}
	}
	else {
		//g_logger.info("read_head socket = [%08x] error = [%s]",this,error.message().c_str());
		delete_self(error);
	}
}

void CTcpSocketBase::handler_write(const boost::system::error_code& error, std::size_t bytes_transferred){
	if (error) {
		//g_logger.info("write socket = [%08x] error= [%s]",this,error.message().c_str());
		delete_self(error);
	}
}

void CTcpSocketBase::delete_self(const boost::system::error_code& error) {
	if (!m_deleted) {
		m_deleted = true;
		m_socket.get_io_service().post(boost::bind(&CTcpSocketBase::do_delete_self,this,error));
	}
}


#include "Connection.h"

namespace ShareLib{namespace Server{
using namespace ShareLib::Data;


void CSession::handle_read( void* buffer,const boost::system::error_code& error,std::size_t bytes_transferred ,void* p)
{
	do 
	{
		status_.clr_bit<READ_IN_PROGRESS>();


		if (status_.get_bit<CLOSED>())
		{
			break;
		}

		if (error)
		{
			progress_error(error);
		}

		dispatch(buffer,bytes_transferred);

		async_read();
	} while (0);
	
	Free(buffer);
}

void CSession::handle_write( char* buffer,const boost::system::error_code& error,std::size_t bytes_transferred )
{
	do 
	{
		status_.clr_bit<WRITE_IN_PROGRESS>();

		if (is_closed())
		{
			break;
		}

		if (error)
		{
			progress_error(error);
			break;
		}
		if ( !is_reading())
		{
			async_read();
		}

		//LogInst.DebugInfo("send msg ok.\n");
	} while (0);

	Free(buffer);
}

void CSession::start()
{
	async_read();
	on_start();
}

void CSession::async_connect( const std::string ip,unsigned short port )
{
	using namespace boost::asio::ip ;
	async_connect(TCPENDPOINT(address::from_string(ip),port));
}

void CSession::async_connect( TCPENDPOINT endpoint)
{
	socket_.async_connect(endpoint,
		boost::bind(&CSession::handle_connect,shared_from_this(),
		boost::asio::placeholders::error));
}

void CSession::handle_connect( const boost::system::error_code& error )
{
	do 
	{
		if (error)
		{
			progress_error(error);
		}

		if (is_closed())
			break;

		//LogInst.DebugInfo("connect_handler(),connected %s successed.\n",remote_address().c_str());

		start();

	} while (0);
}

void CSession::async_read()
{
	async_read(Malloc());
}

void CSession::async_read( void* buffer )
{
	socket_.async_read_some(boost::asio::buffer(buffer,BufferSize()),
		boost::bind(&CSession::handle_read,
		shared_from_this(),
		buffer,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred,
		buffer));
	
	status_.set_bit<READ_IN_PROGRESS>();
}

void CSession::async_read(void* dbuffer,void*basebuffer){
	socket_.async_read_some(boost::asio::buffer(dbuffer,BufferSize() - ((int)dbuffer - (int)basebuffer)),
		boost::bind(&CSession::handle_read,
		shared_from_this(),
		dbuffer,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred,
		basebuffer));

	status_.set_bit<READ_IN_PROGRESS>();
}

void CSession::async_write( void* buffer,size_t size )
{
	socket_.async_write_some(boost::asio::buffer(buffer,size),
		boost::bind(&CSession::handle_write,
		shared_from_this(),
		(char*)buffer,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));

	status_.set_bit<WRITE_IN_PROGRESS>();
}

void CSession::send_msg( IMessage* msg )
{
	size_t size(0);
	void * buff = on_msg2buffer(msg,size);
	async_write(buff,size);
}


std::string CSession::remote_address()
{
	std::string ret;
	boost::system::error_code ignore;
	ret = socket().remote_endpoint(ignore).address().to_string();
	if (ignore)
	{
		//LogInst.WarningInfo("Exception%s",ignore.message().c_str());
	}
	return  ret;
}

void* CSession::on_msg2buffer( IMessage* msg,size_t& length )
{
	void* ret = Malloc();

	CTypeStream dataStream(ret,BufferSize(),0);
	dataStream << *msg;

	length = dataStream.data_size();
	return ret;
}

void CSession::progress_error( const boost::system::error_code& error )
{
	//LogInst.ErrorInfo("Session Error:%s",error.message().c_str());
	close();
	remove();
}

void CSession::remove()
{
	session_manager_.Lock();
	session_manager_.ISessionManager::remove(getid());
	session_manager_.unLock();
}

void CSession::close()
{
	if (is_closed())
		return;
	status_.set_bit<CLOSED>();
	boost::system::error_code ignored;
	socket_.close(ignored);
}

void* CSession::Malloc()
{
	return session_manager_.GetBuffer();
}

void CSession::Free( void* buff)
{
	return session_manager_.ReleaseBuffer(buff);
}

size_t CSession::BufferSize()
{
	return CSessionManagerBase::BufferSize();
}

bool CSession::dispatch( void* buff,size_t size )
{
	if (size == 0)
	{
		//LogInst.DebugInfo("接受到0字节消息内容");
		return false;
	}
	ShareLib::Data::CTypeStream stream(buff,BufferSize(),size);
	u_long msg_id = *(u_long *)stream.current();
	IMsgMapManager& msgMana = session_manager_.getMapManager();
	if (msgMana[msg_id])
	{
		msgMana[msg_id]->create_exec(stream,static_cast<void*>(this));
		return true;
	}
	
	//LogInst.ErrorInfo("Error Message: %d\n",msg_id);
	return false;

}

void CSessionManager::stop()
{
	if (m_stop)
		return;

	m_stop = true;

	io_service_.stop();
	//ISessionManager::ITERATOR iter = ISessionManager::get_iterator();
	//for (ISessionManager::VALUEPTR value = ISessionManager::walk(iter);value;value = walk(iter))
	//{
	//	io_service_.post(boost::bind(&CSession::close,value));
		//value->close();
	//}
	ISessionManager::clear();
	//io_service_.post(boost::bind(&boost::asio::io_service::stop,boost::ref(io_service_)));

	for (std::list<thread_ptr>::iterator it = m_workthreads.begin();it != m_workthreads.end(); it ++){
		(*it)->join();
	}
}

}}
#ifndef __SHARE_LIB_CONNECTION_H__
#define __SHARE_LIB_CONNECTION_H__

#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <boost\noncopyable.hpp>
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\array.hpp>
#include "..\Data\Message.h"
#include "..\Data\IObjectPtrManager.h"
#include "..\Data\interface.h"
#include "..\Data\BitManager.h"
#include "..\Data\BufferCollector.h"
#include <boost\thread\mutex.hpp>
#include <boost\pool\pool.hpp>
#include <boost\thread.hpp>


namespace ShareLib{namespace Server{
using namespace ShareLib::Data;

class CSession;
class CSessionManager;

typedef boost::asio::ip::tcp::endpoint TCPENDPOINT;

typedef IManager<u_long,CMSGFUN> IMsgMapManager;
typedef boost::shared_ptr<CSession> CCSessionPtr;
typedef IAutoIDObjectPtrManager<CSession> ISessionManager;

#define MAXBUFFERSIZE (4096*2)

static int count = 0;

class CSessionManagerBase:public IMsgMapManager,public ISessionManager
{
	boost::mutex m_mem_Locker;
	// The allocator to use for handler-based custom memory allocation.
	//boost::pool<> m_pool;
	CSessionBufferCollector m_bufferCollector;

	boost::mutex m_lock;

protected:
	CSessionManagerBase():m_bufferCollector(BufferSize()){}
	boost::asio::io_service m_IOService;

	inline IMsgMapManager& getMapManager() const {return static_cast<IMsgMapManager&>(*(CSessionManagerBase*)this);}
	inline ISessionManager& getSessionManager() const{return static_cast<ISessionManager&>(*(CSessionManagerBase*)this);}

	void* GetBuffer()
	{
		//LogInst.DebugInfo("Buffer%d",++count);
		return m_bufferCollector.GetBuffer();
	}

	void ReleaseBuffer(void* p)
	{
		//LogInst.DebugInfo("Buffer%d",--count);
		m_bufferCollector.ReleaseBuffer(p);
	}

	static const size_t BufferSize()
	{
		return MAXBUFFERSIZE;
	}

	void Lock() {m_lock.lock();}
	void unLock(){m_lock.unlock();}
};

typedef  CSession* (*GETNEWSESSION)(CSessionManager& );

class CSessionManager:public CSessionManagerBase
{
	friend class CSession;

protected:
	boost::asio::io_service io_service_;

	bool m_stop;

	GETNEWSESSION SsnCreator;
	typedef boost::shared_ptr<boost::thread> thread_ptr;
	//thread_ptr m_WorkThread;
	std::list<thread_ptr> m_workthreads;
public:
	CSessionManager(GETNEWSESSION ssc):m_stop(false),SsnCreator(ssc){}
	~CSessionManager(){}
	inline boost::asio::io_service& IOService() 
	{
		return io_service_;
	}

	inline void run(int thread_num = 1)
	{
		for (int i = 0; i < thread_num - 1; ++i){
			m_workthreads.push_back(thread_ptr(new boost::thread( boost::bind(&boost::asio::io_service::run,&io_service_))));
		}
		io_service_.run();
	}

	void stop();

	inline bool is_stop(){return m_stop;}

};

class CSession
	: public boost::enable_shared_from_this<CSession>
{
	enum{
		WRITE_IN_PROGRESS=1,
		READ_IN_PROGRESS=2,
		CLOSED=4,
	};
public:
	CSession(CSessionManager& mana)
		: session_manager_(mana)
		,socket_(mana.IOService())
		,id_(0)
	{
	}

	boost::asio::ip::tcp::socket& socket(){return socket_;}

	inline u_long getid(){return id_;}

	virtual void start();
	virtual void on_start() {}

	void async_connect(const std::string ip,unsigned short port);
	void async_connect(TCPENDPOINT );

	void send_msg(IMessage* msg);

	std::string remote_address();

	void close();

	inline int is_closed()  {return status_.get_bit<CLOSED>();}
	inline int is_reading() {return status_.get_bit<READ_IN_PROGRESS>();}
	inline int is_writing() {return status_.get_bit<WRITE_IN_PROGRESS>();}
protected:
	void* Malloc();
	void Free(void*);
	size_t BufferSize();

	void async_read();
	void async_read(void* buffer);
	void async_read(void* dbuffer,void*basebuffer);
	void async_write(void* buffer,size_t size);
	void remove();
private:
	void* on_msg2buffer(IMessage* msg,size_t& length);
	bool dispatch(void* buff,size_t size);
	void handle_connect(const boost::system::error_code& error);
	virtual void handle_read(void* buffer,const boost::system::error_code& error,std::size_t bytes_transferred,void* p = NULL);
	void handle_write(char* buffer,const boost::system::error_code& error,std::size_t bytes_transferred);
	void progress_error(const boost::system::error_code& error);
private:
	// The socket used to communicate with the client.
	boost::asio::ip::tcp::socket socket_;

	CSessionManager& session_manager_;

	u_long id_;

	CBitSysType<int> status_;

	
};

typedef boost::shared_ptr<CSession> session_ptr;

}}


#endif	//__SHARE_LIB_CONNECTION_H__
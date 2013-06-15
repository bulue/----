#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <boost\noncopyable.hpp>
#include "global_types.h"
#include "typestream.h"
#include <boost\shared_ptr.hpp>

namespace ShareLib{namespace Data{

#define TARGETINVAILD -1
	struct NullTarget{
		static const u_long Type=TARGETINVAILD;
	};

	inline CTypeStream& operator << (CTypeStream &dataStream, const NullTarget& value)
	{
		return dataStream;
	}
	inline CTypeStream& operator >> (CTypeStream& dataStream, NullTarget& value)
	{
		return dataStream;
	}

	class CTypeStreamExt:public CTypeStream{
		std::vector<char> vec_buffer_;
	public:
		CTypeStreamExt(size_t size):vec_buffer_(size)
		{
			_init(vec_buffer_.data(),size,0);
		}
		~CTypeStreamExt(){}
	};
	typedef boost::shared_ptr<CTypeStreamExt> CTypeStreamExtPtr;

	class IMessage:private boost::noncopyable{

	protected:
		struct IMessage_Base_Info{
			u_long	id;
		};
		IMessage_Base_Info base_info_;

		virtual void to_buffer(CTypeStream & data) const=0;
		//virtual void to_buffer(CEmptyStream & data) const=0;

		virtual void from_buffer(CTypeStream & data)=0;

		inline IMessage_Base_Info const& base_info() const {return base_info_;}
		inline IMessage_Base_Info & base_info() {return base_info_;}
	public:
		virtual u_long targetType()const=0;
		virtual void* target()const=0;
	public:

		size_t to_buffer(void* buffer,size_t size)const{
//#if _DEBUG
//			CEmptyStream data;
//			to_buffer(data);
//			ASSERT(data.size()<size,"to_buffer overflow");
//#endif
			CTypeStream stream(buffer,size,0);
			to_buffer(stream);
			return stream.data_size();// .size();
		}
		inline u_long id()const{return base_info_.id;};
		inline void id(u_long p_id){base_info_.id=p_id;}

		friend CTypeStream& operator >> (CTypeStream & data,IMessage::IMessage_Base_Info& base_info);
		friend CTypeStream& operator << (CTypeStream & data,IMessage::IMessage_Base_Info const& base_info);

		friend CTypeStream& operator >> (CTypeStream & data,IMessage& msg);
		friend CTypeStream& operator << (CTypeStream & data, IMessage const & msg);


		//friend CEmptyStream& operator >> (CEmptyStream & data,IMessage::IMessage_Base_Info& base_info);
		//friend CEmptyStream& operator << (CEmptyStream & data,IMessage::IMessage_Base_Info const& base_info);

		//friend CEmptyStream& operator >> (CEmptyStream & data,IMessage& msg);
		//friend CEmptyStream& operator << (CEmptyStream & data, IMessage const & msg);
	};
	CTypeStream& operator >> (CTypeStream & data,IMessage::IMessage_Base_Info& base_info);
	CTypeStream& operator << (CTypeStream & data,IMessage::IMessage_Base_Info const& base_info);

	CTypeStream& operator >> (CTypeStream & data,IMessage& msg);
	CTypeStream& operator << (CTypeStream & data,IMessage const & msg);

	//CEmptyStream& operator >> (CEmptyStream & data,IMessage::IMessage_Base_Info& base_info);
	//CEmptyStream& operator << (CEmptyStream & data,IMessage::IMessage_Base_Info const& base_info);

	//CEmptyStream& operator >> (CEmptyStream & data,IMessage& msg);
	//CEmptyStream& operator << (CEmptyStream & data,IMessage const & msg);



	template<typename T,typename TargetName=NullTarget>
	class CMessage:public IMessage
	{
	protected:
		void from_buffer(CTypeStream &data){
			data>>*this;
		};
		void to_buffer(CTypeStream &data) const{
			data<<*this;
		};
		//void to_buffer(CEmptyStream &data) const{
		//	data<<*this;
		//};
		T msg_;
		TargetName target_;
		virtual void* target()const{return &Target();}
	public:
		virtual u_long targetType()const{return target_.Type;}
	public:
		inline CMessage(){id(T::MSGID);}
		inline CMessage(CTypeStream &data){
			data>>*this;
		}
		inline typename TargetName& Target()const{return (typename TargetName&)target_;}
		inline typename T& operator *() const{
			return msg_;
		}
		inline typename T* operator ->() const {return (T*)&msg_;}

		inline static CMessage<T,TargetName>* Create(){
			return new CMessage<T,TargetName>();
		}
		inline static CMessage<T,TargetName>* Create(IMessage*& msg){
			CMessage<T,TargetName>* ret= new CMessage<T,TargetName>();
			msg = ret;
			return ret;
		}
		template<typename Y,typename Z>
		friend CTypeStream & operator >> (CTypeStream & data, CMessage<Y,Z>& msg);

		template<typename Y,typename Z>
		friend CTypeStream & operator << (CTypeStream & data, CMessage<Y,Z> const& msg);


		//template<typename Y,typename Z>
		//friend CEmptyStream & operator >> (CEmptyStream & data, CMessage<Y,Z>& msg);

		//template<typename Y,typename Z>
		//friend CEmptyStream & operator << (CEmptyStream & data, CMessage<Y,Z> const& msg);

	};
	template<typename Y,typename Z>CTypeStream& operator >> (CTypeStream & data,CMessage<Y,Z>& msg)
	{
		data>>msg.base_info()
			>>msg.msg_
			>>msg.target_;
		return data;
	}

	template<typename Y,typename Z>CTypeStream& operator << (CTypeStream & data,CMessage<Y,Z> const& msg)
	{
		data<<msg.base_info()
			<<msg.msg_
			<<msg.target_;
		return data;
	}


	struct CMSGFUN {
		private:
		void* exec_fun;//执行函数
		void* fun_obj_point;//对象指针

		boost::tribool (CMSGFUN::* create_exec_)(CTypeStream &,void *) ;

		template<class T>
		boost::tribool _create_exec_msg_obj_fun(CTypeStream &data,void *session){
			T null_msg;
			return _internal_exec_msg_obj_fun(null_msg,data,session);
		}

		u_long _msg_obj_fun(void*,IMessage& ,IMessage*& );
		boost::tribool _internal_exec_msg_obj_fun(IMessage& imsg,CTypeStream& dataStream ,void *session);
	public:
		//调用成员函数
		template<class Tx,class Ty,class Tz,class Tu>
		CMSGFUN(Ty* obj,u_long (Ty::* exec)(Tx&,const CMessage<Tz,Tu> &)):fun_obj_point(obj)
		{
			*(void**)&exec_fun  =*(void**)&exec;
			create_exec_ = &CMSGFUN::_create_exec_msg_obj_fun<CMessage<Tz,Tu>>;
		}

		CMSGFUN(){printf("Error:CMSGFUN()\n");}

	public:
		boost::tribool create_exec(CTypeStream &data,void *session){
			return (this->*create_exec_)(data,session);
		}	
	};


	u_long hack_invoke_2(void* obj,void*fun,void* p1,void *p2);

}}

#endif //__MESSAGE_H__


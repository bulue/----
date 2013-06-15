#include "Message.h"

namespace ShareLib{namespace Data{
	CTypeStream& operator>>( CTypeStream & dataStream,IMessage::IMessage_Base_Info& base_info )
	{
		dataStream >> base_info.id;
		return dataStream;
	}

	CTypeStream& operator<<( CTypeStream & dataStream,IMessage::IMessage_Base_Info const& base_info )
	{
		dataStream << base_info.id;
		return dataStream;
	}

	CTypeStream& operator<<( CTypeStream & dataStream,IMessage const & msg )
	{
		msg.to_buffer(dataStream);
		return dataStream;
	}

	CTypeStream& operator>>( CTypeStream & dataStream,IMessage& msg )
	{
		msg.from_buffer(dataStream);
		return dataStream;
	}

	u_long hack_invoke_2(void* obj,void*fun,void* p1,void *p2)
	{
		class T{};
		typedef u_long (T::*TFUN)(double&,double&);
		return ((T *)obj->*(*(TFUN*)&fun))(*(double*)p1,*(double*)p2);
	}


	boost::tribool CMSGFUN::_internal_exec_msg_obj_fun( IMessage& imsg,CTypeStream& dataStream ,void *session )
	{
		dataStream >> imsg;
		hack_invoke_2(fun_obj_point,exec_fun,session,&imsg);

		return true;
	}

}}
#ifndef TYPESTREAM_H
#define TYPESTREAM_H

#include <boost\noncopyable.hpp>
#include <list>
#include <vector>
#include <boost/array.hpp>
#include "global_types.h"


namespace ShareLib{namespace Data {
	class CTypeStream;

	CTypeStream& operator << (CTypeStream &dataStream, const std::string& str_value);
	CTypeStream& operator >> (CTypeStream &dataStream, std::string& str_value);
	CTypeStream& operator << (CTypeStream &dataStream, const double value);
	CTypeStream& operator >> (CTypeStream &dataStream, double& value);
	CTypeStream& operator << (CTypeStream &dataStream, const float value);
	CTypeStream& operator >> (CTypeStream &dataStream, float& value);
	//CTypeStream& operator << (CTypeStream &dataStream, const u_long value);
	CTypeStream& operator >> (CTypeStream &dataStream, u_long& value);
	CTypeStream& operator << (CTypeStream &dataStream, const long value);
	CTypeStream& operator >> (CTypeStream &dataStream, long& value);
	CTypeStream& operator << (CTypeStream &dataStream, const u_int value);
	CTypeStream& operator >> (CTypeStream &dataStream, u_int& value);
	CTypeStream& operator << (CTypeStream &dataStream, const int value);
	CTypeStream& operator >> (CTypeStream &dataStream, int& value);
	CTypeStream& operator << (CTypeStream &dataStream, const u_short value);
	CTypeStream& operator >> (CTypeStream &dataStream, u_short& value);
	CTypeStream& operator << (CTypeStream &dataStream, const short value);
	CTypeStream& operator >> (CTypeStream &dataStream, short& value);
	CTypeStream& operator << (CTypeStream &dataStream, const u_char value);
	CTypeStream& operator >> (CTypeStream &dataStream, u_char& value);
	CTypeStream& operator << (CTypeStream &dataStream, const char value);
	CTypeStream& operator >> (CTypeStream &dataStream, char& value);
	CTypeStream& operator << (CTypeStream &dataStream, const bool value);
	CTypeStream& operator >> (CTypeStream &dataStream, bool& value);

	class CTypeStream:private boost::noncopyable{
		unsigned int current_pos_;
		char* buffer_;
		size_t buffer_size_;
		bool out_of_rang_;

		unsigned int current_size_;
	protected:
		CTypeStream():out_of_rang_(false),current_size_(0){};

		void _init(void* buffer,size_t size,size_t data_size){
			current_pos_=0;
			buffer_=(char*)buffer;
			buffer_size_=size;
			current_size_ = data_size;
		}
	private:
		template<typename type> type get_value() throw(.../*std::exception*/)
		{
			return *(type*)get_bytes(sizeof(type));
		}
		template<typename type>void set_value(const type& value) throw(.../*std::exception*/)
		{
			set_bytes(&value,sizeof(value));
		}

	public:
		void* get_bytes(unsigned int len) throw(.../*std::exception*/)
		{
			void* ret = current();
			offset(offset()+len);
			return ret;
		}
		void set_bytes(const void* value,unsigned int len) throw(.../*std::exception*/)
		{
			data_size(data_size()+len);
			memcpy(current(),value,len);
			offset(offset()+len);
		}

		inline size_t data_size(){return current_size_;}
		inline void data_size(size_t _size) throw(.../*std::exception*/){
			if(_size>buffer_size())
				throw new std::exception("CTypeStream::data_size over buffer_size");
			current_size_ = _size;
		}

		inline size_t buffer_size(){return buffer_size_;}
		inline bool eof(){return current_pos_>=data_size();}

	public:
		inline explicit CTypeStream(void* buffer,size_t size,size_t datasize)
		{
			_init(buffer,size,datasize);
		}
		template<typename A,template<typename A> class T>
		inline explicit CTypeStream(T<A>& array,size_t datasize)
		{
			typedef T<A>::iterator TTT;
			_init(array.data(),sizeof(A)*array.size(),datasize);
		}

		virtual ~CTypeStream() {}

		//reset the stream, data will be clear.
		inline void reset(){current_pos_=0;out_of_rang_=false;current_size_=0;}

		//return the data pointer of the buffer;
		inline void* data(){return buffer_;}

		//return the current pointer of the buffer;
		inline void* current(){return &buffer_[current_pos_];}

		//return current offset;
		inline size_t offset(){return current_pos_;}

		inline void offset(size_t pos) throw(.../*std::exception*/){
			if(pos>data_size())
				throw new std::exception("CTypeStream::offset over data_size");
			current_pos_ =pos;
		}

		//is out of the range
		inline bool out_of_rang(){return out_of_rang_; }

		//left size of vaild data.
		inline size_t left_data_size(){return data_size()-offset();}

		//bool 
		friend CTypeStream & operator << (CTypeStream &dataStream, const bool value);
		friend CTypeStream & operator >> (CTypeStream &dataStream, bool& value);
		//double 
		friend CTypeStream & operator << (CTypeStream &dataStream, const double value);
		friend CTypeStream & operator >> (CTypeStream &dataStream, double& value);
		//float 
		friend CTypeStream & operator << (CTypeStream &dataStream, const float value);
		friend CTypeStream & operator >> (CTypeStream &dataStream, float& value);
		//u_long 
		friend CTypeStream & operator << (CTypeStream &dataStream, const u_long value);
		friend CTypeStream & operator >> (CTypeStream &dataStream, u_long& value);
		//long 
		friend CTypeStream & operator << (CTypeStream &dataStream, const long value);
		friend CTypeStream & operator >> (CTypeStream &dataStream, long& value);
		//u_int
		friend CTypeStream & operator << (CTypeStream &dataStream, const u_int value);
		friend CTypeStream & operator >> (CTypeStream &dataStream, u_int& value);
		//int
		friend CTypeStream & operator << (CTypeStream &dataStream, const int value);
		friend CTypeStream & operator >> (CTypeStream &dataStream, int& value);
		//u_short
		friend CTypeStream & operator << (CTypeStream &dataStream, const u_short value);
		friend CTypeStream & operator >> (CTypeStream &dataStream, u_short& value);
		//short
		friend CTypeStream & operator << (CTypeStream &dataStream, const short value);
		friend CTypeStream & operator >> (CTypeStream &dataStream, short& value);

		//u_char
		friend CTypeStream & operator << (CTypeStream &dataStream, const u_char value);
		friend CTypeStream & operator >> (CTypeStream &dataStream, u_char& value);
		//char
		friend CTypeStream & operator << (CTypeStream &dataStream, const char value);
		friend CTypeStream & operator >> (CTypeStream &dataStream, char& value);


	};

	template<typename Ty>
	CTypeStream& operator << (CTypeStream &dataStream, const std::list<Ty>& value)
	{
		int &size = *(int*)dataStream.current();
		dataStream<<(int)0;
		int i=0;
		for(std::list<Ty>::const_iterator itor = value.cbegin();itor!=value.cend();itor++,i++)
		{
			dataStream<<*itor; 
		}
		size = i;
		return dataStream;
	}

	template<typename Ty>
	CTypeStream& operator >> (CTypeStream &dataStream, std::list<Ty>& value)
	{
		int size;
		dataStream>>size;
		for (int i=0;i<size;i++)
		{
			Ty tmp;
			dataStream>>tmp;
			value.insert(value.cend(),tmp);
		}
		return dataStream;
	}

	template<typename Ty>
	CTypeStream& operator << (CTypeStream &dataStream, std::vector<Ty>& value)
	{
		dataStream << value.size();
		for (size_t i = 0; i < value.size(); ++i)
		{
			dataStream << value[i];
		}
		return dataStream;
	}
	
	template<typename Ty>
	CTypeStream& operator >> (CTypeStream &dataStream, std::vector<Ty>& value)
	{
		size_t size;
		dataStream >> size;
		value.resize(size);
		for (size_t i = 0; i < size; ++i)
		{
			dataStream >> value[i];
		}

		return dataStream;
	}

	template<typename Ty,std::size_t N>
	CTypeStream& operator >> (CTypeStream &dataStream,boost::array<Ty,N> value)
	{
		for (std::size_t i =0; i < N; ++i)
		{
			dataStream >> value[i];
		}

		return dataStream;
	}

	template<typename Ty,std::size_t N>
	CTypeStream& operator << (CTypeStream &dataStream,boost::array<Ty,N> value)
	{
		for (std::size_t i = 0; i < N; ++i)
		{
			dataStream << value[i];
		}
		return dataStream;
	}
}};
#endif
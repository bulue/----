#ifndef __PACKET_H__
#define __PACKET_H__
#include "../define.h"
#include "../mem_allocate.h"
#include <memory.h>

#pragma pack(push,1)

template <typename _DT,typename _TSIZE = int>
struct stZeroArray {
public:
	_TSIZE size;
private:
	_DT thearray[0];
public:
	stZeroArray():size(0) {}
	_DT& operator [] (_TSIZE index) {
		return thearray[index];
	}
	const _DT& operator [] (_TSIZE index) const{
		return thearray[index];
	}
	__inline _DT* getptr(){
		return (_DT*)(&thearray);
	}
	__inline unsigned int getarraysize() {
		return sizeof(_DT)*size;
	}
	__inline unsigned int getallsize() {
		return sizeof(_DT)*size + sizeof(size);
	}
	__inline void push_back(_DT& t){
		thearray[size] = t;
		size++;
	}
	__inline void push_back(const _DT* pin, int ninlen){
		memcpy(&thearray[size],pin,ninlen*sizeof(_DT));
		size += ninlen;
	}
	__inline void clear() {size = 0;}

	_inline void push_str(const char* str) {
		int nlen = strlen(str);
		memcpy(&thearray[size],str,nlen);
		size += nlen;
	}
};


typedef BYTE CMD_TYPE;
typedef BYTE SUBCMD_TYPE;
typedef WORD CMD_SIZE_TYPE;

#define MAKECMDVALUE(cmd,subcmd)		(CMD_SIZE_TYPE)( (subcmd<<(sizeof(CMD_TYPE)*8)) | cmd )	

template<CMD_TYPE cmdvalue = 0,SUBCMD_TYPE subcmdvalue = 0>
struct stCmdBase {
	enum {
		_cmd=cmdvalue,
	};
	enum {
		_subcmd=subcmdvalue,
	};
	enum {
		_value=MAKECMDVALUE(cmdvalue,subcmdvalue),
	};
	union {
		struct {
			CMD_TYPE cmd;
			SUBCMD_TYPE subcmd;
		};
		CMD_SIZE_TYPE value;
	};
	stCmdBase():cmd(cmdvalue),subcmd(subcmdvalue){
	}
};

typedef stCmdBase<> stBaseCmd;

#define ZEROPCMD(p)			memset((char*)(p)+sizeof(stBaseCmd),0,sizeof(*(p))-sizeof(stBaseCmd))
#define ZEROCMD				ZEROPCMD(this)
#define ZEROSELF			memset(this,0,sizeof(*this))

#define DEFINEUSERCMD(cmdtype,p1,p2) struct cmdtype:public stBaseCmd<p1,p2>{};
#define DEFINEUSERCMD1(cmdtype,p1) \
template<SUBCMD_TYPE subcmdvalue = 0> \
struct cmdtype:public stCmdBase<p1,subcmdvalue>{};

#define BUFFER_CMD(cmd,cmdname,len) char buffer##cmdname[(len)];cmd* cmdname = (cmd*)buffer##cmdname; constructInPlace(cmdname);
#define PTR_CMD(cmd,cmdname,ptr) cmd*cmdname=(cmd*)(ptr);constructInPlace(cmdname);
typedef unsigned long SIZE_TYPE;

//template<typename _CMDTYPE=stBaseCmd >
//struct stPacketBase {
//	__inline unsigned int getpacketsize() {
//		return size+sizeof(*this);
//	}
//	__inline _CMDTYPE* cmd() {
//		return (_CMDTYPE*)((int)this+sizeof(*this));
//	}
//	__inline SIZE_TYPE getcmdsize() {
//		return size;
//	}
//	//__inline void setcmdsize(SIZE_TYPE rawsize) {
//	//	size = rawsize;
//	//}
//	
//	__inline bool isfullcmd(unsigned int nlen) {
//		return getcmdsize() <= nlen;
//	}
//	__inline void fullcmd(char* pin,unsigned int nlen){
//		memcpy(cmd()+getcmdsize(),pin,nlen);
//		size += nlen;
//	}
//	stPacketBase():size(0){}
//private:
//	SIZE_TYPE size;
//};

//typedef stPacketBase<> stBasePacket;

struct stBaseMessage{
	enum {
		_MAX_MSG_SIZE = (6*1024),
	};
	//unsigned int type;
	unsigned int size;

	__inline char* head(){
		return (char*)this;
	}
	__inline static unsigned int headlen() {
		return sizeof(stBaseMessage);
	}

	__inline char* body() {
		return ((char*)this)+sizeof(*this);
	}
	unsigned int bodylen() {
		return size;
	}

	__inline void pushbuffer(const char* pin,unsigned int ninlen){
		memcpy(body()+bodylen(),pin,ninlen);
		size += ninlen;
	}

	unsigned int alllen() {
		return size + sizeof(*this);
	}
	stBaseMessage() {
		memset(this,0,sizeof(*this));
	}

	stBaseMessage* clone() {
		void *bmsg = SF_MALLOC(alllen());
		memcpy(bmsg,this,alllen());
		return (stBaseMessage*)bmsg;
	}
};

struct stProxyMessage {
	unsigned int npacketsize;
	int iproxid;

	int bufferlen() {
		return npacketsize + sizeof(iproxid) - sizeof(stProxyMessage);
	}
	char* buffer() {
		return (char*)this + sizeof(stProxyMessage);
	}
	int& proxid(){
		return iproxid;
	}
	void pushbuffer(char* pin,unsigned int ninlen) {
		memcpy(buffer()+bufferlen(),pin,ninlen);
		npacketsize += ninlen;
	}

	int getallsize() {
		return npacketsize + sizeof(npacketsize);
	}

	stProxyMessage() {
		memset(this,0,sizeof(*this));
		npacketsize = sizeof(iproxid);
	}
};


#pragma pack(pop)


#endif
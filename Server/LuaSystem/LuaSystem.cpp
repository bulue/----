#include <boost/cregex.hpp> 

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <luabind/luabind.hpp>

namespace
{
	bool match(boost::RegEx& r, const char* s)
	{
		return r.Match(s);
	}

	bool search(boost::RegEx& r, const char* s)
	{
		return r.Search(s);
	}
} // namespace unnamed


void wrap_regex(lua_State* L)
{
	using boost::RegEx;
	using namespace luabind;

	module(L) 
		[
			class_<RegEx>("regex")
			.def(constructor<const char*>())
			.def(constructor<const char*, bool>())
			.def("match", match)
			.def("search", search)
			.def("what", &RegEx::What)
			.def("matched", &RegEx::Matched)
			.def("length", &RegEx::Length)
			.def("position", &RegEx::Position)
		];
}

#ifndef LUABIND_NO_EXCEPTIONS 

#define LUABIND_TRY_ERRORMSG(s,L)		const char* s=NULL; try{	__noop;

#define LUA_GET_ERRORMSG(s,L)			s=(const char*)lua_tostring(L, -1);if(s==NULL){s="";};lua_pop(L, 1);
#define LUABIND_LOG_CALL_ERROR(cs,fns,es,s)		printf( "(%s)Luabind::callfunc: %s () -> err: %s ( %s )\n",cs,fns,es,s);

#define LUABIND_CATCH_ERRORMSG(s,L,fns)		}catch (luabind::cast_failed& e){ LUA_GET_ERRORMSG(s,L);LUABIND_LOG_CALL_ERROR("cast_failed",fns,e.what(),s) }	\
	catch (luabind::error& e){ LUA_GET_ERRORMSG(s,L);LUABIND_LOG_CALL_ERROR("error",fns,e.what(),s) }		\
	catch (const char* e){ LUA_GET_ERRORMSG(s,L);LUABIND_LOG_CALL_ERROR("char*",fns,e,s) }		\
	catch (...){ LUA_GET_ERRORMSG(s,L);LUABIND_LOG_CALL_ERROR("...",fns,"",s) }		\

#else

#define LUABIND_TRY_ERRORMSG(s,L)		    const char* s=NULL; try{	__noop;
#define LUA_GET_ERRORMSG(s,L)              
#define LUABIND_LOG_CALL_ERROR(cs,fns,es,s) 
#define LUABIND_CATCH_ERRORMSG(s,L,fns) }catch (...){ }

#endif

class CLuaVM{
protected:
	lua_State *m_luaState;
	int m_ErrFn;
	int m_nParseStatus;
//private:
//	static DWORD dwThreadsId;
public:
	CLuaVM(bool bOpenStdLib = true){m_luaState = lua_open();if (bOpenStdLib) OpenLibs();}
	virtual ~CLuaVM() {}
	void OpenLibs() {luaL_openlibs(m_luaState);luabind::open(m_luaState);}
	__inline lua_State* GetHandle(){	return m_luaState;}
	__inline lua_State* lua() const { return m_luaState; }
	__inline int ErrorFn(){	return m_ErrFn; }

	virtual bool LoadFileToBuffer(lua_State *L, const char *filename,char* szbuffer,int &maxlen,bool& loadlocal) {return true;}

	bool DoFile(const char* filename) {
		int error = luaL_dofile(m_luaState,filename); if (error){
		fprintf(stderr, "%s", lua_tostring(m_luaState, -1)); 
		lua_pop(m_luaState, 1);/* pop error message from the stack */ 
		return false;
		}
		return true;
	}
	bool DoString(const char* buffer) {
		int error = luaL_dostring(m_luaState,buffer); if (error){
			fprintf(stderr, "%s", lua_tostring(m_luaState, -1)); 
			lua_pop(m_luaState, 1);/* pop error message from the stack */ 
			return false;
		}
		return true;
	}
	bool DoBuffer(const char* buffer, size_t size) {}


	bool IsExistFunction(const char* name){
		lua_getglobal(m_luaState, name);
		if(lua_type(m_luaState,-1) == LUA_TFUNCTION){
			lua_pop(m_luaState,1);
			return true;
		}else{
			lua_pop(m_luaState,1);
			//g_logger.error("LuaScript Error: No find function %s",name);
			return false;
		}
	}

	template<typename R>
	R Call(const char* funcname,R defVal)
	{
		//FUNCTION_MONITOR(48,funcname);
		if(IsExistFunction(funcname)){
			LUABIND_TRY_ERRORMSG(errmsg,m_luaState);
			return luabind::call_function<R>(m_luaState,funcname);
			LUABIND_CATCH_ERRORMSG(errmsg,m_luaState,funcname);
		}
		return defVal;
	}
	template<typename R, typename P1>
	R Call(const char* funcname,R defVal, P1 p1)
	{
		//FUNCTION_MONITOR(48,funcname);
		if(IsExistFunction(funcname)){
			LUABIND_TRY_ERRORMSG(errmsg,m_luaState);
			return luabind::call_function<R>(m_luaState,funcname,p1);
			LUABIND_CATCH_ERRORMSG(errmsg,m_luaState,funcname);
		}
		return defVal;
	}
	template<typename R, typename P1, typename P2>
	R Call(const char* funcname,R defVal, P1 p1, P2 p2)
	{
		//FUNCTION_MONITOR(48,funcname);
		if(IsExistFunction(funcname)){
			LUABIND_TRY_ERRORMSG(errmsg,m_luaState);
			return luabind::call_function<R>(m_luaState,funcname,p1,p2);
			LUABIND_CATCH_ERRORMSG(errmsg,m_luaState,funcname);
		}
		return defVal;
	}
};

int main()
{
	CLuaVM *luavm = NULL;
	CLuaVM *luavm2 = NULL;
	luavm2 = new CLuaVM();
	luavm2->DoFile("regex.lua");
	do 
	{
		luavm = new CLuaVM();
		wrap_regex(luavm->lua());

		luavm->DoFile("regex.lua");

		printf("输入重新加载luavm的命令:\n");
		char gmcmd[1024];
		scanf("%s",gmcmd);
		if (strncmp(gmcmd,"exit",strlen("exit")) == 0) {
			break;
		}else {
			luavm->DoString(gmcmd);

			if (luavm2) {
				luabind::object tbl;
				luavm->Call<bool>("GetScriptGlobalSaveData",false,tbl);
				luavm2->Call<bool>("LoadScriptData",false,tbl);
			}
		}
	} while (true);

	return 0;
}
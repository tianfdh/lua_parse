// luatest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <string>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#ifdef _DEBUG
#pragma comment(lib, "lua5d.lib")
#else
#pragma comment(lib, "lua5.1.lib")
#endif // _DEBUG

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define err_exit(num,fmt,args)  \
do{printf("[%s:%d]"fmt"/n",__FILE__,__LINE__,##args);exit(num);} while(0)
#define err_return(num,fmt,args)  \
do{printf("[%s:%d]"fmt"/n",__FILE__,__LINE__,##args);return(num);} while(0)

//lua�е��õ�c��������,ʵ�ּӷ�
int csum(lua_State* l)
{
	int a = lua_tointeger(l,1) ;
	int b = lua_tointeger(l,2) ;
	lua_pushinteger(l,a+b) ;
	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	lua_State * l = luaL_newstate() ;        //����lua���л���
	if ( l == NULL ) err_return(-1,"luaL_newstat() failed",1); 
	luaL_openlibs(l);

	int ret = 0 ;
	ret = luaL_loadfile(l,"func.lua") ;      //����lua�ű��ļ�
	if ( ret != 0 ) err_return(-1,"luaL_loadfile failed",1) ;
	ret = lua_pcall(l,0,0,0) ;
	if ( ret != 0 ) err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;

	lua_getglobal(l,"width");              //��ȡlua�ж���ı���
	lua_getglobal(l,"height");
	printf("height:%ld width:%ld\n",lua_tointeger(l,-1),lua_tointeger(l,-2)) ;
	lua_pop(l,2) ;                        //�ָ�lua��ջ

	int a = 11 ;
	int b = 12 ;
	lua_getglobal(l,"sum");               //����lua�еĺ���sum
	lua_pushinteger(l,a) ;
	lua_pushinteger(l,b) ;
	ret = lua_pcall(l,2,1,0) ;
	if ( ret != 0 ) err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;
	printf("sum:%d + %d = %ld\n",a,b,lua_tointeger(l,-1)) ;
	lua_pop(l,1) ;

	const char str1[] = "hello" ;
	const char str2[] = "world" ;
	lua_getglobal(l,"mystrcat");          //����lua�еĺ���mystrcat
	lua_pushstring(l,str1) ;
	lua_pushstring(l,str2) ;
	ret = lua_pcall(l,2,1,0) ;
	if ( ret != 0 ) err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;
	printf("mystrcat:%s%s = %s\n",str1,str2,lua_tostring(l,-1)) ;
	lua_pop(l,1) ;

	lua_pushcfunction(l,csum) ;         //ע����lua��ʹ�õ�c����
	lua_setglobal(l,"csum") ;           //�󶨵�lua�е�����csum

	lua_getglobal(l,"mysum");           //����lua�е�mysum�������ú������ñ������ж����csum����ʵ�ּӷ�
	lua_pushinteger(l,a) ;
	lua_pushinteger(l,b) ;
	ret = lua_pcall(l,2,1,0) ;
	if ( ret != 0 ) err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;
	printf("mysum:%d + %d = %ld\n",a,b,lua_tointeger(l,-1)) ;
	lua_pop(l,1) ;

	int error;
	char str[100];
	while (fgets(str, 100, stdin))
	{
		int nTop = lua_gettop(l);
		error = luaL_loadbuffer(l, str, strlen(str), "") || lua_pcall(l, 0, 0, 0);

		if (error) {

			fprintf(stderr, "%s\n", lua_tostring(l, -1));

			lua_pop(l, 1);/* pop error message from the stack */

		}

		lua_settop(l, nTop);
	}

	lua_close(l) ;                     //�ͷ�lua���л���

	system("pause");
	return 0;
}


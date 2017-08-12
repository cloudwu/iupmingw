#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stdio.h>
#include <stdlib.h>

int luaopen_iuplua(lua_State* L);

static void
l_message (const char *pname, const char *msg) {
	if (pname) lua_writestringerror("%s: ", pname);
	lua_writestringerror("%s\n", msg);
}

static int 
report (lua_State *L, int status, const char *progname) {
	if (status != LUA_OK) {
		const char *msg = lua_tostring(L, -1);
		l_message(progname, msg);
		lua_pop(L, 1);  /* remove message */
	}
	return status;
}

static int
msghandler (lua_State *L) {
	const char *msg = lua_tostring(L, 1);
	if (msg == NULL) {  /* is error object not a string? */
		if (luaL_callmeta(L, 1, "__tostring") &&  /* does it have a metamethod */
			lua_type(L, -1) == LUA_TSTRING)  /* that produces a string? */
			return 1;  /* that is the message */
		else
			msg = lua_pushfstring(L, "(error object is a %s value)",
								   luaL_typename(L, 1));
	}
	luaL_traceback(L, L, msg, 1);  /* append a standard traceback */
	return 1;  /* return the traceback */
}

int iuplua_open(lua_State * L);

static int
dummy_iuplua(lua_State *L) {
	lua_getglobal(L, "iup");
	return 1;
}

static int
pmain (lua_State *L) {
	int i;
	int argc = (int)lua_tointeger(L, 1);
	char **argv = (char **)lua_touserdata(L, 2);
	if (argc < 2) {
		return luaL_error(L, "Need filename");
	}
	luaL_openlibs(L);
	iuplua_open(L);
	luaL_requiref(L, "iuplua", dummy_iuplua, 0);
	lua_settop(L, 0);
	lua_pushcfunction(L, msghandler);
	if (luaL_loadfile(L, argv[1]) != LUA_OK) {
		return lua_error(L);
	}
	for (i=2;i<argc;i++) {
		lua_pushstring(L, argv[i]);
	}
	if (lua_pcall(L, argc - 2, 0, 1) != LUA_OK) {
		return lua_error(L);
	}
	return 0;
}

int
main (int argc, char **argv) {
	int status, result;

	lua_State *L = luaL_newstate();  /* create state */
	if (L == NULL) {
		l_message(argv[0], "cannot create state: not enough memory");
		return EXIT_FAILURE;
	}
	lua_pushcfunction(L, &pmain);  /* to call 'pmain' in protected mode */
	lua_pushinteger(L, argc);  /* 1st argument */
	lua_pushlightuserdata(L, argv); /* 2nd argument */
	status = lua_pcall(L, 2, 1, 0);  /* do the call */
	result = lua_toboolean(L, -1);  /* get result */
	report(L, status, argv[0]);
	lua_close(L);
	return (result && status == LUA_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
}

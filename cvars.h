#pragma once
#include "main.h"

#define nc(cvar_name) cvar_t *cvar_name;
#define rnc(cvar_name, con_name, value) {cvar.cvar_name = g_Engine.pfnRegisterVariable(con_name, value, 0);}
#define rcmd(cmd_name, function) {g_Engine.pfnAddCommand(cmd_name, function);}

class cvars_s
{
public:
	nc(bhop_scroll_start)
		nc(bhop_scroll_count)
		nc(bhop_standup)
		nc(bhop_standup_start)

		nc(gstrafe_scroll_count)
		nc(gstrafe_scroll_start)
		nc(gstrafe_standup)
		nc(gstrafe_standup_start)

		nc(speed)
};
extern cvars_s cvar;

class cfunc_s
{
public:
	static void Init();
	static bool Bhop;
	static void BhopOn();
	static void BhopOff();

	static bool Gstrafe;
	static void GstrafeOn();
	static void GstrafeOff();
};
extern cfunc_s cfunc;
#pragma once
#include "main.h"

#define nc(cvar_name) cvar_t *cvar_name;
#define rnc(cvar_name, con_name, value) {cvar.cvar_name = g_Engine.pfnRegisterVariable(con_name, value, 0);}
#define rcmd(cmd_name, function) {g_Engine.pfnAddCommand(cmd_name, function);}

class cvars_s
{
public:

};
extern cvars_s cvar;

class cfunc_s
{
public:
	static void Init();
};
extern cfunc_s cfunc;
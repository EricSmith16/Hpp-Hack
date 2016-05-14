#pragma once
#include "main.h"
#include "calculations.h"

#define nc(cvar_name) cvar_t *cvar_name;
#define rnc(cvar_name, con_name, value) {cvar.cvar_name = g_Engine.pfnRegisterVariable(con_name, value, 0);}
#define rcmd(cmd_name, function) {g_Engine.pfnAddCommand(cmd_name, function);}
#define cc(old_name, new_name, value) {g_Calculation.CopyCvar(old_name, new_name, value, 0);}

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
		nc(gstrafe_slowdown)
		nc(gstrafe_slowdown_angle)

		nc(speed)
		nc(speed_engine)

		nc(fastrun_nsd)

		nc(jumpbug_auto)
		nc(jumpbug_slow)
		nc(jumpbug_slow_speed)

		nc(eb_dist)
		nc(eb_ddist)
		nc(eb_speed)
		nc(eb_calc)

		nc(wb_duck)
		nc(wb_ang)
		nc(wb_move)

		nc(show)
		nc(show_color)
		nc(show_time)
		nc(show_fps)
		nc(show_copyright)
		nc(show_keys)
		nc(show_info_color)
		nc(show_info_kz)
		nc(show_info_moves)
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

	static bool FastRun;
	static void FastRunOn();
	static void FastRunOff();
	static void FastRunSwitch();

	static bool JumpBug;
	static void JumpBugOn();
	static void JumpBugOff();

	static bool EdgeBug;
	static void EdgeBugOn();
	static void EdgeBugOff();

	static bool WallBug;
	static void WallBugOn();
	static void WallBugOff();

	static bool forwardm;
	static bool moveleft;
	static bool moveright;
	static bool back;
	static bool duck;
	static bool jump;

	static void p_for();
	static void m_for();
	static void p_ml();
	static void m_ml();
	static void p_mr();
	static void m_mr();
	static void p_bk();
	static void m_bk();
	static void p_dk();
	static void m_dk();
	static void p_jp();
	static void m_jp();
};
extern cfunc_s cfunc;
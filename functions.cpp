#include "functions.h"
#include "cvars.h"
#include "calculations.h"

function_s g_Function;

void function_s::BunnyHop(float frametime, usercmd_s *cmd)
{
	int frame[2] = {}, count[2] = {};
	g_Calculation.ScanFromCvar(cvar.bhop_scroll_start->string, frame);
	g_Calculation.ScanFromCvar(cvar.bhop_scroll_count->string, count);
	static int state, counts;
	float frames = abs((g_Local.Height / g_Local.flFallSpeed * 2) / frametime);
	float frame_standup = g_Local.flFallSpeed * frametime * 10;
	if (counts > 0)
	{
		if (state == 1)
		{
			cmd->buttons &= ~IN_JUMP;
			state = 2;
			counts--;
			return;
		}
		if (state == 2) 
		{ 
			cmd->buttons |= IN_JUMP;
			state = 1; 
			return; 
		}
	}
	else if (state != 0 && g_Local.flFallSpeed <= 0) { state = 0; counts = 0; }
	if (cfunc.Bhop && g_Local.bAlive)
	{
		if (cvar.bhop_standup->value && frame_standup >= cvar.bhop_standup_start->value && g_Local.flGroundAngle <= 25) cmd->buttons |= IN_DUCK;
		if ((g_Local.iFlags&FL_ONGROUND || frames <= g_Engine.pfnRandomFloat(frame[0], frame[1])) && state == 0 && counts == 0)
		{
			cmd->buttons |= IN_JUMP;
			state = 1;
			counts = g_Engine.pfnRandomLong(count[0], count[1]);
		}
	}
}

void function_s::CL_CreateMove(float frametime, usercmd_s *cmd, int active)
{
	cl_entity_s *LocalEnt = g_Engine.GetLocalPlayer();
	g_Local.bAlive = LocalEnt && !(LocalEnt->curstate.effects & EF_NODRAW) && LocalEnt->player && LocalEnt->curstate.movetype != 6 && LocalEnt->curstate.movetype != 0;
	BunnyHop(frametime, cmd);
}
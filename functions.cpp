#include "functions.h"
#include "cvars.h"
#include "calculations.h"
#include "bugs.h"

function_s g_Function;

double *g_Net;

void function_s::AdjustSpeed(double x)
{
	if (g_Local.dwSpeedptr == 0) g_Local.dwSpeedptr = g_Local.SpeedPtr;
	static double LastSpeed = 1;
	if (x != LastSpeed)
	{
		*(double*)g_Local.dwSpeedptr = (x * 1000);
		LastSpeed = x;
	}
}

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
	else if (state != 0) { state = 0; counts = 0; }
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

void function_s::GroundStrafe(float frametime, struct usercmd_s *cmd)
{
	int frame[2] = {}, count[2] = {};
	g_Calculation.ScanFromCvar(cvar.gstrafe_scroll_count->string, count);
	g_Calculation.ScanFromCvar(cvar.gstrafe_scroll_start->string, frame);
	static int state, counts;
	float frames = abs((g_Local.Height / g_Local.flFallSpeed * 2) / frametime);
	float frame_standup = g_Local.flFallSpeed * frametime * 10;
	if (counts > 0)
	{
		if (state == 1)
		{
			cmd->buttons &= ~IN_DUCK;
			if (g_Local.flGroundAngle < cvar.gstrafe_slowdown_angle->value && (g_Local.iFlags & FL_ONGROUND)) { AdjustSpeed(cvar.gstrafe_slowdown->value <= 0 ? 0.000001 : cvar.gstrafe_slowdown->value); }
			state = 2;
			counts--;
			return;
		}
		if (state == 2)
		{
			if (g_Local.flGroundAngle < cvar.gstrafe_slowdown_angle->value && (g_Local.iFlags & FL_ONGROUND)) { AdjustSpeed(cvar.gstrafe_slowdown->value <= 0 ? 0.000001 : cvar.gstrafe_slowdown->value); }
			cmd->buttons |= IN_DUCK;
			state = 1;
			return;
		}
	}
	else if (state != 0) { state = 0; counts = 0; }
	if (cfunc.Gstrafe && g_Local.bAlive)
	{
		if (cvar.gstrafe_standup->value && frame_standup >= cvar.gstrafe_standup_start->value) cmd->buttons |= IN_DUCK;
		if ((g_Local.iFlags&FL_ONGROUND || frames <= g_Engine.pfnRandomFloat(frame[0], frame[1])) && state == 0 && counts == 0)
		{
			if (g_Local.flGroundAngle < cvar.gstrafe_slowdown_angle->value && (g_Local.iFlags & FL_ONGROUND)) { AdjustSpeed(cvar.gstrafe_slowdown->value <= 0 ? 0.000001 : cvar.gstrafe_slowdown->value); }
			cmd->buttons |= IN_DUCK;
			state = 1;
			counts = g_Engine.pfnRandomLong(count[0], count[1]);
		}
	}
}

void function_s::FastRun(struct usercmd_s *cmd)
{
	if (cvar.fastrun_nsd->value)
	{
		if (cfunc.FastRun && g_Local.iFlags&FL_ONGROUND)
		{
			if (!(cmd->buttons&(IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT))) return;
			if (cmd->buttons&IN_MOVELEFT&&cmd->buttons&IN_MOVERIGHT) { cmd->buttons &= ~(IN_MOVELEFT | IN_MOVERIGHT); }
			if (cmd->buttons&IN_FORWARD&&cmd->buttons&IN_BACK) { cmd->buttons &= ~(IN_FORWARD | IN_BACK); }
			float aaddtova;
			if (cmd->buttons&IN_FORWARD)
			{
				if (cmd->buttons&IN_MOVELEFT) aaddtova = 45.0f;
				else if (cmd->buttons&IN_MOVERIGHT) aaddtova = -45.0f;
				else aaddtova = 0.0f;
			}
			else if (cmd->buttons&IN_BACK)
			{
				if (cmd->buttons&IN_MOVELEFT) aaddtova = 135.0f;
				else if (cmd->buttons&IN_MOVERIGHT) aaddtova = -135.0f;
				else aaddtova = 180.0f;
			}
			else if (cmd->buttons&IN_MOVELEFT) aaddtova = 90.0f;
			else if (cmd->buttons&IN_MOVERIGHT) aaddtova = -90.0f;
			if (g_Local.flXYspeed < 78)
			{
				cmd->forwardmove = 900;
				cmd->sidemove = 0;
				return;
			}
			bool dir_rig = true;
			float yaw = atan2(g_Local.vVelocity.y, g_Local.vVelocity.x) * 180.0 / M_PI;
			yaw -= aaddtova;
			yaw = cmd->viewangles.y - yaw;
			int fadif = yaw;
			fadif = (fadif + 360) % 360;
			dir_rig = fadif <= 180;
			float x = 138.06374825035877480180939653038 * (dir_rig ? -1 : +1);
			float y = 267.87643402987823669025530311859;
			float a = DegToRad(aaddtova);
			float sinA = sin(a);
			float cosA = cos(a);
			cmd->sidemove = x*cosA - y*sinA;
			cmd->forwardmove = y*cosA + x*sinA;
			x = cmd->sidemove;
			y = cmd->forwardmove;
			float newa = DegToRad(-yaw);
			sinA = sin(newa);
			cosA = cos(newa);
			cmd->sidemove = x*cosA - y*sinA;
			cmd->forwardmove = y*cosA + x*sinA;
		}
	}
	else
	{
		if (cfunc.FastRun && g_Local.iFlags&FL_ONGROUND)
		{
			static bool _FastRun = false;
			if ((cmd->buttons&IN_FORWARD && cmd->buttons&IN_MOVELEFT) || (cmd->buttons&IN_BACK && cmd->buttons&IN_MOVERIGHT)) 
			{
				if (_FastRun) { _FastRun = false; cmd->sidemove -= 89.6; cmd->forwardmove -= 89.6; }
				else { _FastRun = true;  cmd->sidemove += 89.6; cmd->forwardmove += 89.6; }
			}
			else if ((cmd->buttons&IN_FORWARD && cmd->buttons&IN_MOVERIGHT) || (cmd->buttons&IN_BACK && cmd->buttons&IN_MOVELEFT)) 
			{
				if (_FastRun) { _FastRun = false; cmd->sidemove -= 89.6; cmd->forwardmove += 89.6; }
				else { _FastRun = true;  cmd->sidemove += 89.6; cmd->forwardmove -= 89.6; }
			}
			else if (cmd->buttons&IN_FORWARD || cmd->buttons&IN_BACK) 
			{
				if (_FastRun) { _FastRun = false; cmd->sidemove -= 126.6; }
				else { _FastRun = true;  cmd->sidemove += 126.6; }
			}
			else if (cmd->buttons&IN_MOVELEFT || cmd->buttons&IN_MOVERIGHT) 
			{
				if (_FastRun) { _FastRun = false; cmd->forwardmove -= 126.6; }
				else { _FastRun = true;  cmd->forwardmove += 126.6; }
			}
		}
	}
}

void function_s::Skipframes(int value)
{
	static int cntavoidf = 0;
	if (value % 2 != 0) value += 1;
	if (value != 0)
	{
		if (cntavoidf-- <= 0)
		{
			cntavoidf = value;
			g_Engine.Cvar_SetValue("r_norefresh", 0);
		}
		else g_Engine.Cvar_SetValue("r_norefresh", 1);
	}
	else if (cntavoidf != -7)
	{
		g_Engine.Cvar_SetValue("r_norefresh", 0);
		cntavoidf = -7;
	}
}

void function_s::CL_CreateMove(float frametime, usercmd_s *cmd, int active)
{
	AdjustSpeed(cvar.speed->value);
	if (cvar.speed_engine->value != 0) *g_Net += cvar.speed_engine->value / 1000;
	if (cvar.fps_helper->value > 0) AdjustSpeed((1 / frametime) / (cvar.fps_helper->value));
	cl_entity_s *LocalEnt = g_Engine.GetLocalPlayer();
	g_Local.bAlive = LocalEnt && !(LocalEnt->curstate.effects & EF_NODRAW) && LocalEnt->player && LocalEnt->curstate.movetype != 6 && LocalEnt->curstate.movetype != 0;
	g_Local.fFps = 1 / frametime;
	if (cvar.show->value && cvar.show_keys->value)
	{
		if (cmd->buttons&IN_FORWARD) { cfunc.p_for(); } else { cfunc.m_for(); }
		if (cmd->buttons&IN_MOVELEFT) { cfunc.p_ml(); } else { cfunc.m_ml(); }
		if (cmd->buttons&IN_MOVERIGHT) { cfunc.p_mr(); } else { cfunc.m_mr(); }
		if (cmd->buttons&IN_BACK) { cfunc.p_bk(); } else { cfunc.m_bk(); }
		if (cmd->buttons&IN_DUCK) { cfunc.p_dk(); } else { cfunc.m_dk(); }
		if (cmd->buttons&IN_JUMP) { cfunc.p_jp(); } else { cfunc.m_jp(); }
	}
	BunnyHop(frametime, cmd);
	GroundStrafe(frametime, cmd);
	FastRun(cmd);
	Skipframes(cvar.fps_skipframes->value);
	g_Bug.CL_CreateMove(frametime, cmd);
	g_Local.flsidemove = cmd->sidemove;
	g_Local.flforwardmove = cmd->forwardmove;
}
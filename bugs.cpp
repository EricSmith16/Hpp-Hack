#include "bugs.h"
#include "cvars.h"
#include "calculations.h"
#include "functions.h"

bug_s g_Bug;

extern double *g_Net;

void bug_s::JumpBug(float frametime, usercmd_s *cmd)
{
	static float NeedOffset;
	static int state;
	bool autojb = false;
	if ((cvar.jumpbug_auto->value != 0 && g_Local.flFallSpeed >= 504.8) && (g_Local.Height - (g_Local.flFallSpeed*frametime / cvar.speed->value * 15) <= 0)) autojb = true;
	if ((autojb || cfunc.JumpBug) && g_Local.flFallSpeed > 0)
	{
		static float last_h;
		bool curveang = false;
		if (g_Local.flGroundAngle == 45.0) NeedOffset = 20.0;
		else NeedOffset = 19.0;
		if (cvar.jumpbug_slow->value)
		{
			if (g_Local.flGroundAngle > 1)
			{
				if (g_Local.flXYspeed > 1 && g_Local.Height <= cvar.jumpbug_slow->value * 4 + (g_Local.flXYspeed / g_Local.flFallSpeed)) curveang = true;
				if (g_Local.flXYspeed > 10 && g_Local.Height <= 504 * frametime * 7) g_Function.AdjustSpeed(cvar.jumpbug_slow_speed->value);
			}
		}	
		float cur_frame_zdist = abs((g_Local.flFallSpeed + (800 * frametime))*frametime);
		cmd->buttons |= IN_DUCK;
		cmd->buttons &= ~IN_JUMP;
		if (cvar.jumpbug_slow->value && curveang) g_Calculation.SlowVerticalVel(0, frametime, cmd);
		switch (state)
		{
		case 1:
			cmd->buttons &= ~IN_DUCK;
			cmd->buttons |= IN_JUMP;
			state = 2;
			break;
		case 2:
			state = 0;
			break;
		default:
			if (abs(g_Local.Height - cur_frame_zdist*1.5) <= 20.0 && cur_frame_zdist > 0.0f)
			{
				float needspd = abs(g_Local.Height - (g_Local.bPlayerjb ? 19.0 : NeedOffset));
				float scale = abs(needspd / cur_frame_zdist);
				g_Function.AdjustSpeed(scale);
				state = 1;
			}
			break;
		}
		last_h = g_Local.Height;
	}
	else state = 0;
}

void bug_s::EdgeBug(float frametime, struct usercmd_s *cmd)
{
	static int state;
	float eb_dist = g_Calculation.EdgeDistance();
	float pspeed = g_Local.flXYspeed == 0 ? 1 : g_Local.flXYspeed;
	float pfallspeed = g_Local.flFallSpeed == 0 ? 1 : g_Local.flFallSpeed;
	float pheight = g_Local.Height == 0 ? 1 : g_Local.Height;
	float one = abs((eb_dist / pspeed) / (eb_dist / pfallspeed)*(eb_dist / pheight));
	float two = (eb_dist / pspeed) + (eb_dist / pfallspeed) - (eb_dist / pheight);
	float lox = two;
	float lo1x = abs((frametime*g_Calculation.EdgeDistance()*g_Local.flFallSpeed) / g_Local.flXYspeed);
	int eb_speed_final = -2, eb_speed_offset = -1;
	g_Local.EdgeOffset = one;
	if (cfunc.EdgeBug && g_Calculation.EdgeDistance() <= cvar.eb_dist->value && g_Local.flFallSpeed>0 && g_Calculation.EdgeDistance() >= 0)
	{
		static float last_h = 0.0f;
		float cur_frame_zdist = abs((g_Local.flFallSpeed + (800 * frametime))*frametime);
		if (g_Calculation.EdgeDistance()>cvar.eb_ddist->value) cmd->buttons |= IN_DUCK;
		switch (state)
		{
		case 1:
			if (g_Calculation.EdgeDistance() <= cvar.eb_ddist->value) cmd->buttons &= ~IN_DUCK;
			*g_Net += (cvar.eb_speed->value + (g_Local.EdgeOffset * eb_speed_offset))*frametime / 10;
			state = 2;
			break;
		case 2:
			if (g_Calculation.EdgeDistance() <= cvar.eb_ddist->value) cmd->buttons &= ~IN_DUCK;
			*g_Net += (g_Local.EdgeOffset*eb_speed_final)*frametime / 10;
			state = 0;
			break;
		default:
			if (abs(g_Local.Height - cur_frame_zdist*1.5) <= 22 && cur_frame_zdist>0.0f)
			{
				float needspd = g_Local.Height - 1.0f;
				float scale = needspd / cur_frame_zdist;
				if (cvar.eb_calc->value == 1) *g_Net += (g_Local.EdgeOffset*eb_speed_final)*frametime / 10;
				else if (cvar.eb_calc->value == 2) *g_Net += (scale*eb_speed_final)*frametime / 10;
				else if (cvar.eb_calc->value == 3) *g_Net += (g_Local.EdgeOffset*eb_speed_final)*(abs(g_Local.Height - cur_frame_zdist*1.5) / (g_Local.flFallSpeed / g_Local.flMaxSpeed))*frametime / 10;
				else if (cvar.eb_calc->value == 4) *g_Net += g_Local.EdgeOffset*scale*eb_speed_final*(lo1x*g_Local.flFallSpeed - (-1 * lo1x))*(g_Local.Height / g_Local.flFallSpeed)*(g_Local.flXYspeed / g_Local.flFallSpeed)*(g_Local.flFallSpeed / (g_Local.Height*g_Local.flXYspeed))*frametime / 10;
				state = 1;
			}
			break;
		}
		last_h = g_Local.Height;
	}
	else state = 0;
}

void bug_s::WallBug(struct usercmd_s *cmd)
{
	static bool lox = false;
	static int moves = 0;
	static int dr = 1;
	if (cfunc.WallBug && g_Local.fOnLadder != 1)
	{
		Vector fw = (g_Local.vForward);
		fw = fw.Normalize();
		VectorMul(fw, 5, fw);
		fw[2] = 0;
		VectorAdd(fw, g_Local.vOrigin, fw);
		pmtrace_s *trace = g_Engine.PM_TraceLine(g_Local.vOrigin, fw, 1, g_Local.iUseHull, -1);
		float myang[3] = {};
		g_Engine.GetViewAngles(myang);
		if (trace->fraction < 1.0)
		{
			if (trace->plane.normal[2] > 0) {}
			else
			{
				if (cvar.wb_duck->value != 0) cmd->buttons |= IN_DUCK;
				cmd->buttons |= IN_FORWARD;
				cmd->forwardmove = 176.8;
				cmd->sidemove = 176.8 * dr;
				cmd->buttons |= (dr == 1 ? (IN_MOVERIGHT) : (IN_MOVELEFT));
				if (moves <= 0)
				{
					if (lox)
					{
						dr = 1;
						myang[1] += g_Engine.pfnRandomFloat(0, cvar.wb_ang->value);
						lox = !lox;

					}
					else if (!lox)
					{
						dr = -1;
						myang[1] -= g_Engine.pfnRandomFloat(0, cvar.wb_ang->value);
						lox = !lox;
					}
					moves = (int)cvar.wb_move->value;
				}
				else moves--;
				g_Engine.SetViewAngles(myang);
			}
		}
	}
}

void bug_s::CL_CreateMove(float frametime, usercmd_s *cmd)
{
	JumpBug(frametime, cmd);
	EdgeBug(frametime, cmd);
	WallBug(cmd);
}
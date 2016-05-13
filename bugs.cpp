#include "bugs.h"
#include "cvars.h"
#include "calculations.h"
#include "functions.h"

bug_s g_Bug;

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

void bug_s::CL_CreateMove(float frametime, usercmd_s *cmd)
{
	JumpBug(frametime, cmd);
}
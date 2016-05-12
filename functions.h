#pragma once
#include "main.h"

class function_s
{
public:
	static void BunnyHop(float frametime, usercmd_s *cmd);
	static void GroundStrafe(float frametime, struct usercmd_s *cmd);
	static void CL_CreateMove(float frametime, usercmd_s *cmd, int active);
};
extern function_s g_Function;
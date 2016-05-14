#pragma once
#include "main.h"

class function_s
{
public:
	static void AdjustSpeed(double x);
	static void BunnyHop(float frametime, usercmd_s *cmd);
	static void GroundStrafe(float frametime, struct usercmd_s *cmd);
	static void FastRun(struct usercmd_s *cmd);
	static void Skipframes(int value);
	static void CL_CreateMove(float frametime, usercmd_s *cmd, int active);
};
extern function_s g_Function;
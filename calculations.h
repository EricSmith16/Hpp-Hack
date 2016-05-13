#pragma once
#include "main.h"

class calculation_s 
{
public:
	static bool ScanFromCvar(const char* str, int* value);
	static void MyVectorAngles(const float *forward, float *angles);
	static void RotateInvisible(float fixed_yaw, float fixed_pitch, usercmd_s *cmd);
	static float EdgeDistance();
	static void SlowVerticalVel(float to, float frametime, usercmd_s *cmd);
};
extern calculation_s g_Calculation;
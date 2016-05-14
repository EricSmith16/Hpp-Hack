#include "calculations.h"

calculation_s g_Calculation;

bool calculation_s::ScanFromCvar(const char* str, int* value)
{
	int len = strlen(str);
	if (len>11)return false;
	int clrnum = 0;
	int clrsum = 0;
	for (int i = 0; i<len; i++)
	{
		if (clrnum >= 3) return false;
		if (str[i] >= '0'&&str[i] <= '9')
			clrsum = clrsum * 10 + str[i] - (int)'0';
		else
		{
			value[clrnum++] = clrsum;
			clrsum = 0;
		}
	}
	value[clrnum] = clrsum;
	return true;
}

void calculation_s::MyVectorAngles(const float *forward, float *angles)
{
	float tmp, yaw, pitch;
	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0) pitch = 270;
		else pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0) yaw += 360;
		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0) pitch += 360;
	}
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
	while (angles[0]<-89) { angles[0] += 180; angles[1] += 180; }
	while (angles[0]>89) { angles[0] -= 180; angles[1] += 180; }
	while (angles[1]<-180) { angles[1] += 360; }
	while (angles[1]>180) { angles[1] -= 360; }
}

void calculation_s::RotateInvisible(float fixed_yaw, float fixed_pitch, usercmd_s *cmd)
{
	Vector viewforward, viewright, viewup, aimforward, aimright, aimup, vTemp;
	float newforward, newright, newup;
	float forward = cmd->forwardmove;
	float right = cmd->sidemove;
	float up = cmd->upmove;
	Vector tipo_real_va;
	VectorCopy(cmd->viewangles, tipo_real_va);
	g_Engine.pfnAngleVectors(Vector(0.0f, tipo_real_va.y, 0.0f), viewforward, viewright, viewup);
	tipo_real_va.y += fixed_yaw;
	g_Engine.pfnAngleVectors(Vector(0.0f, tipo_real_va.y, 0.0f), aimforward, aimright, aimup);
	newforward = DotProduct(forward * viewforward.Normalize(), aimforward) + DotProduct(right * viewright.Normalize(), aimforward) + DotProduct(up * viewup.Normalize(), aimforward);
	newright = DotProduct(forward * viewforward.Normalize(), aimright) + DotProduct(right * viewright.Normalize(), aimright) + DotProduct(up * viewup.Normalize(), aimright);
	newup = DotProduct(forward * viewforward.Normalize(), aimup) + DotProduct(right * viewright.Normalize(), aimup) + DotProduct(up * viewup.Normalize(), aimup);
	if (fixed_pitch>81) cmd->forwardmove = -newforward;
	else cmd->forwardmove = newforward;
	cmd->sidemove = newright;
	cmd->upmove = newup;
}

void calculation_s::SlowVerticalVel(float to, float frametime, usercmd_s *cmd)
{
	g_Local.bSlowDown = true;
	float bvel = ((g_Local.fVSpeed + g_Local.fVSpeed*frametime) - to) / (frametime * 100);
	float vspeed[3] = { g_Local.vVelocity.x / g_Local.fVSpeed,g_Local.vVelocity.y / g_Local.fVSpeed,0.0f };
	float va_speed[3] = {}; MyVectorAngles(vspeed, va_speed);
	float adif = va_speed[1] - cmd->viewangles[1];
	while (adif<-180)adif += 360;
	while (adif>180)adif -= 360;
	cmd->forwardmove = -bvel;
	cmd->sidemove = 0;
	if (!cmd->buttons&IN_JUMP && !cmd->buttons&IN_DUCK) cmd->buttons = 0;
	RotateInvisible(-(va_speed[1] - cmd->viewangles[1]), 0, cmd);
}

float calculation_s::EdgeDistance()
{
#define TraceEdge(x,y){\
	Vector start=g_Local.vOrigin,airTemp=g_Local.vOrigin;\
	airTemp[2] -= 8192+8192+8192;\
	pmtrace_t *gtrace = g_Engine.PM_TraceLine(start, airTemp, 1, g_Local.iUseHull, -1);\
	vec3_t vDis = (airTemp - g_Local.vOrigin) * gtrace->fraction;\
	start[2]-=0.1f;\
	start[2]-=-vDis[2];\
	Vector end=start;\
	end[1]+=x*mind;\
	end[0]+=y*mind;\
	pmtrace_s* t1 = g_Engine.PM_TraceLine(end,start,1,g_Local.iUseHull,-1);\
	if(!(t1->startsolid))mind=(t1->endpos-start).Length2D();\
}
	float mind = 250;
	TraceEdge(-1, 0);
	TraceEdge(1, 0);
	TraceEdge(0, 1);
	TraceEdge(0, -1);
	TraceEdge(-1, -1);
	TraceEdge(1, 1);
	TraceEdge(1, -1);
	TraceEdge(-1, 1);
	return mind;
}

float* calculation_s::CopyCvar(char* origCvarName, char* newCvarName, char* defaultValue, int origFlags)
{
	cvar_t* pCvar = g_Engine.pfnGetCvarPointer(origCvarName);

	if (!pCvar)
		return NULL;

	cvar_t* pNewVar = pCvar;
	memcpy(pNewVar, pCvar, sizeof(cvar_t));

	char tmp[64];
	sprintf(tmp, "%s", newCvarName);

	strcpy((char*)pCvar->name, tmp);

	g_Engine.pfnRegisterVariable(origCvarName, defaultValue, origFlags);
	g_Engine.pfnCvar_Set(newCvarName, defaultValue);

	return &pCvar->value;
}
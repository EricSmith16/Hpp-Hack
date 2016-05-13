#include "client.h"
#include "functions.h"
#include "cvars.h"

local_s g_Local;
player_s g_Player[33];

bool FirstFrame = false;
extern double *g_Net;

void HookUserMessages()
{
	pUserMsgBase = (PUserMsg)offset.FindUserMsgBase();
	pResetHUD = HookUserMsg("ResetHUD", ResetHUD);
	pSetFOV = HookUserMsg("SetFOV", SetFOV);
	pTeamInfo = HookUserMsg("TeamInfo", TeamInfo);
	pCurWeapon = HookUserMsg("CurWeapon", CurWeapon);
	pDeathMsg = HookUserMsg("DeathMsg", DeathMsg);
}

void HookEngineMessages()
{
	pEngineMsgBase = (PEngineMsg)offset.FindSVCMessages();
	pSVC_sound = HookEngineMsg("svc_sound", SVC_Sound);
	pSVC_StuffText = HookEngineMsg("svc_stufftext", SVC_StuffText);
	pSVC_NewUserMsg = HookEngineMsg("svc_newusermsg", SVC_NewUserMsg);
	pSVC_UpdateUserInfo = HookEngineMsg("svc_updateuserinfo", SVC_UpdateUserInfo);
	pSVC_SendCvarValue = HookEngineMsg("svc_sendcvarvalue", SVC_SendCvarValue);
	pSVC_SendCvarValue2 = HookEngineMsg("svc_sendcvarvalue2", SVC_SendCvarValue2);
	pSVC_Director = HookEngineMsg("svc_director", SVC_Director);
}

void ConsolePrintColor(BYTE R, BYTE G, BYTE B, char* string)
{
	TColor24 DefaultColor;
	PColor24 Ptr;
	Ptr = Console_TextColor;
	DefaultColor = *Ptr;
	Ptr->R = R;
	Ptr->G = G;
	Ptr->B = B;
	g_Engine.Con_Printf("%s", string);
	*Ptr = DefaultColor;
}

char* ConvertTypeToRenderString(byte type)
{
	char* RenderTypeGL = "opengl";
	char* RenderTypeD3 = "d3d";
	char* RenderTypeSF = "software";
	if (type == 3) return RenderTypeD3;
	else if (type == 2) return RenderTypeGL;
	else if (type == 1) return RenderTypeSF;
	return "uncnown";
}

void InitHack()
{
	font.InitText("Lucida Console", 12, 9);
	cfunc.Init();
}

void HUD_Frame(double time)
{
	if (!FirstFrame)
	{
		cOffset* Offset = new cOffset;
		while (!Offset->GetRendererInfo()) Sleep(90);
		g_Local.SpeedPtr = 0;
		g_Local.dwSpeedptr = 0;
		Offset->SpeedPtr = (DWORD)Offset->SpeedHackPtr();
		g_Local.SpeedPtr = Offset->SpeedPtr;
		g_Net = (double*)*(PDWORD)((DWORD)g_Engine.pNetAPI->SendRequest + 0x51);
		if (IsBadReadPtr(g_Net, sizeof(double))) g_Net = (double*)*(PDWORD)((DWORD)g_Engine.pNetAPI->SendRequest + 0x49);
		g_Screen.iSize = sizeof(SCREENINFO);
		offset.ConsoleColorInitalize();
		offset.SVCMessagesInitaizie();
		HookUserMessages();
		HookEngineMessages();
		InitHack();
		FirstFrame = true;
	}
	g_Engine.pfnGetScreenInfo(&g_Screen);
	g_Client.HUD_Frame(time);
}

void HUD_Redraw(float time, int intermission)
{
	g_Client.HUD_Redraw(time, intermission);
	cl_entity_t *pLocal = g_Engine.GetLocalPlayer();
	g_Local.iIndex = pLocal->index;
}

void HUD_PlayerMove(struct playermove_s *ppmove, int server)
{
	g_Client.HUD_PlayerMove(ppmove, server);
	g_Local.vOrigin = ppmove->origin;
	g_Local.iUseHull = ppmove->usehull;
	g_Local.iFlags = ppmove->flags;
	g_Local.flXYspeed = sqrt(POW(ppmove->velocity[0]) + POW(ppmove->velocity[1]));
	g_Local.flFallSpeed = ppmove->flFallVelocity;
	g_Local.fVSpeed = ppmove->velocity.Length();
	g_Local.vVelocity = ppmove->velocity;
	Vector vTemp = ppmove->origin;
	vTemp[2] -= 8192;
	pmtrace_t *trace = g_Engine.PM_TraceLine(ppmove->origin, vTemp, 1, ppmove->usehull, -1);
	g_Local.flGroundAngle = acos(trace->plane.normal[2]) / M_PI * 180;
	Vector vTemp1 = trace->endpos;
	pmtrace_t pTrace;
	g_Engine.pEventAPI->EV_SetTraceHull(ppmove->usehull);
	g_Engine.pEventAPI->EV_PlayerTrace(ppmove->origin, vTemp1, PM_GLASS_IGNORE | PM_STUDIO_BOX, g_Local.iIndex, &pTrace);
	if (pTrace.fraction < 1.0f)
	{
		g_Local.Height = abs(pTrace.endpos.z - ppmove->origin.z);
		int ind = g_Engine.pEventAPI->EV_IndexFromTrace(&pTrace);
		if (ind >= 1 && ind <= 32)
		{
			float dst = ppmove->origin.z - (g_Local.iUseHull == 0 ? 32 : 18) - g_Player[ind].vOrigin.z - g_Local.Height;
			if (dst<30)
				g_Local.Height -= 14.0000;
		}
	}
	else
	{
		if (g_Local.flGroundAngle>1)
		{
			Vector vTemp2 = ppmove->origin;
			vTemp2[2] -= 8192;
			pmtrace_t *trace4 = g_Engine.PM_TraceLine(ppmove->origin, vTemp2, 1, 2, -1);
			g_Local.Height = abs(ppmove->origin.z - trace4->endpos.z - (g_Local.iUseHull == 1 ? 18.0f : 36.0f));
		}
		else
		{
			Vector vTemp5 = ppmove->origin;
			vTemp5[2] -= 8192;
			pmtrace_t *trace5 = g_Engine.PM_TraceLine(ppmove->origin, vTemp5, 1, ppmove->usehull, -1);
			g_Local.Height = abs(trace5->endpos.z - ppmove->origin.z);
		}
	}
}

void CL_CreateMove(float frametime, usercmd_s *cmd, int active)
{
	g_Client.CL_CreateMove(frametime, cmd, active);
	g_Function.CL_CreateMove(frametime, cmd, active);
}

void HookFunction()
{
	g_pClient->HUD_Frame = HUD_Frame;
	g_pClient->HUD_Redraw = HUD_Redraw;
	g_pClient->HUD_PlayerMove = HUD_PlayerMove;
	g_pClient->CL_CreateMove = CL_CreateMove;
}
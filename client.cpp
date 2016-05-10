#include "client.h"
#include "functions.h"
#include "cvars.h"

bool FirstFrame = false;

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
	g_pClient->CL_CreateMove = CL_CreateMove;
}
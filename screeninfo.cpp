#include "screeninfo.h"
#include "cvars.h"
#include <ctime>
#include "calculations.h"

screeninfo_s g_Screeninfo;

void screeninfo_s::Info()
{
	char dateStr[9];
	char timeStr[9];
	_strdate(dateStr);
	_strtime(timeStr);
	int clr[3] = {};
	g_Calculation.ScanFromCvar(cvar.show_color->string, clr);
	if (cvar.show_time->value)
	{
		font.Print(g_Screen.iWidth - 130, 15, clr[0], clr[1], clr[2], 255, 0, "Time: %s", timeStr);
		font.Print(g_Screen.iWidth - 130, 30, clr[0], clr[1], clr[2], 255, 0, "Date: %s", dateStr);
	}
	if (cvar.show_fps->value) font.Print(g_Screen.iWidth / 110, 15, clr[0], clr[1], clr[2], 255, 0, "%.1f", g_Local.fFps);
	if (cvar.show_copyright->value) font.Print(g_Screen.iWidth / 2, 15, clr[0], clr[1], clr[2], 255, FL_CENTER, "[Hpp Hack v2.0]");
	if (cvar.show_keys->value && g_Local.bAlive)
	{
		font.Print(g_Screen.iWidth / 2, g_Screen.iHeight * 0.43, clr[0], clr[1], clr[2], 255, FL_CENTER, "  %s  ", cfunc.forwardm ? "W" : ".");
		font.Print(g_Screen.iWidth / 2, g_Screen.iHeight * 0.45, clr[0], clr[1], clr[2], 255, FL_CENTER, "%s %s %s", cfunc.moveleft ? "A" : ".", cfunc.back ? "S" : ".", cfunc.moveright ? "D" : ".");
		font.Print(695, g_Screen.iHeight * 0.45, clr[0], clr[1], clr[2], 255, FL_CENTER, "%s", cfunc.duck ? "duck" : "-");
		font.Print(695, g_Screen.iHeight * 0.43, clr[0], clr[1], clr[2], 255, FL_CENTER, "%s", cfunc.jump ? "jump" : "-");
	}
}

void screeninfo_s::InfoKreedz()
{
	int clr_kz[4] = {};
	g_Calculation.ScanFromCvar(cvar.show_info_color->string, clr_kz);
	if (cvar.show_info_kz->value && g_Local.bAlive)
	{
		font.Print(g_Screen.iWidth / 2, g_Screen.iHeight / 1.75, clr_kz[0], clr_kz[1], clr_kz[2], 255, FL_CENTER, "HSpeed: %.1f", g_Local.flXYspeed);
		font.Print(g_Screen.iWidth / 2, g_Screen.iHeight / 1.70, clr_kz[0], clr_kz[1], clr_kz[2], 255, FL_CENTER, "VSpeed: %.1f", abs(g_Local.flFallSpeed));
		font.Print(g_Screen.iWidth / 2, g_Screen.iHeight / 1.65, clr_kz[0], clr_kz[1], clr_kz[2], 255, FL_CENTER, "Height: %.1f", g_Local.Height);
		font.Print(g_Screen.iWidth / 2, g_Screen.iHeight / 1.60, clr_kz[0], clr_kz[1], clr_kz[2], 255, FL_CENTER, "EdgeDistance: %.1f", g_Local.EdgeDist);
		font.Print(g_Screen.iWidth / 2, g_Screen.iHeight / 1.55, clr_kz[0], clr_kz[1], clr_kz[2], 255, FL_CENTER, "GroundAngle: %.1f", g_Local.flGroundAngle);
	}
	if (cvar.show_info_moves->value)
	{
		font.Print(g_Screen.iWidth / 110, 200, clr_kz[0], clr_kz[1], clr_kz[2], 255, 0, "Sidemove: %.1f", g_Local.flsidemove);
		font.Print(g_Screen.iWidth / 110, 215, clr_kz[0], clr_kz[1], clr_kz[2], 255, 0, "Forwardmove: %.1f", g_Local.flforwardmove);
	}
}

void screeninfo_s::HUD_RedRaw()
{
	if (cvar.show->value)
	{
		Info();
		InfoKreedz();
	}
}
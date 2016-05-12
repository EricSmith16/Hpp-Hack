#include "cvars.h"

cvars_s cvar;
cfunc_s cfunc;

bool cfunc_s::Bhop = false;
void cfunc_s::BhopOn() { Bhop = true; }
void cfunc_s::BhopOff() { Bhop = false; }

bool cfunc_s::Gstrafe = false;
void cfunc_s::GstrafeOn() { Gstrafe = true; }
void cfunc_s::GstrafeOff() { Gstrafe = false; }

bool cfunc_s::FastRun = false;
void cfunc_s::FastRunOn() { FastRun = true; }
void cfunc_s::FastRunOff() { FastRun = false; }
void cfunc_s::FastRunSwitch() { FastRun = !FastRun; }

void cfunc_s::Init()
{
	rnc(bhop_scroll_start, "hpp_bhop_scroll_start", "5 9")
		rnc(bhop_scroll_count, "hpp_bhop_scroll_count", "4 6")
		rnc(bhop_standup, "hpp_bhop_standup", "0")
		rnc(bhop_standup_start, "hpp_bhop_standup_start", "10")
		rcmd("+hpp_bhop", BhopOn)
		rcmd("-hpp_bhop", BhopOff)

		rnc(gstrafe_scroll_count, "hpp_gstrafe_scroll_count", "2 3")
		rnc(gstrafe_scroll_start, "hpp_gstrafe_scroll_start", "2 4")
		rnc(gstrafe_standup, "hpp_gstrafe_standup", "0")
		rnc(gstrafe_standup_start, "hpp_gstrafe_standup_start", "6")
		rnc(gstrafe_slowdown, "hpp_gstrafe_slowdown", "1")
		rnc(gstrafe_slowdown_angle, "hpp_gstrafe_slowdown_angle", "15")
		rcmd("+hpp_gstrafe", GstrafeOn)
		rcmd("-hpp_gstrafe", GstrafeOff)

		rnc(speed, "hpp_speed", "1")
		rnc(speed_engine, "hpp_speed_engine", "0")

		rnc(fastrun_nsd, "hpp_fastrun_nsd", "1")
		rnc(fastrun_sidemove, "hpp_fastrun_sidemove", "138")
		rnc(fastrun_forwardmove, "hpp_fastrun_forwardmove", "267")
		rcmd("+hpp_fastrun", FastRunOn)
		rcmd("-hpp_fastrun", FastRunOff)
		rcmd("hpp_switch_fastrun", FastRunSwitch)	
}
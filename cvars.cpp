#include "cvars.h"

cvars_s cvar;
cfunc_s cfunc;

bool cfunc_s::Bhop = false;
void cfunc_s::BhopOn() { Bhop = true; }
void cfunc_s::BhopOff() { Bhop = false; }

void cfunc_s::Init()
{
	rnc(bhop_scroll_start, "hpp_bhop_scroll_start", "5 9")
		rnc(bhop_scroll_count, "hpp_bhop_scroll_count", "4 6")
		rnc(bhop_standup, "hpp_bhop_standup", "0")
		rnc(bhop_standup_start, "hpp_bhop_standup_start", "10")
		rcmd("+hpp_bhop", BhopOn)
		rcmd("-hpp_bhop", BhopOff)
}
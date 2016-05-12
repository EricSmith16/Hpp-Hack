#pragma once
#include "main.h"

class calculation_s 
{
public:
	static bool calculation_s::ScanFromCvar(const char* str, int* value);
};
extern calculation_s g_Calculation;
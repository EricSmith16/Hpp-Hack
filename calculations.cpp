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
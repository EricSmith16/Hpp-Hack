#pragma once

#include "main.h"

class bug_s
{
public:
	static void JumpBug(float frametime, usercmd_s *cmd);
	static void EdgeBug(float frametime, struct usercmd_s *cmd);
	static void WallBug(struct usercmd_s *cmd);
	static void CL_CreateMove(float frametime, usercmd_s *cmd);
};
extern bug_s g_Bug;
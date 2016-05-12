#pragma once

#include "main.h"

#define CompareMemory(Buff1, Buff2, Size) __comparemem((const UCHAR *)Buff1, (const UCHAR *)Buff2, (UINT)Size)
#define FindMemoryClone(Start, End, Clone, Size) __findmemoryclone((const ULONG)Start, (const ULONG)End, (const ULONG)Clone, (UINT)Size)
#define FindReference(Start, End, Address) __findreference((const ULONG)Start, (const ULONG)End, (const ULONG)Address)

class cOffset
{
public:
	DWORD SpeedPtr;

	DWORD HwDll , SwDll , HlMod;

	DWORD HwBase , HwSize , HwEnd;
	DWORD ClBase , ClSize , ClEnd;
	DWORD HlBase , HlSize , HlEnd;
	DWORD VgBase , VgSize , VgEnd;

	BYTE HLType;
	
	void* SpeedHackPtr();

	void GetRenderType();
	bool GetModuleInfo();

	void Error( char* Msg );

	DWORD FindClientTable();
	DWORD FindEngineTable();
	DWORD FindStudioTable();
	DWORD FindUserMsgBase();
	DWORD FindGameConsole();
	DWORD FindSVCMessages();

	void ConsoleColorInitalize();
	void SVCMessagesInitaizie();

	void CopyClient();
	void CopyEngine();
	void CopyStudio();

	DWORD Absolute( DWORD Address );

	DWORD FarProc( DWORD Address , DWORD LB , DWORD HB );

	DWORD FindPattern( PCHAR pattern , PCHAR mask , DWORD start , DWORD end , DWORD offset );
	DWORD FindPattern( PCHAR pattern , DWORD start , DWORD end , DWORD offset );

	DWORD GetModuleSize( DWORD Address );

	bool GetRendererInfo();

	BOOL __comparemem(const UCHAR *buff1, const UCHAR *buff2, UINT size);
	ULONG __findreference(const ULONG start, const ULONG end, const ULONG address);
	ULONG __findmemoryclone(const ULONG start, const ULONG end, const ULONG clone, UINT size);
};

extern cOffset offset;
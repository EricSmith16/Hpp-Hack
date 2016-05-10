#include "main.h"

cl_clientfunc_t *g_pClient = nullptr;
cl_enginefunc_t *g_pEngine = nullptr;
engine_studio_api_t *g_pStudio = nullptr;

cl_clientfunc_t g_Client;
cl_enginefunc_t g_Engine;
engine_studio_api_t g_Studio;

PUserMsg pUserMsgBase;
PEngineMsg pEngineMsgBase;
PColor24 Console_TextColor;

SCREENINFO g_Screen;

BYTE bPreType = 0;
HANDLE hProcessReloadThread = 0;

char* BaseDir;

DWORD WINAPI CheatEntry( LPVOID lpThreadParameter );

DWORD WINAPI ProcessReload(LPVOID lpThreadParameter)
{
	while (true)
	{
		if (FirstFrame)
		{
			offset.GetRenderType();
			if (bPreType != offset.HLType || !offset.GetModuleInfo()) FirstFrame = false;
		}
		else CreateThread(0, 0, CheatEntry, 0, 0, 0);
		Sleep(100);
	}
	return 0;
}

DWORD WINAPI CheatEntry(LPVOID lpThreadParameter)
{
	if (hProcessReloadThread)
	{
		TerminateThread(hProcessReloadThread, 0);
		CloseHandle(hProcessReloadThread);
	}

	BYTE counter_find = 0;

start_hook:

	if (counter_find == 100) offset.Error(ERROR_FIND);

	Sleep(100);
	counter_find++;

	if (!offset.GetModuleInfo()) goto start_hook;

	DWORD ClientTable = offset.FindClientTable();

	if (ClientTable)
	{
		g_pClient = (cl_clientfunc_t*)ClientTable;
		offset.CopyClient();

		if ((DWORD)g_Client.Initialize)
		{
			DWORD EngineTable = offset.FindEngineTable();

			if (EngineTable)
			{
				g_pEngine = (cl_enginefunc_t*)EngineTable;
				offset.CopyEngine();

				if ((DWORD)g_Engine.V_CalcShake)
				{
					DWORD StudioTable = offset.FindStudioTable();

					if (StudioTable)
					{
						g_pStudio = (engine_studio_api_t*)StudioTable;
						offset.CopyStudio();

						if ((DWORD)g_Studio.StudioSetupSkin)
						{
							HookFunction();

							while (!FirstFrame) Sleep(100);

							bPreType = offset.HLType;

							hProcessReloadThread = CreateThread(0, 0, ProcessReload, 0, 0, 0);
						}
						else
							goto start_hook;
					}
					else
						goto start_hook;
				}
				else
					goto start_hook;
			}
			else
				goto start_hook;
		}
		else
			goto start_hook;
	}
	else
		goto start_hook;

	return 0;
}

void HideModule(HINSTANCE hModule)
{
	DWORD dwPEB_LDR_DATA = 0;
	_asm
	{
		pushad;
		pushfd;
		mov eax, fs:[30h];
		mov eax, [eax + 0Ch];
		mov dwPEB_LDR_DATA, eax;
		mov esi, [eax + 0Ch];
		mov edx, [eax + 10h];
	LoopInLoadOrderModuleList:
		lodsd;
		mov esi, eax;
		mov ecx, [eax + 18h];
		cmp ecx, hModule;
		jne SkipA
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InMemoryOrderModuleList
			SkipA :
		cmp edx, esi
			jne LoopInLoadOrderModuleList
			InMemoryOrderModuleList :
		mov eax, dwPEB_LDR_DATA
			mov esi, [eax + 14h]
			mov edx, [eax + 18h]
			LoopInMemoryOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 10h]
			cmp ecx, hModule
			jne SkipB
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InInitializationOrderModuleList
			SkipB :
		cmp edx, esi
			jne LoopInMemoryOrderModuleList
			InInitializationOrderModuleList :
		mov eax, dwPEB_LDR_DATA
			mov esi, [eax + 1Ch]
			mov edx, [eax + 20h]
			LoopInInitializationOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 08h]
			cmp ecx, hModule
			jne SkipC
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp Finished
			SkipC :
		cmp edx, esi
			jne LoopInInitializationOrderModuleList
			Finished :
		popfd;
		popad;
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		BaseDir = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_PATH);
		GetModuleFileNameA(hinstDLL, BaseDir, MAX_PATH);
		char* pos = BaseDir + native_strlen(BaseDir);
		while (pos >= BaseDir && *pos != '\\') --pos; pos[1] = 0;
		HideModule(hinstDLL);
		CreateThread(0, 0, CheatEntry, 0, 0, 0);
		return TRUE;
	}
	return FALSE;
}
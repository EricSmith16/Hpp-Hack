#include "utils.h"

PUserMsg UserMsgByName( char* szMsgName )
{
	PUserMsg Ptr = nullptr;
	Ptr = pUserMsgBase;

	while ( Ptr->next )
	{
		if ( !native_strcmp( Ptr->name , szMsgName ) )
			return Ptr;

		Ptr = Ptr->next;
	}

	Ptr->pfn = 0;
	return Ptr;
}

PEngineMsg EngineMsgByName( char* szMsgName )
{
	PEngineMsg Ptr = nullptr;
	Ptr = pEngineMsgBase;

	while ( native_strcmp( Ptr->name , OFF_SVC_MESSAGES_P ) )
	{
		if ( !native_strcmp( Ptr->name , szMsgName ) )
			return Ptr;

		Ptr++;
	}

	Ptr->pfn = 0;
	return Ptr;
}

pcmd_t CommandByName( char* szName )
{
	pcmd_t pCmd = nullptr;
	pCmd = g_Engine.pfnGetCmdList();

	while ( pCmd )
	{
		if ( !native_strcmp( pCmd->name , szName ) )
			return pCmd;

		pCmd = pCmd->next;
	}

	return 0;
}

pfnUserMsgHook HookUserMsg( char *szMsgName , pfnUserMsgHook pfn )
{
	PUserMsg Ptr = nullptr;
	pfnUserMsgHook Original = nullptr;

	Ptr = UserMsgByName( szMsgName );

	if ( Ptr->pfn != 0 )
	{
		Original = Ptr->pfn;
		Ptr->pfn = pfn;
	}
	else
	{
		MessageBoxA( 0 , szMsgName , 0 , MB_OK | MB_ICONERROR );
	}

	return Original;
}

pfnEngineMessage HookEngineMsg( char *szMsgName , pfnEngineMessage pfn )
{
	PEngineMsg Ptr = nullptr;
	pfnEngineMessage Original = nullptr;

	Ptr = EngineMsgByName( szMsgName );

	if ( Ptr->pfn != 0 )
	{
		Original = Ptr->pfn;
		Ptr->pfn = pfn;
	}
	else
	{
		MessageBoxA( 0 , szMsgName , 0 , MB_OK | MB_ICONERROR );
	}

	return Original;
}

char* native_strcpy( char *dest , const char *src )
{
	return lstrcpyA( dest , src );
}

int native_strcmp( char *pStr1 , char *pStr2 )
{
	return lstrcmpA( pStr1 , pStr2 );
}

int native_strlen( char *pStr )
{
	return lstrlenA( pStr );
}

void* native_memcpy( void * dst , const void * src , size_t count )
{
	_asm
	{
		mov	edi , [dst]
		mov	esi , [src]
		mov	ecx , [count]
		rep	movsb
	}
}

int CallApi( HMODULE hMod , char* szFunction , int numberParameters , ... )
{
	DWORD dwAddrFunc = 0;
	va_list param;
	LPVOID* pargv;
	int ret_value;

	dwAddrFunc = (DWORD)GetProcAddress( hMod , szFunction );

	if ( !dwAddrFunc ) return 0;

	va_start( param , numberParameters );

	pargv = &va_arg( param , PVOID );

	for ( int n = numberParameters - 1; n >= 0; n-- )
	{
		PVOID temp = pargv[n];
		__asm push[temp]
	}

	va_end( param );

	_asm call dwAddrFunc
	_asm mov ret_value , eax

	return ret_value;
}
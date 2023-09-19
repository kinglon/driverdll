#include "stdafx.h"

CLogUtil* g_dllLog = nullptr;
HMODULE g_hDllModule;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call) 
	{
	case DLL_PROCESS_ATTACH:
        g_hDllModule = hModule;
        g_dllLog = CLogUtil::GetLog(DEVICE_NAME);
        break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


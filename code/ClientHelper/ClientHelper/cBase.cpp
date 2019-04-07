#include "cBase.h"

bool IsSJSet = false, IsHIPSet = false;

DWORD WINAPI Thread1()
{
	MH_Initialize();

	while (!IsSJSet)
	{
		if (DWORD NSessionJoinAdr = 0x496A50)
		{
			if (MH_CreateHook((void*)NSessionJoinAdr, SessionJoinHook, (LPVOID*)&mSessionJoin) == MH_OK)
			{
				MH_EnableHook((void*)NSessionJoinAdr);
				IsSJSet = true;
			}
		}
		Sleep(100);
	}

	while (!IsHIPSet)
	{
		if (DWORD dwNHitInfoParser = 0x6D1470)
		{
			if (MH_CreateHook((void*)dwNHitInfoParser, HitInfoParserHook, (LPVOID*)&mHitInfoParser) == MH_OK)
			{
				MH_EnableHook((void*)dwNHitInfoParser);
				IsHIPSet = true;
			}
		}
		Sleep(100);
	}

	while (true)
	{
		Update1Info();
		Sleep(50);
	}
	return 0;
}

BOOL __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread1, 0, 0, 0));
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CRecvFromDH, 0, 0, 0));
		return TRUE;
	}
	return FALSE;
}
#ifndef TOOLS_H_
#define TOOLS_H_
#include <Windows.h>


class SigScanVars {
public:

	DWORD dwLTClientEXE;
	DWORD dwEngineEntryPoint, dwEngineCodeSize, dwCShellEntry; // CShell/Engine Stuff
	DWORD dwNameTags1, dwNameTags2, dwSuperbullets; // Confirmed working for NameTags & SuperBullets

  //DWORD dwNoRecoil, dwRapidFire, dwSuperbullets, dwWeaponRange1, dwWeaponRange2; //Update during free time..
  //DWORD dwClientInfoMgr, dwGameStatus, dwPlayerByIndex, dwLocalPlayer; // Update during free time..
};


class cTools {
public:
	INT MemCopy(VOID *lpMem, VOID *lpSrc, size_t len);
	VOID WINAPI PushToConsole(const char *PTCCommand);
	bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask);
	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask);
	DWORD GetCodeSize(const char* szModuleName);
	void AddressSigScans();

	bool GameReady2Hook(VOID);
	VOID *DetourCreate(BYTE *src, const BYTE *dst);
	DWORD* FindDevice(DWORD Base, DWORD Len);
	DWORD GetVTable(int value);
};
#endif 
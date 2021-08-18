#include "Tools.h"
#include "Addresses.h"
#include "EncryptedStrings.h"


SigScanVars scanVars;
cTools *tools;
DWORD Base = (DWORD)LoadLibraryA(D3D9DLL);
#define JMP32_SZ 5 // the size of JMP <address>
#define NOP 0x90 // opcode for NOP


INT cTools::MemCopy(VOID *lpMem, VOID *lpSrc, size_t len)
{
	DWORD flNewProtect = PAGE_READWRITE, lpflOldProtect;
	unsigned char *pDst = (unsigned char *)lpMem, *pSrc = (unsigned char *)lpSrc;

	if (VirtualProtect(lpMem, len, flNewProtect, &lpflOldProtect))
	{
		while (len-- > 0) *pDst++ = *pSrc++;
		return 0;
	}

	return 1;

}

VOID WINAPI cTools::PushToConsole(const char *PTCCommand)
{
	__asm
	{
		PUSHAD
		PUSH PTCCommand
		MOV EAX, UnwrappedConsole
		CALL EAX
		ADD ESP, 0x4
		POPAD
	}
}

bool cTools::bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	return (*szMask) == NULL;

}

DWORD cTools::FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
{
	for (DWORD i = 0; i < dwLen; i++)
		if (tools->bDataCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);

	return 0;
}

DWORD cTools::GetCodeSize(const char* szModuleName)
{

	HMODULE hModule = GetModuleHandleA(szModuleName);

	if (!hModule)
		return NULL;

	PIMAGE_DOS_HEADER pDosHeader = PIMAGE_DOS_HEADER(hModule);

	if (!pDosHeader)
		return NULL;

	PIMAGE_NT_HEADERS pNTHeader = PIMAGE_NT_HEADERS((LONG)hModule + pDosHeader->e_lfanew);

	if (!pNTHeader)
		return NULL;

	PIMAGE_OPTIONAL_HEADER pOptionalHeader = &pNTHeader->OptionalHeader;

	if (!pOptionalHeader)
		return NULL;

	return pOptionalHeader->SizeOfCode;


}

void cTools::AddressSigScans()
{
	scanVars.dwEngineEntryPoint = (DWORD)GetModuleHandleA(Engine);
	scanVars.dwEngineCodeSize = GetCodeSize(Engine);
	scanVars.dwCShellEntry = (DWORD)GetModuleHandleA(CShell);

	scanVars.dwLTClientEXE = FindPattern(scanVars.dwEngineEntryPoint, scanVars.dwEngineCodeSize, (BYTE*)"\x8B\x44\x24\x04\x50\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x08\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x8B\xC1\x33\xC9", "xxxxxx????x????xxxxxxxxxxxxxxxxxxxxx");
	scanVars.dwNameTags1 = FindPattern(scanVars.dwCShellEntry, 0xbb6000, (PBYTE)"\x75\x05\xBB\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x8D\x54\x24\x1C\xC7\x44\x24\x00\x00\x00\x00\x00", "xxx????x????xxxxxxx?????"); // confirmed still working
	scanVars.dwNameTags2 = FindPattern(scanVars.dwCShellEntry, 0xbb6000, (PBYTE)"\x75\x05\xBB\x00\x00\x00\x00\x8B\x17\x8B\x92\x00\x00\x00\x00", "xxx????xxxx????"); // confirmed still working
	scanVars.dwSuperbullets = FindPattern(scanVars.dwCShellEntry, 0xbb6000, (PBYTE)"\x0F\x94\xC0\x5E\xC3\xCC\xCC", "xxxxxxx"); // confirmed still working
}

bool cTools::GameReady2Hook(VOID)
{
	if (GetModuleHandle(D3D9DLL) != NULL
		&& GetModuleHandle(CShell) != NULL
		&& GetModuleHandle(CLIENTFX) != NULL)
		return true;
	return false;
}

void *cTools::DetourCreate(BYTE *src, const BYTE *dst)
{
	unsigned int ePointInstruction[] = { 0xE9 };
	int len = 5;

	BYTE *jmp = (BYTE*)malloc(len + 5);
	DWORD DatVPBackToBack;

	VirtualProtect(src, len, PAGE_READWRITE, &DatVPBackToBack);
	memcpy(jmp, src, len);

	jmp += len;
	jmp[0] = ePointInstruction[0];
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;

	src[0] = ePointInstruction[0];
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;

	VirtualProtect(src, len, DatVPBackToBack, &DatVPBackToBack);

	return(jmp - len);
}

DWORD* cTools::FindDevice(DWORD Base, DWORD Len)
{

	unsigned long i = 0, n = 0;

	for (i = 0; i < Len; i++)
	{
		if (*(BYTE *)(Base + i + 0x00) == 0xC7)n++;
		if (*(BYTE *)(Base + i + 0x01) == 0x06)n++;
		if (*(BYTE *)(Base + i + 0x06) == 0x89)n++;
		if (*(BYTE *)(Base + i + 0x07) == 0x86)n++;
		if (*(BYTE *)(Base + i + 0x0C) == 0x89)n++;
		if (*(BYTE *)(Base + i + 0x0D) == 0x86)n++;

		if (n == 6) return (DWORD_PTR *)(Base + i + 2);
		n = 0;
	}
	return(0);
}



DWORD cTools::GetVTable(int value)
{
	DWORD *Device = ***(DWORD ****)DeviceGame;

	if (Device == NULL)
		return 0;

	return Device[value];
}


/*
void *cTools::DetourCreate2(BYTE *original, BYTE *hook, int length)
{
	DWORD dwJumpBack;
	BYTE *jmp = (BYTE*)malloc(length + 5);//allocate a place in the memory
	VirtualProtect(original, length, PAGE_READWRITE, &dwJumpBack);//remove protection
	memcpy(jmp, original, length);//copying a place in the memory for our bytes
	jmp += length;//increment to the end of the bytes
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(original + length - jmp) - 5;
	memset(original, 0x90, length);//NOP all the bytes for god practice
	original[0] = 0xE9;//writing/creating a jump at the original to the hook function
	*(DWORD*)(original + 1) = (DWORD)(hook - original) - 5;// ^
	VirtualProtect(original, length, dwJumpBack, 0);//apply protection
	return(jmp - length);//return the pointer to the start of the memory place
}
*/
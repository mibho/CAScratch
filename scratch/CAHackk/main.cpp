#undef UNICODE
#include <Windows.h>
#include "Tools.h"
#include "Menu.h"
#include "Addresses.h"
#include "HackClass.h"


#define FDLength 0x128000

LPD3DXLINE pLine;

extern LPD3DXFONT D3DpFont;
HackClass *hClass;
extern cTools *tools;
extern menuFunct * cMenu;
extern DWORD Base;

extern LPDIRECT3DDEVICE9 pDevice;

extern D3DVIEWPORT9 Viewport;


bool bFont;
extern char sFPS[20];



typedef HRESULT(WINAPI *tReset)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS *a);
tReset oReset;

typedef HRESULT(WINAPI *tPresent)(LPDIRECT3DDEVICE9 pDevice, const RECT *a, const RECT *b, HWND c, const RGNDATA *d);
tPresent oPresent;

HRESULT WINAPI myReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS *a)
{
	_asm nop;
	D3DpFont->OnLostDevice();

	HRESULT hTorna = oReset(pDevice, a);

	if (SUCCEEDED(hTorna))
		D3DpFont->OnResetDevice();


	return hTorna;
	_asm jmp[myReset];

}

HRESULT WINAPI myPresent(LPDIRECT3DDEVICE9 pDevice, const RECT *a, const RECT *b, HWND c, const RGNDATA *d)
{
	_asm pushad;
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

	//	if( !pLine )
	//	D3DXCreateLine( pDevice, &pLine );
	cMenu->ControlFont(pDevice);
	cMenu->showMenu(pDevice);

		hClass->FPSCheck(sFPS, "%d");
	hClass->InitAllHacks(pDevice);
	//	ESP(pDevice);
	//	doaimbot();
	pDevice->SetRenderState(D3DRS_ZENABLE, true);
	_asm popad;
	return oPresent(pDevice, a, b, c, d);

	_asm jmp[myPresent];
}



VOID Hook(VOID)
{

	DWORD * VtablePtr = tools->FindDevice(Base, FDLength);
	//  DWORD* devicePtr = ***(DWORD****)VtablePtr;
	DWORD * VTable;
	//DWORD * VTableR ;
	*(DWORD *)&VTable = *(DWORD *)VtablePtr;
	//*(DWORD *)&VTableR = *(DWORD *)VtablePtr;

	DWORD VReset = tools->GetVTable(16);
	DWORD VPresent = tools->GetVTable(17);


	oReset   = ( tReset   )tools->DetourCreate((PBYTE) VTable[16], ( PBYTE ) &myReset);
	oPresent = ( tPresent )tools->DetourCreate((PBYTE) VTable[17], ( PBYTE ) &myPresent);
	oPresent = (tPresent) (DWORD)VTable[17];

}


DWORD WINAPI dwHookThread(LPVOID)
{
	while (!tools->GameReady2Hook())
		Sleep(25);

	Hook();
	tools->AddressSigScans();
	return NULL;


}

/*

DWORD WINAPI teleThread(LPVOID)
{
	while (!tools->GameReady2Hook())
		wait(25);
	Reduce();
	hClass->TeleKillHack(pDevice);

	return NULL;
}

*/
/*
DWORD WINAPI aimbot(LPVOID)
{
	while (!GameReady2Hook())//{
		Sleep(300);
	doaimbot();
	//}

	return 0;

}
*/

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);

	if (dwReason == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, NULL, &dwHookThread, NULL, NULL, NULL);
//		CreateThread(NULL, NULL, &teleThread, NULL, NULL, NULL);
//		CreateThread(NULL, NULL, &aimbot, NULL, NULL, NULL);
		MessageBox(0, "Successfully Injected!", "Created using Scratch!", MB_OK);
	}
	return TRUE;
}

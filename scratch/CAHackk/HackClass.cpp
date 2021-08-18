#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include "HackClass.h"
#include "Tools.h"
#include "Addresses.h"
#include "consoleTime.h"
#include "EncryptedStrings.h"
#include "Menu.h"

CTimer ConsoleTimer;

extern cTools *tools;
extern SigScanVars scanVars;
extern menuFunct * cMenu;

extern int MenuPosX, MenuPosY, MenuPosy;

float OgBwepData[1][500];
bool SetWepData2, StoreWepData2;

INT Mems[30];
INT Consoles[15];
INT misc[5];
INT crossHair;
bool Console[15];
bool Mem[15];
int	  FPScounter = 0;
float FPSfLastTickCount = 0.0f;
float FPSfCurrentTickCount;



bool rRapidfire, SetRapidFire;
float EndWait = 2;

void HackClass::InitMemoryHacks(LPDIRECT3DDEVICE9 pDevice)
{
	if ((*(BYTE *)GameStatus == 1) && Mems[0])
	{
		if (!Mem[0])
		{
			tools->MemCopy((void *)NoRecoil, (void *)(PBYTE)"\x90\x90\x90\x90", 4);
			Mem[0] = true;
		}
	}
	else
	{
		if (Mem[0])
		{
			tools->MemCopy((void *)NoRecoil, (void *)(PBYTE)"\xD9\x44\x24\x14", 4);
			Mem[0] = false;
		}
	}

	if ((*(BYTE *)GameStatus == 1) && Mems[1])
	{
		if (!Mem[1])
		{
			tools->MemCopy((void *)WeaponRange1, (void *)(PBYTE *)"\xB8\x00\x00\x00\x10\x90", 6);
			tools->MemCopy((void *)WeaponRange2, (void *)(PBYTE *)"\xB8\x00\x00\x00\x10\x90", 6);
			Mem[1] = true;
		}
	}
	else
	{
		if (Mem[1])
		{
			tools->MemCopy((void *)WeaponRange1, (void *)(PBYTE *)"\x8B\x80\xD4\x02\x00\x00", 6);
			tools->MemCopy((void *)WeaponRange2, (void *)(PBYTE *)"\x8B\x80\xD4\x02\x00\x00", 6);
			Mem[1] = false;
		}
	}

	if ((*(BYTE *)GameStatus == 1) && Mems[2])
	{
		if (!Mem[2])
		{
			tools->MemCopy((void *)scanVars.dwSuperbullets, (void *)(PBYTE)"\x90\x90\x90", 3);
			Mem[2] = true;
		}
	}
	else
	{
		if (Mem[2])
		{
			tools->MemCopy((void *)scanVars.dwSuperbullets, (void *)(PBYTE)"\x0F\x94\xC0", 3);
			Mem[2] = false;
		}
	}

	if ((*(BYTE *)GameStatus == 1) && Mems[3])
	{
		if (!Mem[3])
		{
			if (GetAsyncKeyState(VK_LBUTTON)<0)
			{

				tools->MemCopy((void*)RapidFire, (void *)(PBYTE *)"\x90\x90", 2);
				Mem[3] = true;

			}
			else
				tools->MemCopy((void*)RapidFire, (void *)(PBYTE *)"\x74\x3E", 2);
			Mem[3] = false;

		}
	}
	else
	{
		if (Mem[3])
		{
			tools->MemCopy((void*)RapidFire, (void *)(PBYTE *)"\x74\x3E", 2);
			Mem[3] = false;
		}
	}

	if ((*(BYTE *)GameStatus == 1) && Mems[4])
	{
		if (!Mem[4])
		{
			tools->MemCopy((void *)scanVars.dwNameTags1, (void *)(PBYTE)"\x90\x90", 2);
			tools->MemCopy((void *)scanVars.dwNameTags2, (void *)(PBYTE)"\x90\x90", 2);
			Mem[4] = true;
		}
	}
	else
	{
		if (Mem[4])
		{
			tools->MemCopy((void *)scanVars.dwNameTags1, (void *)(PBYTE)"\x75\x05", 2);
			tools->MemCopy((void *)scanVars.dwNameTags2, (void *)(PBYTE)"\x75\x05", 2);
			Mem[4] = false;
		}
	}

	if ((*(BYTE *)GameStatus == 1) && Mems[5])

	{
		if (!Mem[5])
		{
			tools->MemCopy((void *)NoReload, (void *)(PBYTE)"\x90\x90\x90\x90\x90\x90", 6);
			Mem[5] = true;
		}
	}
	else
	{
		if (Mem[5])
		{
			tools->MemCopy((void *)NoReload, (void *)(PBYTE)"\x0F\x84\xB1\x01\x00\x00", 6);
			Mem[5] = false;
		}
	}

	if ((*(BYTE *)GameStatus == 1) && Mems[6]) // Acea
	{
		if (*(long*)ADDR_CAMERA == 1)
			*(long*)ADDR_CAMERA = -5;
	}
	else {
		if (*(long*)ADDR_CAMERA == -5)
			*(long*)ADDR_CAMERA = 1;
	}


}




void HackClass::InitConsoleHacks(LPDIRECT3DDEVICE9 pDevice)
{
	if (!ConsoleTimer.Running())
	{
		if (Consoles[0])
		{
			if (!Console[0])
			{
				tools->PushToConsole(ShowFPSON);
				Console[0] = true;
			}
		}
		else
		{
			if (Console[0])
			{
				tools->PushToConsole(ShowFPSOFF);
				Console[0] = false;
			}
		}


		if (Consoles[1] && (*(BYTE *)GameStatus == 1))
		{
			if (!Console[1])
			{
				tools->PushToConsole(NXCHAMSON);
				Console[1] = true;
			}
		}
		else
		{
			if (Console[1])
			{
				tools->PushToConsole(NXCHAMSOFF);
				Console[1] = false;
			}
		}



		if (Consoles[2] && (*(BYTE *)GameStatus == 1))
		{
			if (!Console[2])
			{
				tools->PushToConsole(NOFOGON);
				Console[2] = true;
			}
		}
		else
		{
			if (Console[2])
			{
				tools->PushToConsole(NOFOGOFF);
				Console[2] = false;
			}
		}

		if ((*(BYTE *)GameStatus == 1) && Consoles[3])
		{
			if (!Console[3])
			{
				tools->PushToConsole(NOSPREAD1);
				tools->PushToConsole(NOSPREAD2);
				tools->PushToConsole(NOSPREAD3);
				tools->PushToConsole(NOSPREAD4);
				tools->PushToConsole(NOSPREAD5);
				tools->PushToConsole(NOSPREAD6);
				tools->PushToConsole(NOSPREAD7);
				tools->PushToConsole(NOSPREAD8);
				tools->PushToConsole(NOSPREAD9);
				Console[3] = true;
			}
		}
		else
		{
			if (Console[3])
			{
				tools->PushToConsole(NOSPREADOFF1);
				tools->PushToConsole(NOSPREADOFF2);
				tools->PushToConsole(NOSPREADOFF3);
				tools->PushToConsole(NOSPREADOFF4);
				tools->PushToConsole(NOSPREADOFF5);
				tools->PushToConsole(NOSPREADOFF6);
				tools->PushToConsole(NOSPREADOFF7);
				tools->PushToConsole(NOSPREADOFF8);
				tools->PushToConsole(NOSPREADOFF9);
				Console[3] = false;
			}
		}


		switch ((*(BYTE *)GameStatus == 1) && Consoles[4])
		{
		case 0:
			tools->PushToConsole("BaseMoveAccel 3000.000000");    //normal speed "[Off]"
			tools->PushToConsole("StartAccel 500.000000");
			tools->PushToConsole("MaxAccel 3000.000000");
			tools->PushToConsole("AccelInc 6000.000000");
			tools->PushToConsole("WalkVel 70.000000");
			tools->PushToConsole("FRunVel 285.000000");
			tools->PushToConsole("BRunVel 285.000000");
			tools->PushToConsole("SRunVel 285.000000");
			tools->PushToConsole("JumpVel 330.000000");
			tools->PushToConsole("DuckVel 50.000000");
			break;
		case 1:
			tools->PushToConsole("BaseMoveAccel 3000.000000");    //Slow Speed
			tools->PushToConsole("StartAccel 1500.000000");
			tools->PushToConsole("MaxAccel 3000.000000");
			tools->PushToConsole("AccelInc 2000.000000");
			tools->PushToConsole("WalkVel 1000.000000");
			tools->PushToConsole("FRunVel 1000.000000");
			tools->PushToConsole("BRunVel 1000.000000");
			tools->PushToConsole("SRunVel 1000.000000");
			tools->PushToConsole("JumpVel 330.000000");
			tools->PushToConsole("DuckVel 1000.000000");
			break;
		case 2:
			tools->PushToConsole("BaseMoveAccel 3500.000000");    //Legit Speed
			tools->PushToConsole("StartAccel 2500.000000");
			tools->PushToConsole("MaxAccel 3000.000000");
			tools->PushToConsole("AccelInc 3500.000000");
			tools->PushToConsole("WalkVel 3000.000000");
			tools->PushToConsole("FRunVel 3000.000000");
			tools->PushToConsole("BRunVel 3000.000000");
			tools->PushToConsole("SRunVel 3000.000000");
			tools->PushToConsole("JumpVel 330.000000");
			tools->PushToConsole("DuckVel 3000.000000");
			break;
		case 3:
			tools->PushToConsole("BaseMoveAccel 4000.000000");    //Fast Speed
			tools->PushToConsole("StartAccel 3000.000000");
			tools->PushToConsole("MaxAccel 3000.000000");
			tools->PushToConsole("AccelInc 4500.000000");
			tools->PushToConsole("WalkVel 4000.000000");
			tools->PushToConsole("FRunVel 4000.000000");
			tools->PushToConsole("BRunVel 4000.000000");
			tools->PushToConsole("SRunVel 4000.000000");
			tools->PushToConsole("JumpVel 330.000000");
			tools->PushToConsole("DuckVel 4000.000000");
			break;
		case 4:
			tools->PushToConsole("BaseMoveAccel 6000.000000");    //Faster Speed
			tools->PushToConsole("StartAccel 6000.000000");
			tools->PushToConsole("MaxAccel 6000.000000");
			tools->PushToConsole("AccelInc 6500.000000");
			tools->PushToConsole("WalkVel 6000.000000");
			tools->PushToConsole("FRunVel 6000.000000");
			tools->PushToConsole("BRunVel 6000.000000");
			tools->PushToConsole("SRunVel 6000.000000");
			tools->PushToConsole("JumpVel 330.000000");
			tools->PushToConsole("DuckVel 6000.000000");
			break;
		case 5:
			tools->PushToConsole("BaseMoveAccel 8000.000000");    //Fastest Speed
			tools->PushToConsole("StartAccel 8000.000000");
			tools->PushToConsole("MaxAccel 8000.000000");
			tools->PushToConsole("AccelInc 8500.000000");
			tools->PushToConsole("WalkVel 8000.000000");
			tools->PushToConsole("FRunVel 8000.000000");
			tools->PushToConsole("BRunVel 8000.000000");
			tools->PushToConsole("SRunVel 8000.000000");
			tools->PushToConsole("JumpVel 330.000000");
			tools->PushToConsole("DuckVel 8000.000000");
			break;
		}

		switch ((*(BYTE *)GameStatus == 1) && Consoles[5])
		{
		case 1:
			tools->PushToConsole("JumpVel 750.000000"); // Low 
			break;
		case 2:
			tools->PushToConsole("JumpVel 900.000000"); // High
			break;
		case 3:
			tools->PushToConsole("JumpVel 1250.000000"); // Higher
			break;
		case 4:
			tools->PushToConsole("JumpVel 1600.000000"); // Highest
			break;
		case 5:
			tools->PushToConsole("JumpVel 2000.000000"); // Super Jump
			break;
		}



		if ((*(BYTE *)GameStatus == 1) && Consoles[6])
		{
			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
				tools->PushToConsole(FLYON);
			else
				tools->PushToConsole(FLYOFF);


		}


		if ((*(BYTE *)GameStatus == 1) && Consoles[7])
		{
			if (!Console[7])
			{
				tools->PushToConsole(SUICIDEON);
				Console[7] = true;
			}
		}
		else
		{
			if (Console[7])
			{
				tools->PushToConsole(SUICIDEOFF);
				Console[7] = false;
			}

		}

		if ((*(BYTE *)GameStatus == 1) && Consoles[8])
		{
			if (!Console[8])
			{
				tools->PushToConsole(PICKUPON);
				Console[8] = true;
			}
		}
		else
		{
			if (Console[8])
			{
				tools->PushToConsole(PICKUPOFF);
				Console[8] = false;
			}
		}

		if ((*(BYTE *)GameStatus == 1) && Consoles[9])
		{
			if (!Console[9])
			{
				tools->PushToConsole(TRACERSON);
				Console[9] = true;
			}
		}
		else
		{
			if (Console[9])
			{
				tools->PushToConsole(TRACERSOFF);
				Console[9] = false;
			}
		}

		if ((*(BYTE *)GameStatus == 1) && Consoles[10])
		{
			if (!Console[10])
			{
				tools->PushToConsole(WIREPPLON);
				Console[10] = true;
			}
		}
		else
		{
			if (Console[10])
			{
				tools->PushToConsole(WIREPPLOFF);
				Console[10] = false;
			}

		}

		if ((*(BYTE *)GameStatus == 1) && Consoles[11])
		{
			if (!Console[11])
			{
				tools->PushToConsole(NOCAMDAMON);
				Console[11] = true;
			}
		}
		else
		{
			if (Console[11])
			{
				tools->PushToConsole(NOCAMDAMOFF);
				Console[11] = false;
			}
		}

		if ((*(BYTE *)GameStatus == 1) && Consoles[12])
		{
			if (!Console[12])
			{
				tools->PushToConsole(HOVERON1);
				tools->PushToConsole(HOVERON2);
				Console[12] = true;
			}
		}
		else
		{
			if (Console[12])
			{
				tools->PushToConsole(HOVEROFF1);
				tools->PushToConsole(HOVEROFF2);
				Console[12] = false;
			}
		}

		if ((*(BYTE *)GameStatus == 1) && Consoles[13])
		{
			if (!Console[13])
			{
				tools->PushToConsole(WSWAYON);
				Console[13] = true;
			}
		}
		else
		{
			if (Console[13])
			{
				tools->PushToConsole(WSWAYOFF);
				Console[13] = false;
			}
		}
		
		ConsoleTimer.Start(400.0f); //333.0f | 222.0f (past ones used)
	}

}

void HackClass::miscHacks(LPDIRECT3DDEVICE9 pDevice)
{
	if (misc[0])
	{
		POINT MyCursor;
		GetCursorPos(&MyCursor);
		MenuPosX = MyCursor.x;
		MenuPosY = MyCursor.y;
		MenuPosy = MyCursor.y - 10;
	}

	if (misc[1])
		ExitProcess(EXIT_SUCCESS);
}

void HackClass::xHairHack(LPDIRECT3DDEVICE9 pDevice)
{
	switch (crossHair)
	{
	case 1:
		cMenu->CrossHair(pDevice, Red);
		break;
	case 2:
		cMenu->CrossHair(pDevice, Blue);
		break;
	case 3:
		cMenu->CrossHair(pDevice, Yellow);
		break;
	case 4:
		cMenu->CrossHair(pDevice, Green);
		break;
	case 5:
		cMenu->CrossHair(pDevice, Cyan);
		break;

	}

}

void HackClass::FPSCheck(char *str, char *format)
{
	FPSfCurrentTickCount = clock() * 0.001f;
	FPScounter++;

	if ((FPSfCurrentTickCount - FPSfLastTickCount) > 1.0f) {
		FPSfLastTickCount = FPSfCurrentTickCount;
		sprintf(str, format, FPScounter);
		FPScounter = 0;
	}


}

void HackClass::InitAllHacks(LPDIRECT3DDEVICE9 pDevice)
{
	this->InitMemoryHacks(pDevice);
	this->InitConsoleHacks(pDevice);
	this->miscHacks(pDevice);
	this->xHairHack(pDevice);
}
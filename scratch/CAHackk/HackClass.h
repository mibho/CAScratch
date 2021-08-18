#ifndef HACK_H_
#define HACK_H_

#include "d3d9includes.h"


class HackClass {
public:

	void InitMemoryHacks(LPDIRECT3DDEVICE9 pDevice);
	void InitConsoleHacks(LPDIRECT3DDEVICE9 pDevice);
	void miscHacks(LPDIRECT3DDEVICE9 pDevice);
	void xHairHack(LPDIRECT3DDEVICE9 pDevice);
	void FPSCheck(char *str, char *format);
	void InitAllHacks(LPDIRECT3DDEVICE9 pDevice);

};

#endif

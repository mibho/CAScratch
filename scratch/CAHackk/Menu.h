#ifndef MENU_H_
#define MENU_H_

#include "d3d9includes.h"

enum Gr_Orientation //Shadow` && USSR's (Check DrawGradientRect for their funct)
{
	horizontal,
	vertical
};

class menuFunct;

class menuFunct {
public:

	void FillRGB(int x, int y, int w, int h, D3DCOLOR Color, LPDIRECT3DDEVICE9 pDevice);
	void DrawBorder(int x, int y, int w, int h, int px, D3DCOLOR BorderColor, LPDIRECT3DDEVICE9 pDevice);
	void DrawText(LPDIRECT3DDEVICE9 pDevice, int x, int y, DWORD Color, char *Text);
	void DrawCenterText(LPDIRECT3DDEVICE9 pDevice, int x, int y, DWORD Color, char *Text);
	void DrawRect(LPDIRECT3DDEVICE9 pDevice, int x, int y, int h, int w, DWORD Color);
	void CrossHair(LPDIRECT3DDEVICE9 pDevice, D3DCOLOR Color);
	void showMenu(LPDIRECT3DDEVICE9 pDevice);
	void DrawGradientRect(float x, float y, float w, float h, DWORD ColorStart, DWORD ColorEnd, Gr_Orientation Orientation, LPDIRECT3DDEVICE9 pDevice);
	void Additem(char* hackName, int MaxValue, int &hackVariable, int Kind, char** Choice, LPDIRECT3DDEVICE9 pDevice);
	void ControlFont(LPDIRECT3DDEVICE9 pDevice);
	void WriteTextCenter(LPDIRECT3DDEVICE9 pDevice, INT x, INT y, DWORD color, CHAR *text);
	void FillRGB2(int x, int y, int w, int h, D3DCOLOR Color, LPDIRECT3DDEVICE9 pDevice);
};
#endif
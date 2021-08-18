#include "Menu.h"
#include "Colors.h"
#include "XorStr.h"
#include "EncryptedStrings.h"



LPD3DXFONT D3DpFont;
LPDIRECT3DDEVICE9 pDevice;
LPDIRECT3DDEVICE9 pDeviceFontBuffer;

menuFunct *cMenu;
extern LPD3DXLINE pLine;
//int ScreenWidth = GetSystemMetrics( SM_CXBORDER ); // 0 and 5


int MenuPosX = 30;
int MenuPosY = 70;
int MenuPosy = 63;

// DrawCheckBox Function (Created by merp.)
int cPosX;
int cPosY;
bool isTabClickedByMouse = false;
static int noVarValue = 0;
// functions necessary for this

//DrawMainTabs function (Created by merp.)
bool ;
bool bTab1Visual = false;
bool bTab2Weapon = false;
bool bTab3Player = false;
bool bTab4Others = false;
bool bTab1Check = false;
bool bIsMainTabClickedByMouse, bTab5VIP, bTab1Clicked, bTab2Clicked, bTab3Clicked, bTab4Clicked, bTab5Clicked;
int iMouseClick = 0;

//

extern INT Mems[30];
extern INT Consoles[20];
extern INT misc[5];
extern INT crossHair;

extern bool bFont;

// Menu stuff
char* menuToggle[] = { "[Off]", "[On]" };
char* crossHairOpt[] = { "[Off]", "[Red]", "[Blue]", "[Yellow]", "[Green]", "[Cyan]" };
char* speedToggle[] = { "[Off]", "[Slow]", "[Legit]", "[Fast]", "[Faster]", "[Fastest]", "[Super Speed]" };
char* jumpToggle[] = { "[Off]", "[Low]", "[High]", "[Higher]", "[Highest]", "[Super Jump]" };
char* tabToggle[] = { "[+]", "[-]" };
char* BCtabToggle[] = { ">>", "<<" };
char* timeToggle[] = { "[Off]", "[On]" };
char* fireToggle[] = { "[Off]", "[On]", "[Knife]" };
char* testtab[] = { "[Off],", "[On]" };
char* nVToggle[] = { "[Off]", "[Vision 1]", "[Vision 2]", "[Vision 3]" };
char* ESPToggle[] = { "[Off]", "[NameTags]", "[Distance]", "[Lines]", "[2D Boxes]" };
char* DIPStuff[] = { "[Off]", "[Test1]", "[Test2]", "[Test3]", "[Test4]" };
char* aimStuff[] = { "[Off]", "[Head]", "[Neck]", "[Chest]", "[Test4]", "[Test5]" };
/****************************************/

//Menu Control stuff**************************/
int Current = true;
int hackSelect;
int ON = 0;

/**********************************/

INT Tab[10];
char sFPS[20] = "xxx Fps";
bool menuVisible = true;

D3DVIEWPORT9 Viewport;
DWORD timeX = Viewport.Width / 2;
DWORD timeY = Viewport.Height / 2 - 50;
DWORD fontX = Viewport.Width / 2;
DWORD fontY = Viewport.Height / 2;




VOID menuFunct::WriteTextCenter(LPDIRECT3DDEVICE9 pDevice, INT x, INT y, DWORD color, CHAR *text)
{
	RECT rect;
	SetRect(&rect, x - 5, y, x - 5, y);
	D3DpFont->DrawText(NULL, text, -1, &rect, DT_NOCLIP | DT_CENTER, color);
}

void menuFunct::FillRGB(int x, int y, int w, int h, D3DCOLOR Color, LPDIRECT3DDEVICE9 pDevice)
{
	D3DRECT rec = { x, y, x + w, y + h };
	pDevice->Clear(1, &rec, D3DCLEAR_TARGET, Color, 0, 0);
}

void menuFunct::FillRGB2(int x, int y, int w, int h, D3DCOLOR Color, LPDIRECT3DDEVICE9 pDevice)
{
	const DWORD D3D_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
	struct Vertex
	{
		float x, y, z, ht;
		DWORD yourcolor;
	}

	V[4] =
	{
		{ (float)x,(float)(y + h), 0.0f, 0.0f, Color },
		{ (float)x,(float)y, 0.0f, 0.0f, Color },
		{ (float)(x + w),(float)(y + h), 0.0f, 0.0f, Color },
		{ (float)(x + w),(float)y, 0.0f, 0.0f, Color }
	};
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_FOGENABLE, false);
	pDevice->SetFVF(D3D_FVF);
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, V, sizeof(Vertex));
}

void menuFunct::DrawBorder(int x, int y, int w, int h, int px, D3DCOLOR BorderColor, LPDIRECT3DDEVICE9 pDevice)
{
	FillRGB(x, (y + h - px), w, px, BorderColor, pDevice);
	FillRGB(x, y, px, h, BorderColor, pDevice);
	FillRGB(x, y, w, px, BorderColor, pDevice);
	FillRGB(x + w - px, y, px, h, BorderColor, pDevice);
}

BOOL isMouseinRange(int x, int y, int x2, int y2)
{
	POINT TabCursor;
	GetCursorPos(&TabCursor);
	cPosX = TabCursor.x;
	cPosY = TabCursor.y;
	if (cPosX > x && cPosX < x2 && cPosY > y && cPosY < y2)
	{
		return true;
	}
	else
	{
		return false;

	}
}

void menuFunct::DrawText(LPDIRECT3DDEVICE9 pDevice, int x, int y, DWORD Color, char *Text)
{
	RECT rect;
	SetRect(&rect, x, y, x, y);
	D3DpFont->DrawText(NULL, Text, -1, &rect, DT_LEFT | DT_NOCLIP, Color);
}

void menuFunct::DrawCenterText(LPDIRECT3DDEVICE9 pDevice, int x, int y, DWORD Color, char *Text)
{
	RECT rect;
	SetRect(&rect, x, y, x, y);
	D3DpFont->DrawText(NULL, Text, -1, &rect, DT_CENTER | DT_NOCLIP, Color);
}

// Shadow` && USSR's function. Learned alot from their base, and loved how the box was drawn. Thanks to both of them. :)
void menuFunct::DrawGradientRect(float x, float y, float w, float h, DWORD ColorStart, DWORD ColorEnd, Gr_Orientation Orientation, LPDIRECT3DDEVICE9 pDevice)
{
	struct D3DVERTEX { float x, y, z, rhw; DWORD color; }
	vertices[4] = { { 0, 0, 0, 1.0f, 0 },{ 0, 0, 0, 1.0f, 0 },{ 0, 0, 0, 1.0f, 0 },{ 0, 0 ,0,1.0f, 0 } };

	vertices[0].x = x;
	vertices[0].y = y;
	vertices[0].color = ColorStart;

	vertices[1].x = x + w;
	vertices[1].y = y;
	vertices[1].color = Orientation == vertical ? ColorEnd : ColorStart;

	vertices[2].x = x;
	vertices[2].y = y + h;
	vertices[2].color = Orientation == vertical ? ColorStart : ColorEnd;

	vertices[3].x = x + w;
	vertices[3].y = y + h;
	vertices[3].color = ColorEnd;

	pDevice->SetTexture(false, false);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)8);
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(D3DVERTEX));

	this->DrawBorder(x, y, w, h, 1, Black, pDevice);
}

void menuFunct::DrawRect(LPDIRECT3DDEVICE9 pDevice, int x, int y, int h, int w, DWORD Color)
{
	D3DRECT rec;
	rec.x1 = x;
	rec.x2 = x + w;
	rec.y1 = y;
	rec.y2 = y + h;
	pDevice->Clear(1, &rec, D3DCLEAR_TARGET, Color, 0, 0);
}

void menuFunct::CrossHair(LPDIRECT3DDEVICE9 pDevice, D3DCOLOR Color)
{
	D3DVIEWPORT9 Viewport;
	pDevice->GetViewport(&Viewport);
	DWORD ScreenX = Viewport.Width / 2;
	DWORD ScreenY = Viewport.Height / 2;
	D3DRECT rec1 = { ScreenX - 15, ScreenY, ScreenX + 15, ScreenY + 2 };
	D3DRECT rec2 = { ScreenX, ScreenY - 15, ScreenX + 2, ScreenY + 15 };
	pDevice->Clear(1, &rec1, D3DCLEAR_TARGET, Color, 0, 0);
	pDevice->Clear(1, &rec2, D3DCLEAR_TARGET, Color, 0, 0);
}

void menuFunct::Additem(char* hackName, int MaxValue, int &hackVariable, int Kind, char** Choice, LPDIRECT3DDEVICE9 pDevice)
{
	if (menuVisible) {
		if (hackSelect == Current)
		{
			if (GetAsyncKeyState(VK_RIGHT) & 1) {
				if (hackVariable < MaxValue)
					hackVariable++;
			}
			else if (GetAsyncKeyState(VK_LEFT) & 1) {
				if (hackVariable > 0)
					hackVariable--;
			}
		}
		if (hackSelect == Current) {
			this->DrawText(pDevice, MenuPosX - 5, MenuPosy + (Current * 15), BCBlue2, hackName); // -5 + (Current *15)
			this->DrawText(pDevice, MenuPosX + 120, MenuPosy + (Current * 15), BCBlue2, Choice[hackVariable]);
		}
		else if (hackVariable > 0)
		{
			if (Kind == 1){}
			else
			{
				this->DrawText(pDevice, MenuPosX - 5, MenuPosy + (Current * 15), BCBlue2, hackName);
				this->DrawText(pDevice, MenuPosX + 120, MenuPosy + (Current * 15), BCBlue2, Choice[hackVariable]);
			}
		}
		else
		{
			if (Kind == 1){}
			else {
				this->DrawText(pDevice, MenuPosX - 5, MenuPosy + (Current * 15), BCGrey2, hackName);
				this->DrawText(pDevice, MenuPosX + 120, MenuPosy + (Current * 15), BCGrey2, Choice[hackVariable]);
			}
		}
		Current++;
	}
}




void menuFunct::showMenu(LPDIRECT3DDEVICE9 pDevice)
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
		menuVisible = !menuVisible;

		this->DrawText(pDevice, fontX + 72, fontY + 30, BCBlue2, sFPS);//D3D FPS
		this->DrawText(pDevice, fontX + 25, fontY + 30, BCBlue2, "D3D FPS: ");// D3D FPS
		this->DrawText(pDevice, fontX + 6, fontY, BCBlue2, WelcomeTitle);// Welcome Message. PosX + 13
		this->DrawGradientRect(0, 0, 2500, 20, BCGrey4, lGray, horizontal, pDevice); // Welcome Message Box


	if (menuVisible)
	{
		//this->DrawBorder(ScreenPosX, MenuPosY - 22, 200, 300, 1, BCGrey, pDevice); 
		this->DrawGradientRect(MenuPosX - 10, MenuPosY - 22, 200, 20, BCGrey, lGray, horizontal, pDevice);//Draws box for the title // not 35 but 20

		this->DrawText(pDevice, MenuPosX + 35, MenuPosY - 20, BCBlue2, menuTitle);// Title hack. PosX + 13

		this->DrawGradientRect(MenuPosX - 10, MenuPosY, 200, (Current * 15), BCGrey4, lGray, horizontal, pDevice); // Main Draw Box Color MenuPosX - 10
		this->DrawGradientRect(MenuPosX - 10, MenuPosy + 2 + (hackSelect * 15), 200, 13, lBlack, TBlack, horizontal, pDevice); // Selection box's color
		Current = 1;

		if (GetAsyncKeyState(VK_UP) & 1)
			hackSelect--;

		if (GetAsyncKeyState(VK_DOWN) & 1)
			hackSelect++;

		this->Additem("Shitty Aimbot", 1, Tab[5], NULL, tabToggle, pDevice);
		if (Tab[5])
		{
			this->Additem("Shit Aimbot [Alt Key]", 3, Mems[21], NULL, aimStuff, pDevice);
			this->Additem("Auto Fire", 1, misc[2], NULL, menuToggle, pDevice);
		}
		this->Additem(subMenu1, 1, Tab[0], NULL, tabToggle, pDevice);
		if (Tab[0])
		{
			this->Additem(showFPS, 1, Consoles[0], NULL, menuToggle, pDevice);
			this->Additem(demNxChams, 1, Consoles[1], NULL, menuToggle, pDevice);
			this->Additem(xcrossHair, 5, crossHair, NULL, crossHairOpt, pDevice);
			this->Additem(nTags, 1, Mems[4], NULL, menuToggle, pDevice);
			this->Additem(tracers, 1, Consoles[9], NULL, menuToggle, pDevice);
			this->Additem(wirePpl, 1, Consoles[10], NULL, menuToggle, pDevice);
			this->Additem(noCamDam, 1, Consoles[11], NULL, menuToggle, pDevice);
			this->Additem("ESP NameTags", 1, Mems[17], NULL, menuToggle, pDevice);
			this->Additem("ESP Distance", 1, Mems[18], NULL, menuToggle, pDevice);
			this->Additem("ESP Lines", 1, Mems[19], NULL, menuToggle, pDevice);
			this->Additem("ESP 2D Boxes", 1, Mems[20], NULL, menuToggle, pDevice);
		}


		this->Additem(subMenu2, 1, Tab[1], NULL, tabToggle, pDevice);
		if (Tab[1])
		{
			this->Additem(noRecoil, 1, Mems[0], NULL, menuToggle, pDevice);
			this->Additem(noSpread, 1, Consoles[3], NULL, menuToggle, pDevice);
			this->Additem(wRange, 1, Mems[1], NULL, menuToggle, pDevice);
			this->Additem(sbullets, 1, Mems[2], NULL, menuToggle, pDevice);
			this->Additem(rFire, 2, Mems[3], NULL, fireToggle, pDevice);
			this->Additem(noSway, 1, Consoles[13], NULL, menuToggle, pDevice);
			this->Additem(noReload, 1, Mems[5], NULL, menuToggle, pDevice);

		}

		this->Additem(subMenu3, 1, Tab[2], NULL, tabToggle, pDevice);
		if (Tab[2])
		{
			this->Additem(speedh4x, 6, Consoles[4], NULL, speedToggle, pDevice);
			this->Additem(superjmp, 5, Consoles[5], NULL, jumpToggle, pDevice);
			this->Additem(flyh4x, 1, Consoles[6], NULL, menuToggle, pDevice);
			this->Additem(hover, 1, Consoles[12], NULL, menuToggle, pDevice);
		}

		this->Additem(subMenu4, 1, Tab[3], NULL, tabToggle, pDevice);
		if (Tab[3])
		{
			this->Additem(pickup, 1, Consoles[8], NULL, menuToggle, pDevice);
			this->Additem(moveMenu, 1, misc[0], NULL, menuToggle, pDevice);
			this->Additem(suicide, 1, Consoles[7], NULL, menuToggle, pDevice);
			this->Additem(noFog, 1, Consoles[2], NULL, menuToggle, pDevice);
		}

		this->Additem(subMenu5, 1, Tab[4], NULL, tabToggle, pDevice);
		if (Tab[4])
		{
			this->Additem(teleKill, 1, Mems[14], NULL, menuToggle, pDevice);
			this->Additem(invisible, 1, Mems[6], NULL, menuToggle, pDevice);
			this->Additem(RageQuit, 1, misc[1], NULL, menuToggle, pDevice);

		}

		if (hackSelect >= Current)
			hackSelect = 1;

		else if (hackSelect < 1)
			hackSelect = Current;
	}
}


void menuFunct::ControlFont(LPDIRECT3DDEVICE9 pDevice)
{
	if (D3DpFont == NULL)
		D3DXCreateFont(pDevice, 16, 6, 500, 1, 0, 1, 0, 4, 0 | (0 << 4), "Calibri", &D3DpFont);
	else
	{
		if (pDeviceFontBuffer != pDevice)
		{
			pDeviceFontBuffer = pDevice;
		    try
			{
				if (D3DpFont != 0)
					D3DpFont->Release();
				pLine->Release();
			}
			catch (...) {}
			pLine = NULL;
			D3DpFont = 0;
			D3DXCreateFont(pDevice, 16, 6, 500, 1, 0, 1, 0, 4, 0 | (0 << 4), "Calibri", &D3DpFont);
		}
	}
}
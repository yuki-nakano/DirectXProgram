#ifndef DIRECT_GRAPHICS_H
#define DIRECT_GRAPHICS_H

#include <d3d9.h>
#include <d3dx9.h>

struct CustomVertex
{
	float X;
	float Y;
	float Z;
	float Rhw;
	DWORD Color;
};

enum colorPattern
{
	black,
	white,
	red,
	green,
	blue
};

bool InitDirectGraphics(HWND window_handle);

void ReleseDirectGraphics();

void StartDrowing();

void FinishDrowing();

void DrawTriangle();

void DrawRect(float posX, float posY, float sizeX, float sizeY, colorPattern colorpattern);
DWORD colorCheck(colorPattern colorpattern);


#endif DIRECT_GRAPHICS_H
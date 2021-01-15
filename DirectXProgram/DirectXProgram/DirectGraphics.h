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

struct CustomVertex_tex
{
	float X;
	float Y;
	float Z;
	float Rhw;
	DWORD Color;

	float Tu;
	float Tv;
};

enum colorPattern
{
	black,
	white,
	red,
	green,
	blue
};

struct CustomVertex3D
{
	float X;
	float Y;
	float Z;

	DWORD Color;
};

enum vec3D
{
	X,
	Y,
	Z
};

// FVF設定(2D用テクスチャ無し)
#define FVF_2D_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
// FVF設定(2D用テクスチャ有り)
#define FVF_2D_VERTEX_TEXTURE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// FVF設定(3D用)
#define FVF_3D_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

enum TextureID
{
	TexIDusa,
	TexIDMAX
};

enum XFileDate
{
	box,

	XFileDateMax,
};


bool InitDirectGraphics(HWND window_handle);

void ReleseDirectGraphics();

void StartDrawing();

void FinishDrawing();

void DrawTriangle();

void DrawRect(float posX, float posY, float sizeX, float sizeY, colorPattern colorpattern);
DWORD colorCheck(colorPattern colorpattern);

void DrawRectTexture(TextureID tex_id);

void SetUpView();

void SetUpProjection();

void Draw3DPorigon(float posx_, float posy_, float posz_, colorPattern colorpattern);

bool LoadXFile(int fileName_);

void DrawXFile(float theta[], float pos[], float scale[], XFileDate xfiledate_);

bool LoadTexture(TextureID texid);
void ReleaseTexture();

void ReleaseXFile(int fileName_);

void UpdateCamera(float posx_, float posz_, float theta_);

void Move(float* theta_, float* posx_, float* posz_);

#endif DIRECT_GRAPHICS_H
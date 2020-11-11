#include "DirectGraphics.h"

LPDIRECT3D9 g_Interface = nullptr;
LPDIRECT3DDEVICE9 g_Device = nullptr;

LPDIRECT3DTEXTURE9 g_Textures[TextureID::TexIDMAX];
LPCWSTR g_TextureNameList[] =
{
	TEXT("res/9549.jpg")
};

bool InitDirectGraphics(HWND window_handle)
{
	g_Interface = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_Interface == nullptr)
	{
		return false;
	}

	//Deviceの情報を設定する
	D3DPRESENT_PARAMETERS parameters;

	//指定されたデータをサイズ分だけ0で初期化する
	ZeroMemory(&parameters, sizeof(D3DPRESENT_PARAMETERS));

	//バックバッファーの数
	parameters.BackBufferCount = 1;
	// バックバッファのフォーマット
	parameters.BackBufferFormat = D3DFMT_UNKNOWN;
	//ウィンドウモードの指定
	parameters.Windowed = true;

	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	HRESULT result = g_Interface->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window_handle,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&parameters,
		&g_Device
	);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ReleseDirectGraphics()
{
	if (g_Device != nullptr)
	{
		//Release ⇒　開放関数
		g_Device->Release();
		//解放後の不正アクセス対応
		g_Device = nullptr;
	}

	if (g_Interface != nullptr)
	{
		//Release ⇒　開放関数
		g_Interface->Release();
		//解放後の不正アクセス対応
		g_Interface = nullptr;
	}
}

void StartDrowing()
{
	//バックバッファをクリア
	g_Device->Clear(
		0,
		nullptr,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(255, 255, 255),
		1.0f,
		0
	);

	//DirectXで描画開始
	g_Device->BeginScene();
}

void FinishDrowing()
{
	//DirectXの描画終了
	g_Device->EndScene();

	g_Device->Present(nullptr, nullptr, nullptr, nullptr);
}

void DrawTriangle()
{
	CustomVertex vertices[] =
	{
		{320.0f, 0.0f, 0.0f, 1.0f, 0x000000 },
		{640.0f, 480.0f, 0.0f, 1.0f, 0x000000 },
		{0.0f, 480.0f, 0.0f, 1.0f, 0x000000 },
	};

	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		1,
		vertices,
		sizeof(CustomVertex)
	);
}

void DrawRect(float posX, float posY, float sizeX, float sizeY, colorPattern colorpattern)
{
	DWORD Color = colorCheck(colorpattern);
	CustomVertex vertices[] =
	{
		{posX, posY, 0.0f, 1.0f, Color},
		{posX + sizeX, posY, 0.0f, 1.0f, Color},
		{posX, posY + sizeY, 0.0f, 1.0f, Color},
		{posX + sizeX, posY + sizeY, 0.0f, 1.0f, Color}
	};

	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2,
		vertices,
		sizeof(CustomVertex)
	);
}

void DrawRectTexture(TextureID tex_id)
{
	if (g_Textures[tex_id] == nullptr)
	{
		return;
	}

	CustomVertex vertices[] = {
		{0.0f, 0.0f, 0.0f, 1.0f, 0xffffff, 0.0f, 0.0f},
		{100.0f, 0.0f, 0.0f, 1.0f, 0xffffff, 1.0f, 0.0f},
		{100.0f, 100.0f, 0.0f, 1.0f, 0xffffff, 1.0f, 1.0f},
		{0.0f, 100.0f, 0.0f, 1.0f, 0xffffff, 0.0f, 1.0f},
	};

	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	g_Device->SetTexture(0, g_Textures[tex_id]);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		vertices,
		sizeof(CustomVertex)
	);
}

DWORD colorCheck(colorPattern colorpattern)
{
	switch (colorpattern)
	{
	case black:
		return 0x000000;
		break;
	case white:
		return 0xffffff;
		break;
	case red:
		return 0xff0000;
		break;
	case green:
		return 0x00ff00;
		break;
	case blue:
		return 0x0000ff;
		break;
	}
}

bool LoadTexture(TextureID tex_id)
{
	HRESULT hr = D3DXCreateTextureFromFile(
		g_Device,
		g_TextureNameList[tex_id],
		&g_Textures[tex_id]
	);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void ReleaseTexture()
{
	for (int i = 0; i < TextureID::TexIDMAX; i++)
	{
		if (g_Textures[i] != nullptr)
		{
			g_Textures[i]->Release();
			g_Textures[i] = nullptr;
		}
	}
}

#include "DirectGraphics.h"

LPDIRECT3D9 g_Interface = nullptr;
LPDIRECT3DDEVICE9 g_Device = nullptr;

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

﻿#include <Windows.h>
#include "DirectGraphics.h"
#include "DirectInput.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"dxguid.lib")

static float pos[3] = { 0.0f, 0.0f, 0.0f };
static float rad[3] = { 0.0f, 0.0f, 0.0f };
static float scale[3] = { 1.0f, 1.0f, 1.0f };



//ウィンドウプロシージャ
LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	switch (message_id)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	return 0;
}

//エントリポイント
int APIENTRY WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		LpCmpLine,
	INT			nCmdShow)
{
	//ウィンドウ情報の初期化
	WNDCLASS window_class =
	{
		CS_HREDRAW | CS_VREDRAW,
		WindowProcedure,
		0,
		0,
		hInstance,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		NULL,
		NULL,
		TEXT("WindowClass")
	};

	int width = 640;
	int height = 480;

	//ウィンドウ情報の登録
	if (RegisterClass(&window_class) == 0)
	{
		return 0;
	}

	//ウィンドウ作成
	HWND window_handle = CreateWindowA(
		"WindowClass",
		"test",
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		NULL,
		NULL,
		hInstance,
		NULL);

	RECT window_rect;

	if (GetWindowRect(window_handle, &window_rect) == false)
	{
		return 0;
	}

	RECT client_rect;

	if (GetClientRect(window_handle, &client_rect) == false)
	{
		return 0;
	}

	//フレームのサイズ算出
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	//リサイズ用サイズの算出
	int resize_width = frame_size_x + width;
	int resize_height = frame_size_y + height;

	//ウィンドウのリサイズ
	SetWindowPos(
		window_handle,
		nullptr,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		resize_width,
		resize_height,
		SWP_NOMOVE
	);

	//ウィンドウの表示
	ShowWindow(window_handle, SW_SHOW);

	if (InitDirectGraphics(window_handle) == false)
	{
		return 0;
	}

	if (LoadTexture(TextureID::TexIDusa) == false)
	{
		return 0;
	}

	if (InitDirectInput() == false)
	{
		return 0;
	}

	for (int i = 0; i < XFileDateMax; i++)
	{
		LoadXFile(i);
	}
	
	//メインループ
	while (true)
	{
		MSG message;

		//Windowsからのメッセージの対応
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//受信したデータの翻訳
				TranslateMessage(&message);

				//ウィンドウプロシージャに送信
				DispatchMessage(&message);
			}
		}
		else
		{
			//キーの状態を更新
			UpdateDirectInput();

			Move(&rad[Y], &pos[X], &pos[Z]);

			//カメラの設定
			SetUpView();

			UpdateCamera(pos[X], pos[Z], rad[Y]);

			SetUpProjection();

			StartDrawing();

			//DrawTriangle();

			//DrawRect(100.0f, 150.0f, 200.0f, 150.0f, blue);

			//DrawRectTexture(TextureID::TexIDusa);

			DrawXFile(rad, pos, scale, box);

			FinishDrawing();
		}
	}

	//解放
	ReleaseTexture();
	ReleasDirectInput();
	ReleseDirectGraphics();
	for (int i = 0; i < XFileDateMax; i++)
	{
		ReleaseXFile(i);
	}

	return 0;
}
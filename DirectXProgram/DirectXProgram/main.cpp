#include <Windows.h>

//ウィンドウプロシージャ
LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(window_handle, message_id, wparam, lparam);
}

//エントリポイント
int APIENTRY WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		LpCmpLine,
	INT			nCmdShow)
{
	//ウィンドウ情報の初期化
	WNDCLASSA window_class =
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
		"WindowClass"
	};

	//ウィンドウ情報の登録
	if (RegisterClassA(&window_class) == 0)
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

	//ウィンドウの表示
	ShowWindow(window_handle, SW_SHOW);

	//メインループ
	while (true)
	{
		MSG message;

		//Windowsからのメッセージの対応
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			//受信したデータの翻訳
			TranslateMessage(&message);

			//ウィンドウプロシージャに送信
			DispatchMessage(&message);
		}
		else
		{

		}
	}

	return 0;
}
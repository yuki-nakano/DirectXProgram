#include <Windows.h>
#include "DirectGraphics.h"

//�E�B���h�E�v���V�[�W��
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

//�G���g���|�C���g
int APIENTRY WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		LpCmpLine,
	INT			nCmdShow)
{
	//�E�B���h�E���̏�����
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

	//�E�B���h�E���̓o�^
	if (RegisterClass(&window_class) == 0)
	{
		return 0;
	}

	//�E�B���h�E�쐬
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

	//�t���[���̃T�C�Y�Z�o
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	//���T�C�Y�p�T�C�Y�̎Z�o
	int resize_width = frame_size_x + width;
	int resize_height = frame_size_y + height;

	//�E�B���h�E�̃��T�C�Y
	SetWindowPos(
		window_handle,
		nullptr,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		resize_width,
		resize_height,
		SWP_NOMOVE
	);

	//�E�B���h�E�̕\��
	ShowWindow(window_handle, SW_SHOW);

	if (InitDirectGraphics(window_handle) == false)
	{
		return 0;
	}

	//���C�����[�v
	while (true)
	{
		MSG message;

		//Windows����̃��b�Z�[�W�̑Ή�
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//��M�����f�[�^�̖|��
				TranslateMessage(&message);

				//�E�B���h�E�v���V�[�W���ɑ��M
				DispatchMessage(&message);
			}
		}
		else
		{

		}
	}

	//���
	ReleseDirectGraphics();

	return 0;
}
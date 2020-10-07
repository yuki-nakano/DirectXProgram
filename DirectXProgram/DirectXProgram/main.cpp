#include <Windows.h>

//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WindowProcedure(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(window_handle, message_id, wparam, lparam);
}

//�G���g���|�C���g
int APIENTRY WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		LpCmpLine,
	INT			nCmdShow)
{
	//�E�B���h�E���̏�����
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

	//�E�B���h�E���̓o�^
	if (RegisterClassA(&window_class) == 0)
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

	//�E�B���h�E�̕\��
	ShowWindow(window_handle, SW_SHOW);

	//���C�����[�v
	while (true)
	{
		MSG message;

		//Windows����̃��b�Z�[�W�̑Ή�
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			//��M�����f�[�^�̖|��
			TranslateMessage(&message);

			//�E�B���h�E�v���V�[�W���ɑ��M
			DispatchMessage(&message);
		}
		else
		{

		}
	}

	return 0;
}
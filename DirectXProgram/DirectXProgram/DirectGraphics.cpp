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

	//Device�̏���ݒ肷��
	D3DPRESENT_PARAMETERS parameters;

	//�w�肳�ꂽ�f�[�^���T�C�Y������0�ŏ���������
	ZeroMemory(&parameters, sizeof(D3DPRESENT_PARAMETERS));

	//�o�b�N�o�b�t�@�[�̐�
	parameters.BackBufferCount = 1;
	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g
	parameters.BackBufferFormat = D3DFMT_UNKNOWN;
	//�E�B���h�E���[�h�̎w��
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
		//Release �ˁ@�J���֐�
		g_Device->Release();
		//�����̕s���A�N�Z�X�Ή�
		g_Device = nullptr;
	}

	if (g_Interface != nullptr)
	{
		//Release �ˁ@�J���֐�
		g_Interface->Release();
		//�����̕s���A�N�Z�X�Ή�
		g_Interface = nullptr;
	}
}

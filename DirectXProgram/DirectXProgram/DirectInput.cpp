#include "DirectInput.h"

LPDIRECTINPUT8 g_InputInterface;
LPDIRECTINPUTDEVICE8 g_InputKeyboardDevice;

InputState g_KeyStates[256];

InputState UpdateInputState(bool is_push, InputState state)
{
	if (is_push == true)
	{
		if (state == InputState::NotHold)
		{
			return InputState::Pushed;
		}
		else if (state == InputState::Pushed ||
				 state == InputState::Held)
		{
			return InputState::Held;
		}
	}
	else
	{
		if (state == InputState::Pushed ||
			state == InputState::Held)
		{
			return InputState::Relesed;
		}
		else
		{
			return InputState::NotHold;
		}
	}

	return InputState::NotHold;
}

bool InitDirectInput()
{
	HRESULT hr = DirectInput8Create(
		GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_InputInterface,
		nullptr);

	if (FAILED(hr))
	{
		return false;
	}

	hr = g_InputInterface->CreateDevice(
		GUID_SysKeyboard,
		&g_InputKeyboardDevice,
		nullptr);

	if (FAILED(hr))
	{
		return false;
	}

	hr = g_InputKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	
	if (FAILED(hr))
	{
		return false;
	}

	hr = g_InputKeyboardDevice->SetCooperativeLevel(
		FindWindow(TEXT("WindowClass"), nullptr),
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	if (FAILED(hr))
	{
		return false;
	}

	hr = g_InputKeyboardDevice->Acquire();

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void UpdateDirectInput()
{
	BYTE key_state[256];

	HRESULT hr;

	hr = g_InputKeyboardDevice->GetDeviceState(256, key_state);

	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < 256; i++)
		{
			bool is_push = false;
			if (key_state[i] & 0x80)
			{
				is_push = true;
			}

			g_KeyStates[i] = UpdateInputState(is_push, g_KeyStates[i]);
		}
	}
}

void ReleasDirectInput()
{
	if (g_InputKeyboardDevice != nullptr)
	{
		g_InputKeyboardDevice->Unacquire();

		g_InputKeyboardDevice->Release();
		g_InputKeyboardDevice = nullptr;
	}

	if (g_InputInterface != nullptr)
	{
		g_InputInterface->Release();
		g_InputInterface = nullptr;
	}
}

bool IsHeldKey(int key)
{
	if (key < 0 || key >= 256)
	{
		return false;
	}

	return g_KeyStates[key] == InputState::Held ? true : false;
}

bool IsPushedKey(int key)
{
	if (key < 0 || key >= 256)
	{
		return false;
	}

	if (g_KeyStates[key] == InputState::Pushed)
	{
		return true;
	}

	return false;
}

bool IsRelesedKey(int key)
{
	if (key < 0 || key >= 256)
	{
		return false;
	}

	if (g_KeyStates[key] == InputState::Relesed)
	{
		return true;
	}

	return false;
}

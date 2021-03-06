#ifndef DIRECTINPUT_H
#define DIRECTINPUT_H

#include <dinput.h>

enum InputState
{
	NotHold,	//押されていない
	Pushed,		//押した
	Held,		//押している
	Relesed,	//離した
};

bool InitDirectInput();
void UpdateDirectInput();
void ReleasDirectInput();

bool IsHeldKey(int key);
bool IsPushedKey(int key);
bool IsRelesedKey(int key);

#endif // !DIRECTINPUT_H

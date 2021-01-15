#ifndef DIRECTINPUT_H
#define DIRECTINPUT_H

#include <dinput.h>

enum InputState
{
	NotHold,	//������Ă��Ȃ�
	Pushed,		//������
	Held,		//�����Ă���
	Relesed,	//������
};

bool InitDirectInput();
void UpdateDirectInput();
void ReleasDirectInput();

bool IsHeldKey(int key);
bool IsPushedKey(int key);
bool IsRelesedKey(int key);

#endif // !DIRECTINPUT_H

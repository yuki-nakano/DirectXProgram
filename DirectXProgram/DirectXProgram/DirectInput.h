#ifndef DIRECTINPUT_H
#define DIRECTINPUT_H

#include <dinput.h>

enum InputState
{
	NotHold,	//‰Ÿ‚³‚ê‚Ä‚¢‚È‚¢
	Pushed,		//‰Ÿ‚µ‚½
	Held,		//‰Ÿ‚µ‚Ä‚¢‚é
	Relesed,	//—£‚µ‚½
};

bool InitDirectInput();
void UpdateDirectInput();
void ReleasDirectInput();

bool IsHeldKey(int key);
bool IsPushedKey(int key);
bool IsRelesedKey(int key);

#endif // !DIRECTINPUT_H

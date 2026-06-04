#pragma once
#include "Define.h"

class InputManager
{
private:
	FKeyState KeyState;
public:
	void GetInput();
	void ClearInput();

	inline FKeyState GetKeyState() const { return KeyState; }
};


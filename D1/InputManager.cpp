#include "InputManager.h"


void InputManager::GetInput()
{
	bool isSpaceCurrentDown = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		KeyState.UpArrowDown = true;
	}
	if (GetAsyncKeyState(VK_UP) & 1)
	{
		KeyState.UpArrowDown = false;
	}


	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		KeyState.RightArrowDown = true;
	}
	else
	{
		KeyState.RightArrowDown = false;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		KeyState.DownArrowDown = true;
	}
	else
	{
		KeyState.DownArrowDown = false;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		KeyState.LeftArrowDown = true;
	}
	else
	{
		KeyState.LeftArrowDown = false;
	}

	if (isSpaceCurrentDown && !KeyState.KEYSpaceWasDown)
	{
		KeyState.KEYSpaceDown = true;
	}
	else
	{
		KeyState.KEYSpaceDown = false;
	}

	KeyState.KEYSpaceWasDown = isSpaceCurrentDown;
	if (GetAsyncKeyState('W') & 0x8001)	KeyState.KEYW = true;
	if (GetAsyncKeyState('D') & 0x8001)	KeyState.KEYD = true;
	if (GetAsyncKeyState('S') & 0x8001)	KeyState.KEYS = true;
	if (GetAsyncKeyState('A') & 0x8001)	KeyState.KEYA = true;
}

void InputManager::ClearInput()
{
	KeyState.KEYW = false;
	KeyState.KEYD = false;
	KeyState.KEYS = false;
	KeyState.KEYA = false;
}

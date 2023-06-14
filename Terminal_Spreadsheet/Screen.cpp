#include "Screen.h"

Screen::Screen(HANDLE newHandle) : handle(newHandle)
{
	GetConsoleScreenBufferInfo(newHandle, &initInfo);
}

CONSOLE_SCREEN_BUFFER_INFO Screen::Info() const
{
	CONSOLE_SCREEN_BUFFER_INFO currentInfo;
	GetConsoleScreenBufferInfo(handle, &currentInfo);
	return currentInfo;
}

COORD Screen::Size() const
{
	return Info().dwSize;
}

void Screen::ChangeSize(int width, int height)
{
	COORD size{ static_cast<short>(width), static_cast<short>(height) };
	SetConsoleScreenBufferSize(handle, size);
}
#pragma once
#include "windows.h"
class Screen
{
public:
	Screen() = default;
	Screen(HANDLE newHandle);
	CONSOLE_SCREEN_BUFFER_INFO Info() const;
	const CONSOLE_SCREEN_BUFFER_INFO& InitInfo() const { return initInfo; }
	COORD Size() const;
	void ChangeSize(int width, int height);
	HANDLE Handle() const { return handle; }
private:
	HANDLE handle{ nullptr };
	CONSOLE_SCREEN_BUFFER_INFO initInfo{};
};


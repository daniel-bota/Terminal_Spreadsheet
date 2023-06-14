#pragma once
#include "windows.h"
#include "Position.h"
#include "Direction.h"
#include <string>

class Console
{
public:
	static void WriteW(std::wstring text);
	static void Write(std::string text);
	static Position CurrentCursor();
	static CONSOLE_SCREEN_BUFFER_INFO ScreenInfo();
	static COORD ScreenSize();
	static void MoveCursor(Position position);
	static void MoveCursor(int x, int y);
	static void MoveCursor(Direction direction, int positions);
};
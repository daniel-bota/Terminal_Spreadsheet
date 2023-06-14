#include "Console.h"

void Console::WriteW(std::wstring text)
{
	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), text.c_str(), static_cast<long>(text.size()), NULL, NULL);
}

void Console::Write(std::string text)
{
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), text.c_str(), static_cast<long>(text.size()), NULL, NULL);
}

Position Console::CurrentCursor()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	return Position{ static_cast<int>(info.dwCursorPosition.X), static_cast<int>(info.dwCursorPosition.Y) };
}

CONSOLE_SCREEN_BUFFER_INFO Console::ScreenInfo()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	return info;
}

COORD Console::ScreenSize()
{
	return ScreenInfo().dwSize;
}

void Console::MoveCursor(Position position)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),
		COORD{ static_cast<short>(position.X()), static_cast<short>(position.Y()) });
}

void Console::MoveCursor(int x, int y)
{
	MoveCursor(Position{ x, y });
}

void Console::MoveCursor(Direction direction, int positions)
{
	if (direction & Right)
	{
		MoveCursor(CurrentCursor().X() + positions, CurrentCursor().Y());
	}
	if (direction & Left)
	{
		MoveCursor(CurrentCursor().X() - positions, CurrentCursor().Y());
	}
	if (direction & Down)
	{
		MoveCursor(CurrentCursor().X(), CurrentCursor().Y() + positions);
	}
	if (direction & Up)
	{
		MoveCursor(CurrentCursor().X(), CurrentCursor().Y() - positions);
	}
}

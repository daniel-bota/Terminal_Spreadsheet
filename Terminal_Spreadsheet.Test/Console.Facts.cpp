#include "pch.h"
#include "Console.h"

namespace ConsoleFacts
{
	TEST(Console, CanGetCurrentScreenSize)
	{
		HANDLE handle = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(handle, COORD(300, 200));
		SetConsoleActiveScreenBuffer(handle);
		SetStdHandle(STD_OUTPUT_HANDLE, handle);

		EXPECT_EQ(300, Console::ScreenSize().X);
		EXPECT_EQ(200, Console::ScreenSize().Y);
	}
}
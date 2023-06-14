#include "pch.h"
#include "Screen.h"

namespace ScreenFacts
{
	TEST(Screen, CanGetInitialInfo)
	{
		HANDLE handle = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(handle, COORD(200, 300));
		Screen screen(handle);
		const auto& info = screen.InitInfo();
		EXPECT_EQ(200, info.dwSize.X);
		EXPECT_EQ(300, info.dwSize.Y);
	}

	TEST(Screen, CanGetInfo)
	{
		HANDLE handle = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		Screen screen(handle);
		SetConsoleScreenBufferSize(screen.Handle(), COORD(200, 300));
		auto info = screen.Info();
		EXPECT_EQ(200, info.dwSize.X);
		EXPECT_EQ(300, info.dwSize.Y);
	}

	TEST(Screen, CanChangeSize)
	{
		HANDLE handle = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetStdHandle(STD_OUTPUT_HANDLE, handle);
		Screen screen(handle);
		screen.ChangeSize(200, 500);
		auto info = screen.Info();
		EXPECT_EQ(200, screen.Info().dwSize.X);
		EXPECT_EQ(500, screen.Info().dwSize.Y);
	}
}
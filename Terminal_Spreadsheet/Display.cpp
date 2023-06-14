#include "Display.h"

Display::Display(int displayHeight) : height(displayHeight)
{
	width = Console::ScreenInfo().dwSize.X;
}

void Display::Clear()
{
	auto width = Console::ScreenInfo().dwSize.X;
	Console::MoveCursor(cellInfo);

	for (int i = 0; i < height - 1; i++)
	{
		Console::Write(std::string(width, ' '));
	}
}

void Display::ShowCellInfo(const std::shared_ptr<Cell>& cell, bool edit)
{
	Clear();
	Console::MoveCursor(cellInfo);
	Console::Write(std::format("{}: ", cell->Title()));

	auto formCell = std::dynamic_pointer_cast<FormulaCell>(cell);
	std::string info;
	if (formCell)
	{
		info = edit ? formCell->Expression() : formCell->ValueString();
	}
	else
	{
		info = cell->ValueString();
	}

	if (edit)
	{
		TextInput();
	}

	Console::Write(info);
}

void Display::TextInput()
{
	Console::MoveCursor(0, cellInfo.Y() + 1);
}
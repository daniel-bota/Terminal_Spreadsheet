#include "TUI.h"

TUI::TUI(CComPtr<Excel::_Application> excelApp): excelApp(excelApp)
{
	InitMenuScreen();
	DisplayMenuInfo();

	InitSheetScreen();

	InitSheet();
	UserInput();
}

void TUI::InitMenuScreen(const Area& newScreenSize)
{
	HANDLE hMenu = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD conMode;
	GetConsoleMode(hMenu, &conMode);
	conMode |= ENABLE_WINDOW_INPUT;
	SetConsoleMode(hMenu, conMode);

	CONSOLE_CURSOR_INFO cInfo{ 1, TRUE };
	SetConsoleCursorInfo(hMenu, &cInfo);
	sMenu = Screen(hMenu);
	SetActiveScreen("menu");
	if (newScreenSize != Area{ -1, -1 })
	{
		short newWidth = newScreenSize.Width() >= 120 ? newScreenSize.Width() : 120;
		short newHeight = newScreenSize.Height() >= 30 ? newScreenSize.Height() : 30;
		SetConsoleScreenBufferSize(hMenu, COORD{newWidth, newHeight});
	}
	ChangeScreenSize(newScreenSize.X(), newScreenSize.Y());
}

void TUI::InitSheetScreen(const Area& newScreenSize)
{
	HANDLE hSheet = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	DWORD conMode;
	GetConsoleMode(hSheet, &conMode);
	conMode |= ENABLE_WINDOW_INPUT;
	SetConsoleMode(hSheet, conMode);

	if (newScreenSize != Area{ -1, -1 })
	{
		short newWidth = newScreenSize.Width() >= 120 ? newScreenSize.Width() : 120;
		short newHeight = newScreenSize.Height() >= 30 ? newScreenSize.Height() : 30;
		SetConsoleScreenBufferSize(hSheet, COORD{ newWidth, newHeight });
	}

	CONSOLE_CURSOR_INFO cInfo{ 1, TRUE };
	SetConsoleCursorInfo(hSheet, &cInfo);
	sSheet = Screen(hSheet);

	SetActiveScreen("sheet");

	std::string toggleKey = "Menu (ESC)";
	Console::MoveCursor(Console::ScreenSize().X - static_cast<int>(toggleKey.size()), 0);
	Console::Write(toggleKey);
}

void TUI::InitSheet
(const std::map<Address, std::shared_ptr<Cell>>& imported)
{
	std::map<Address, std::shared_ptr<Cell>> newValues = imported;
	displayH = maxValueLength / sSheet.Size().X + 4;
	display = Display(displayH);
	
	defRowHeaderW = defCellW / 2;

	sheet = Sheet(defCellH, defCellW, defRowHeaderW, maxValueLength, displayH);
	
	sheet.Initialize(newValues);

	auto buffered = sheet.GetBufferedCells();
	DrawHeaders(sheet.ColHeadersBuff());
	DrawHeaders(sheet.RowHeadersBuff());
	DrawCells(buffered);

	Console::MoveCursor(sheet.BufferAddressToPosition(Address{ 0, 0 }));
	HighlightCell(Console::CurrentCursor());
}

void TUI::ToggleActiveScreen()
{
	if (activeScreen == "menu")
	{
		SetActiveScreen("sheet");
		return;
	}

	SetActiveScreen("menu");
}

void TUI::SetActiveScreen(const std::string& newActive)
{
	if (newActive != "menu" && newActive != "sheet")
	{
		return;
	}

	activeScreen = newActive;
	SetConsoleActiveScreenBuffer(ActiveScreen().Handle());
	SetStdHandle(STD_OUTPUT_HANDLE, ActiveScreen().Handle());
}

Screen& TUI::ActiveScreen()
{
	if (activeScreen == "menu")
	{
		return sMenu;
	}

	return sSheet;
}

void TUI::ChangeScreenSize(int width, int height)
{
	ActiveScreen().ChangeSize(width, height);

	if (activeScreen != "sheet")
	{
		return;
	}
}

void TUI::DrawHeaders(const std::vector<std::shared_ptr<Cell>>& headers)
{
	for (const auto& header : headers)
	{
		DrawHeader(header);
	}
}

void TUI::DrawHeader(const std::shared_ptr<Cell>& header)
{
	DrawCell(true, header, B_DARKGRAY);
}

void TUI::DrawCell(bool header, const std::shared_ptr<Cell>& cell, const WORD& textAttr)
{
	auto formCell = std::dynamic_pointer_cast<FormulaCell>(cell);
	if (formCell) { formCell->Update(); }

	Position position = sheet.AddressToPosition(cell->GetAddress());
	Console::MoveCursor(position);

	SetConsoleTextAttribute(sSheet.Handle(), textAttr);

	Console::Write(cell->ValueToDraw());

	SetConsoleTextAttribute(sSheet.Handle(), sSheet.InitInfo().wAttributes);
}

void TUI::DrawCells(const std::vector<std::shared_ptr<Cell>>& cells)
{
	for (const auto& ptr : cells)
	{
		DrawCell(false, ptr, sSheet.InitInfo().wAttributes);
	}
}

void TUI::HighlightCell(const Position& position, const WORD& textAttr)
{
	Address address = sheet.PositionToAddress(position);
	Address bufferAddress = sheet.AddressToBufferAddress(address);
	auto& colHeaders = sheet.ColHeadersBuff();
	auto& rowHeaders = sheet.RowHeadersBuff();
	auto& values = sheet.Values();

	auto& cellPtr = values.contains(address) ? values.at(address) :
		std::make_shared<TextCell>(address, sheet.DefaultCellWidth(), sheet.DefaultCellHeight(), &sheet, "");

	display.ShowCellInfo(cellPtr);

	WORD attributes[255]{};
	for (int i = 0; i < 255; i++)
	{
		attributes[i] = textAttr;
	}

	WORD oppAttributes[255]{};
	for (int i = 0; i < 255; i++)
	{
		oppAttributes[i] = textAttr | COMMON_LVB_REVERSE_VIDEO;
	}

	COORD pos{ static_cast<short>(position.X()),  static_cast<short>(position.Y()) };
	DWORD numAttrWritten;
	WriteConsoleOutputAttribute(sSheet.Handle(), attributes, cellPtr->Width(), pos, &numAttrWritten);

	// Highlight Col Header

	const auto& col = colHeaders.at(bufferAddress.Col());
	Position colPos = sheet.BufferAddressToPosition(Address{ bufferAddress.Col(), -1});
	pos = COORD{ static_cast<short>(colPos.X()),  static_cast<short>(colPos.Y()) };
	WriteConsoleOutputAttribute(sSheet.Handle(), oppAttributes, col->Width(), pos, &numAttrWritten);

	// Highlight Row Header

	const auto& row = rowHeaders.at(bufferAddress.Row());
	Position rowPos = sheet.BufferAddressToPosition(Address{ -1, bufferAddress.Row()});
	pos = COORD{ static_cast<short>(rowPos.X()),  static_cast<short>(rowPos.Y()) };
	WriteConsoleOutputAttribute(sSheet.Handle(), oppAttributes, row->Width(), pos, &numAttrWritten);

	Console::MoveCursor(position);
}

void TUI::HighlightCurrent()
{
	HighlightCell(Console::CurrentCursor());
}

void TUI::UnhighlightCurrent()
{
	HighlightCell(Console::CurrentCursor(), F_DARKGRAY);
}

void TUI::Highlight(const Direction& direction, int distance)
{
	Address address = sheet.PositionToAddress(Console::CurrentCursor());

	if (direction & Left && address.Col() == 0 ||
		direction & Up && address.Row() == 0)
	{
		return;
	}

	Position currentCursor = Console::CurrentCursor();
	Address currBuffAddress = sheet.PositionToBufferAddress(currentCursor);

	UnhighlightCurrent();

	switch (direction)
	{
	case Right:
		if (currBuffAddress.Col() < sheet.ColHeadersBuff().size() - 1)
		{
			break;
		}
		ScrollTable(Left);
		HighlightCurrent();
		return;
	case Left:
		if (currBuffAddress.Col() > 0)
		{
			break;
		}
		ScrollTable(Right);
		HighlightCurrent();
		return;
	case Down:
		if (currBuffAddress.Row() < sheet.RowHeadersBuff().size() - 1)
		{
			break;
		}
		ScrollTable(Up);
		HighlightCurrent();
		return;
	case Up:
		if (currBuffAddress.Row() > 0)
		{
			break;
		}
		ScrollTable(Down);
		HighlightCurrent();
		return;
	}

	Console::MoveCursor(direction, distance);
	HighlightCurrent();
}

void TUI::ScrollTable(const Direction& direction)
{
	auto initCursor = Console::CurrentCursor();

	auto scrollArea = sheet.GetScrollableArea(direction);
	auto destination = sheet.GetScrollDestination(direction, scrollArea);

	CHAR_INFO charFill{};
	charFill.Attributes = F_DARKGRAY;
	charFill.Char.AsciiChar = ' ';

	ScrollConsoleScreenBuffer(sSheet.Handle(), &scrollArea, &scrollArea, destination, &charFill);

	std::shared_ptr<Cell> newHeader;
	std::vector<std::shared_ptr<Cell>> newCells;

	sheet.ScrollBuffer(direction);
	switch (direction)
	{
	case Right:
		newHeader = sheet.ColHeadersBuff().front();
		newCells = sheet.GetBufferedCells(true, true, 1);
		break;
	case Left:
		newHeader = sheet.ColHeadersBuff().back();
		newCells = sheet.GetBufferedCells(false, true, 1);
		break;
	case Down:
		newHeader = sheet.RowHeadersBuff().front();
		newCells = sheet.GetBufferedCells(true, false, 1);
		break;
	case Up:
		newHeader = sheet.RowHeadersBuff().back();
		newCells = sheet.GetBufferedCells(false, false, 1);
		break;
	}

	DrawHeader(newHeader);
	DrawCells(newCells);

	Console::MoveCursor(initCursor);
}

std::string TUI::ReadLimitedInput(const std::string& currInput)
{
	std::string input = currInput;
	/*DWORD numRead;
	char ch;
	while (true)
	{
		ReadConsoleA(GetStdHandle(STD_INPUT_HANDLE), &ch, 1, &numRead, NULL);*/

	auto initCursor = Console::CurrentCursor();
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD numRead;

	bool active = true;
	auto currentIndex = input.size();

	while (active)
	{
		INPUT_RECORD inBuffer[128]{};
		ReadConsoleInput(hInput, inBuffer, 128, &numRead);

		for (DWORD i = 0; i < numRead; i++)
		{
			if (inBuffer[i].EventType != KEY_EVENT)
			{
				break;
			}

			auto keyEvent = inBuffer[i].Event.KeyEvent;
			if (!keyEvent.bKeyDown)
			{
				break;
			}

			auto inputChar = keyEvent.uChar.AsciiChar;
			if (inputChar == '\0')
			{
				switch (keyEvent.wVirtualKeyCode)
				{
				case VK_RIGHT:
				{
					if (Console::CurrentCursor().X() >= static_cast<int>(input.size()))
					{
						break;
					}

					currentIndex += 1;
					Console::MoveCursor(Right, 1);
					break;
				}
				case VK_LEFT:
				{
					if (Console::CurrentCursor().X() <= 0)
					{
						break;
					}

					currentIndex -= 1;
					Console::MoveCursor(Left, 1);
					break;
				}
				}
			}

			if (inputChar == '\b')
			{
				auto currCursor = Console::CurrentCursor();

				if (currCursor.X() <= 0)
				{
					continue;
				}

				Console::Write("\b ");
				
				auto endSubstring = input.substr(currentIndex, input.size());
				auto substrSize = endSubstring.size();
				Console::Write(std::string(substrSize, ' '));
				Console::MoveCursor(Left, static_cast<int>(substrSize) + 1);
				Console::Write(endSubstring);
				Console::MoveCursor(Left, static_cast<int>(substrSize));

				input.erase(currentIndex - 1, 1);
				currentIndex -= 1;
			}

			if (isprint(inputChar) && input.size() < maxValueLength)
			{
				auto endSubstring = input.substr(currentIndex, input.size());
				auto substrSize = endSubstring.size();
				Console::Write(std::string(substrSize, ' '));
				Console::MoveCursor(Left, static_cast<int>(substrSize));
				Console::Write(std::string(1, inputChar));
				Console::Write(endSubstring);
				Console::MoveCursor(Left, static_cast<int>(substrSize));

				input.insert(currentIndex, 1, inputChar);
				currentIndex += 1;
			}

			if (inputChar == '\r')
			{
				active = false;
			}
		}

		bool success = FlushConsoleInputBuffer(hInput);
	}

	return input;
}

void TUI::UserInput()
{
	auto initCursor = Console::CurrentCursor();
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD consoleMode;
	GetConsoleMode(hInput, &consoleMode);
	consoleMode |= ENABLE_EXTENDED_FLAGS;
	SetConsoleMode(hInput, consoleMode);
	DWORD numRead;

	while (true)
	{
		COORD currentScreenSize = Console::ScreenSize();
		/*bool success = FlushConsoleInputBuffer(hInput);*/
		INPUT_RECORD inBuffer[128]{};
		ReadConsoleInput(hInput, inBuffer, 128, &numRead);

		for (DWORD i = 0; i < numRead; i++)
		{
			switch (inBuffer[i].EventType)
			{
			//case MOUSE_EVENT:
			//	ProcessMouseInput(inBuffer[i].Event.MouseEvent);
			//	break;
			case KEY_EVENT:
				ProcessKeyboardInput(inBuffer[i].Event.KeyEvent);
				break;
			case WINDOW_BUFFER_SIZE_EVENT:
				if (inBuffer[i + 1].EventType == WINDOW_BUFFER_SIZE_EVENT)
				{
					continue;
				}

				auto screenInfo = Console::ScreenInfo();
				auto windowSize = Console::ScreenInfo().srWindow;
				COORD newScreenSize{ windowSize.Right + 1, windowSize.Bottom + 1 };
				ProcessWindowSizeChange(currentScreenSize, newScreenSize);
				bool success = FlushConsoleInputBuffer(hInput);
				break;
			}
		}
	}
}

void TUI::ProcessKeyboardInput(const KEY_EVENT_RECORD& keyEvent)
{
	if (keyEvent.bKeyDown)
	{
		if (!(keyEvent.dwControlKeyState &
			(LEFT_ALT_PRESSED
				| LEFT_CTRL_PRESSED
				| RIGHT_ALT_PRESSED
				| RIGHT_CTRL_PRESSED
				| SHIFT_PRESSED)))
		{
			char inputChar = keyEvent.uChar.AsciiChar;


			if (isprint(inputChar))
			{
				auto position = Console::CurrentCursor();

				display.TextInput();
				INPUT_RECORD ir{};
				ir.EventType = KEY_EVENT;
				ir.Event.KeyEvent = keyEvent;

				auto handle = GetStdHandle(STD_INPUT_HANDLE);
				DWORD numWritten{};
				WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &ir, 1, &numWritten);
				
				auto address = sheet.PositionToAddress(position);
				AddContent(address);
				HighlightCell(position);
			}

			switch (keyEvent.wVirtualKeyCode)
			{
			case VK_RIGHT:
			{
				Highlight(Right, sheet.DefaultCellWidth());
				break;
			}
			case VK_LEFT:
			{
				Highlight(Left, sheet.DefaultCellWidth());
				break;
			}
			case VK_DOWN:
			{
				Highlight(Down, sheet.DefaultCellHeight());
				break;
			}
			case VK_UP:
			{
				Highlight(Up, sheet.DefaultCellHeight());
				break;
			}
			case VK_F2:
			{
				auto position = Console::CurrentCursor();
				auto address = sheet.PositionToAddress(position);
				auto& values = sheet.Values();
				if (!values.contains(address))
				{
					break;
				}

				auto& cell = values.at(address);
				display.ShowCellInfo(cell, true);

				std::string currContent;
				auto formCell = std::dynamic_pointer_cast<FormulaCell>(cell);
				currContent = formCell ? formCell->Expression() : cell->ValueString();
				AddContent(address, currContent);
				HighlightCell(position);
				break;
			}
			case VK_BACK:
			case VK_DELETE:
			{
				auto position = Console::CurrentCursor();
				auto address = sheet.PositionToAddress(position);
				DrawCells(sheet.AddCell(address, ""));
				HighlightCell(position);
				break;
			}

			case VK_ESCAPE:
			{
				ToggleActiveScreen();
			}
			}
		}

		if (keyEvent.dwControlKeyState &
			(LEFT_CTRL_PRESSED
			| RIGHT_CTRL_PRESSED))
		{
			switch (keyEvent.uChar.AsciiChar)
			{
			case 14: //CTRL + N
			{
				if (activeScreen == "sheet")
				{
					break;
				}
				InitSheetScreen();
				InitSheet();
				SetActiveScreen("sheet");
				break;
			}

			case 15: //CTRL + O
			{
				if (activeScreen == "sheet")
				{
					break;
				}

				ClearMenuLog();
				std::string filePath = ProvideFilePath();
				ClearMenuLog();

				std::ifstream sheetFile(filePath);

				if (!sheetFile.is_open())
				{
					Console::Write("File could not be opened.");
					break;
				}

				nlohmann::json input;
				sheetFile >> input;

				if (!sheetFile.good())
				{
					Console::Write("File could not be opened.");
					break;
				}

				if (!sheet.Deserialize(input))
				{
					Console::Write("File could not be opened.");
					break;
				}

				SetActiveScreen("sheet");
				break;
			}

			case 19: //CTRL + S
			{
				if (activeScreen == "sheet")
				{
					break;
				}

				ClearMenuLog();
				std::string filePath = ProvideFilePath();
				ClearMenuLog();

				std::ofstream sheetFile(filePath, std::ios::trunc);

				if (!sheetFile.is_open())
				{
					Console::Write("File path could not be accessed.");
					break;
				}

				nlohmann::json output = sheet.Serialize();
				sheetFile << output;

				break;
			}

			case 5: //CTRL+E
			{
				ExportDataToExcel();
				break;
			}
			}
		}
	}
}

void TUI::AddContent(const Address& address, const std::string& currContent)
{
	std::string input = ReadLimitedInput(currContent);
	auto cellsToRedraw = sheet.AddCell(address, input);
	DrawCells(cellsToRedraw);
}

void TUI::ProcessMouseInput(const MOUSE_EVENT_RECORD& mouseEvent)
{
	if (mouseEvent.dwEventFlags & MOUSE_WHEELED)
	{
		if (mouseEvent.dwControlKeyState & SHIFT_PRESSED)
		{
			int direction = mouseEvent.dwButtonState < 10000000 ? 1 : -1;
			if (direction < 0)
			{
				Highlight(Right, sheet.DefaultCellWidth());
			}
			if (direction > 0)
			{
				Highlight(Left, sheet.DefaultCellWidth());
			}
		}

		if (!(mouseEvent.dwControlKeyState & SHIFT_PRESSED))
		{
			int direction = mouseEvent.dwButtonState < 10000000 ? 1 : -1;
			if (direction < 0)
			{
				Highlight(Down, sheet.DefaultCellHeight());
			}
			if (direction > 0)
			{
				Highlight(Up, sheet.DefaultCellHeight());
			}
		}
	}
}

void TUI::ProcessWindowSizeChange(const COORD& currentScreenSize, const COORD& newScreenSize)
{
	auto minWidth = 120;
	auto minHeight = 30;

	if (newScreenSize.X < minWidth || newScreenSize.Y < minHeight)
	{
		auto resetWidth = std::max<int>(minWidth, currentScreenSize.X);
		auto resetHeight = std::max<int>(minHeight, currentScreenSize.Y);
		if (activeScreen == "menu")
		{
			InitMenuScreen(Area{ resetWidth, resetHeight });
			DisplayMenuInfo();
			return;
		}
		
		InitSheetScreen(Area{ resetWidth, resetHeight });
		InitSheet(sheet.Values());
		return;
	}

	auto info = ActiveScreen().Info();
	auto maxWindow = info.dwMaximumWindowSize;

	int newWidth = (maxWindow.X) <= newScreenSize.X ? maxWindow.X : newScreenSize.X;
	int newHeight = (maxWindow.Y) <= newScreenSize.Y ? maxWindow.Y : newScreenSize.Y;

	auto initActive = activeScreen;
	InitMenuScreen(Area{ newWidth, newHeight });
	DisplayMenuInfo();
	InitSheetScreen(Area{ newWidth, newHeight });
	InitSheet(sheet.Values());
	SetActiveScreen(initActive);
}

void TUI::DisplayMenuInfo()
{
	Console::Write("New Sheet (CTRL+N) | Open File (CTRL+O) | Save File (CTRL+S) | Export to Excel (CTRL+E)");
	Console::MoveCursor(Position{ 0, 2 });
}

void TUI::ClearMenuLog()
{
	Console::MoveCursor(Position{ 0, 2 });
	Console::Write(std::string(Console::ScreenSize().X, ' '));
	Console::Write(std::string(maxValueLength, ' '));
	Console::MoveCursor(Position{ 0, 2 });
}

std::string TUI::ProvideFilePath()
{
	auto currCursor = Console::CurrentCursor();
	Console::Write("File Path: ");
	Console::MoveCursor(Position{ 0, currCursor.Y() + 1 });
	return ReadLimitedInput();
}

void TUI::ExportDataToExcel()
{
	auto values = sheet.Values();
	for (const auto& pair : values)
	{
		auto cellPtr = std::get<1>(pair);
		std::string title = cellPtr->Title();
		auto formCellPtr = std::dynamic_pointer_cast<FormulaCell>(cellPtr);

		auto cell = excelApp->GetRange(title.c_str());
		cell->Value2 = formCellPtr ? formCellPtr->Expression().c_str() : cellPtr->ValueString().c_str();
	}
}

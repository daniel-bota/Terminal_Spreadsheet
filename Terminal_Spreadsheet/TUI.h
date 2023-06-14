#pragma once
#include "Screen.h"
#include "Sheet.h"
#include "Display.h"
#include "Color.h"
#include <string>
#include <memory>
#include <fstream>

class TUI
{
public:
	TUI();
private:
	const int defCellH{ 1 };
	const int defCellW{ 10 };
	int defRowHeaderW{ 0 };
	const int maxValueLength{ 255 };
	int displayH{ 5 };

	Screen sMenu;
	Screen sSheet;

	std::string activeScreen;

	Sheet sheet;

	Display display;
	void InitMenuScreen(Area newScreenSize = Area{ -1, -1 });
	void InitSheetScreen(Area newScreenSize = Area{ -1, -1 });
	void InitSheet
	(const std::map<Address, std::shared_ptr<Cell>>& imported = std::map<Address, std::shared_ptr<Cell>>{});
	void ToggleActiveScreen();
	void SetActiveScreen(const std::string& newActive);
	Screen& ActiveScreen();
	void ChangeScreenSize(int width, int height);

	void DrawHeaders(const std::vector<std::shared_ptr<Cell>>& headers);
	void DrawHeader(const std::shared_ptr<Cell>& header);

	void DrawCell(bool header, const std::shared_ptr<Cell>& cell, const WORD& textAttr = F_DARKGRAY);
	void DrawCells(const std::vector<std::shared_ptr<Cell>>& cells);
	void HighlightCell(Position position, const WORD& textAttr = B_DARKGRAY);
	void HighlightCurrent();
	void UnhighlightCurrent();
	void Highlight(Direction direction, int distance);
	void ScrollTable(Direction direction);

	std::string ReadLimitedInput(std::string currInput = "");
	void UserInput();
	void ProcessKeyboardInput(KEY_EVENT_RECORD keyEvent);
	void AddContent(Address address, std::string currContent = "");
	void ProcessMouseInput(MOUSE_EVENT_RECORD mouseEvent);
	void ProcessWindowSizeChange(const COORD& currentScreenSize, const COORD& newScreenSize);
	void DisplayMenuInfo();
	void ClearMenuLog();
	std::string ProvideFilePath();
};

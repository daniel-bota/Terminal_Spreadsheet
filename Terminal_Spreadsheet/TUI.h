#pragma once
#include "Screen.h"
#include "Sheet.h"
#include "Display.h"
#include "Color.h"
#include <string>
#include <memory>
#include <fstream>
#include <Windows.h>
#include <ole2.h>
#include <atlbase.h>

#import "C:\Program Files\Microsoft Office\Root\VFS\ProgramFilesCommonX64\Microsoft Shared\OFFICE16\MSO.DLL" \
    rename("RGB", "MSORGB")  \
    rename("DocumentProperties", "DocumentPropertiesXL") no_dual_interfaces
#import "C:\Program Files\Microsoft Office\Root\VFS\ProgramFilesCommonX86\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB"
#import "C:\Program Files\Microsoft Office\root\Office16\EXCEL.EXE" \
    exclude("IFont", "IPicture")  \
    rename( "DialogBox", "ExcelDialogBox" ) \
    rename( "RGB", "ExcelRGB" ) \
    rename( "CopyFile", "ExcelCopyFile" ) \
    rename( "ReplaceText", "ExcelReplaceText" )

class TUI
{
public:
	TUI();
	TUI(CComPtr<Excel::_Application> excelApp);
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

	CComPtr<Excel::_Application> excelApp = nullptr;

	Display display;
	void InitMenuScreen(const Area& newScreenSize = Area{ -1, -1 });
	void InitSheetScreen(const Area& newScreenSize = Area{ -1, -1 });
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
	void HighlightCell(const Position& position, const WORD& textAttr = B_DARKGRAY);
	void HighlightCurrent();
	void UnhighlightCurrent();
	void Highlight(const Direction& direction, int distance);
	void ScrollTable(const Direction& direction);

	std::string ReadLimitedInput(const std::string& currInput = "");
	void UserInput();
	void ProcessKeyboardInput(const KEY_EVENT_RECORD& keyEvent);
	void AddContent(const Address& address, const std::string& currContent = "");
	void ProcessMouseInput(const MOUSE_EVENT_RECORD& mouseEvent);
	void ProcessWindowSizeChange(const COORD& currentScreenSize, const COORD& newScreenSize);
	void DisplayMenuInfo();
	void ClearMenuLog();
	std::string ProvideFilePath();

	void ExportDataToExcel();
};

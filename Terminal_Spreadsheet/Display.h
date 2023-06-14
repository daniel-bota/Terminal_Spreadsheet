#pragma once
#include "Cell.h"
#include "FormulaCell.h"
#include "Position.h"
#include "Console.h"
#include <string>
#include <memory>
#include <format>
class Display
{
public:
	Display() = default;
	Display(int displayHeight);
	void Clear();
	void ShowCellInfo(const std::shared_ptr<Cell>& cell, bool edit = false);
	void TextInput();
private:
	int height{ 0 }, width{ 0 };
	Position cmdInfo{ 0, 0 }, cellInfo{ 0, 1 };
};


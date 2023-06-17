#pragma once
#include "windows.h"
#include "ColHeader.h"
#include "RowHeader.h"
#include "Direction.h"
#include "Cell.h"
#include "Formula.h"
#include "FormulaCell.h"
#include "TextCell.h"
#include "NumericCell.h"
#include "Area.h"
#include "Position.h"
#include "Address.h"
#include "Screen.h"
#include "Console.h"
#include <vector>
#include <map>
#include <memory>
#include <tuple>
#include <ranges>
#include <algorithm>
#include <stdexcept>
#include <ranges>
#include <nlohmann/json.hpp>

class Sheet : std::enable_shared_from_this<Sheet>
{
public:
	Sheet() = default;
	Sheet(int defCellH, int defCellW, int defRowHeaderW, int maxValueLength, int displayH);

	const std::vector<std::shared_ptr<Cell>>& ColHeadersBuff() const { return colHeadersBuff; }
	const std::vector<std::shared_ptr<Cell>>& RowHeadersBuff() const { return rowHeadersBuff; }
	void ScrollHeaders(const Direction& direction, int positions);
	const std::map<Address, std::shared_ptr<Cell>>& Values() const { return values; }

	void Initialize(const std::map<Address, std::shared_ptr<Cell>>& importedValues = {});

	void AddCell(std::shared_ptr<Cell> cell);
	std::vector<std::shared_ptr<Cell>> AddCell(const Address& address, const std::string& input);

	int DefaultCellWidth() { return defCellW; }
	int DefaultCellHeight() { return defCellH; }
	int DefaultRowHeaderWidth() { return defRowHeaderW; }
	std::vector<std::shared_ptr<Cell>> GetBufferedCells(bool outOfDateOnly = false) const;
	std::vector<std::shared_ptr<Cell>> GetBufferedCells(bool first, bool col, int number) const;

	SMALL_RECT GetScrollableArea(const Direction& scrollDirection);
	COORD GetScrollDestination(const Direction& scrollDirection, const SMALL_RECT& scrollArea);
	void ScrollBuffer(const Direction& direction);

	Position BufferAddressToPosition(const Address& buffAddress);
	Address BufferAddressToAddress(const Address& buffAddress);
	Address AddressToBufferAddress(const Address& address);
	Position AddressToPosition(const Address& address);
	Address PositionToBufferAddress(const Position& position);
	Address PositionToAddress(const Position& position);
	Address CellTitleToAddress(const std::string& title);
	int ColTitleToIndex(const std::string& colTitle);

	nlohmann::json Serialize();
	bool Deserialize(const nlohmann::json& source);
private:
	int defCellH{ 0 }, defCellW{ 0 }, defRowHeaderW{ 0 }, maxValueLength{ 0 }, displayH{ 0 };
	std::vector<std::shared_ptr<Cell>> colHeadersBuff{};
	std::vector<std::shared_ptr<Cell>> rowHeadersBuff{};
	SMALL_RECT tableArea{ 0, 0 };
	std::map<Address, std::shared_ptr<Cell>> values{};

	std::map<Address, std::vector<Address>> referencing{};
	std::tuple<int, int> InitDefaultHeaders();
	void InitHeaders(int numCols, int numRows);

	void GenerateHeaders(bool rowHeader, bool back, int number, std::vector<std::shared_ptr<Cell>>& headers);
	std::shared_ptr<Cell> GenerateHeader(bool rowHeader, int index);

	void ScrollHeaders(const Direction& direction);

	std::vector<std::shared_ptr<Cell>> GetBufferedCells
	(const std::vector<std::shared_ptr<Cell>>& cols, const std::vector<std::shared_ptr<Cell>>& rows, bool outOfDateOnly = false) const;
};


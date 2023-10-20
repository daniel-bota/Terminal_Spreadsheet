#include "Sheet.h"

Sheet::Sheet(int defCellH, int defCellW, int defRowHeaderW, int maxValueLength, int displayH) :
	defCellH(defCellH), defCellW(defCellW), defRowHeaderW(defRowHeaderW), maxValueLength(maxValueLength), displayH(displayH)
{
	std::tuple<int, int> headersSize = InitDefaultHeaders();

	short left = static_cast<short>(defRowHeaderW);
	short top = static_cast<short>(displayH + defCellH);
	short right = static_cast<short>(left + std::get<0>(headersSize) * defCellW - 1);
	short bottom = static_cast<short>(top + std::get<1>(headersSize) * defCellH - 1);
	tableArea = SMALL_RECT{ left, top, right, bottom };
}

std::tuple<int, int> Sheet::InitDefaultHeaders()
{
	int numCols = (Console::ScreenSize().X - defRowHeaderW) / defCellW;
	int numRows = (Console::ScreenSize().Y - displayH - defCellH) / defCellH;
	InitHeaders(numCols, numRows);

	return std::tuple(numCols, numRows);
}

void Sheet::InitHeaders(int numCols, int numRows)
{
	colHeadersBuff.clear();
	rowHeadersBuff.clear();
	GenerateHeaders(true, true, numRows, rowHeadersBuff);
	GenerateHeaders(false, true, numCols, colHeadersBuff);
}

std::vector<std::shared_ptr<Cell>> Sheet::AddCell(const Address& address, const std::string& input)
{
	std::vector<std::shared_ptr<Cell>> output{};
	std::shared_ptr<Cell> cellPtr = nullptr;
	if (values.contains(address))
	{
		cellPtr = values.at(address);

		auto formCellPtr = std::dynamic_pointer_cast<FormulaCell>(cellPtr);
		if (formCellPtr)
		{
			formCellPtr->Update(true);
		}

		if (input == "" && cellPtr->Referencing().empty())
		{
			cellPtr->NotifyReferencingCells(address);
			values.erase(address);
			output = GetBufferedCells(true);
			output.push_back(std::make_shared<TextCell>(address, defCellW, defCellH, this));
			return output;
		}
	}
	
	double number;
	if (input[0] == '=')
	{
		Formula formula(this, address, input);
		AddCell(std::make_shared<FormulaCell>(address, defCellW, defCellH, this, formula));
	}	
	else if (StringUtility::IsNumber(input, number))
	{
		AddCell(std::make_shared<NumericCell>(address, defCellW, defCellH, this, number));
	}
	else
	{
		AddCell(std::make_shared<TextCell>(address, defCellW, defCellH, this, input));
	}

	if (cellPtr != nullptr)
	{
		cellPtr->NotifyReferencingCells(address);
	}
	
	output = GetBufferedCells(true);
	output.push_back(values.at(address));

	return output;
}

void Sheet::GenerateHeaders(bool rowHeader, bool back, int number, std::vector<std::shared_ptr<Cell>>& headers)
{
	if (back)
	{
		int next;

		if (headers.empty())
		{
			next = 0;
		}
		else
		{
			next = headers.back()->Index() + 1;
		}

		for (int i = next; i < next + number; i++)
		{
			headers.push_back(GenerateHeader(rowHeader, i));
		}

		return;
	}

	int next = headers.front()->Index() - 1;
	for (int i = next; i > next - number && i >= 0; i--)
	{
		headers.insert(headers.begin(), GenerateHeader(rowHeader, i));
	}
}

std::shared_ptr<Cell> Sheet::GenerateHeader(bool rowHeader, int index)
{
	if (rowHeader)
	{
		return std::make_shared<RowHeader>(this, index, defRowHeaderW, defCellH);
	}

	return std::make_shared<ColHeader>(this, index, defCellW, defCellH);
}

void Sheet::Initialize(const std::map<Address, std::shared_ptr<Cell>>& importedValues)
{
	values.clear();
	InitDefaultHeaders();
	for (const auto& pair : importedValues)
	{
		auto formCell = std::dynamic_pointer_cast<FormulaCell>(pair.second);
		if (formCell)
		{
			AddCell(pair.first, formCell->Expression());
			continue;
		}
		AddCell(pair.first, pair.second->ValueString());
	}
}

void Sheet::AddCell(std::shared_ptr<Cell> cell)
{
	values[cell->GetAddress()] = cell;
}

void Sheet::ScrollHeaders(const Direction& direction)
{
	switch (direction)
	{
	case Left:
		colHeadersBuff.erase(colHeadersBuff.begin());
		return GenerateHeaders(false, true, 1, colHeadersBuff);
	case Right:
		colHeadersBuff.erase(colHeadersBuff.end() - 1);
		return GenerateHeaders(false, false, 1, colHeadersBuff);
	case Up:
		rowHeadersBuff.erase(rowHeadersBuff.begin());
		return GenerateHeaders(true, true, 1, rowHeadersBuff);
	case Down:
		rowHeadersBuff.erase(rowHeadersBuff.end() - 1);
		return GenerateHeaders(true, false, 1, rowHeadersBuff);
	}
}

std::vector<std::shared_ptr<Cell>> Sheet::GetBufferedCells
(const std::vector<std::shared_ptr<Cell>>& cols, const std::vector<std::shared_ptr<Cell>>& rows, bool outOfDateOnly) const
{
	std::vector<std::shared_ptr<Cell>> result;

	for (const auto& col : cols)
	{
		for (const auto& row : rows)
		{
			Address address(col->Index(), row->Index());
			if (!values.contains(address))
			{
				continue;
			}

			if (outOfDateOnly)
			{
				auto formCell = std::dynamic_pointer_cast<FormulaCell>(values.at(address));
				if (formCell && formCell->Valid())
				{
					result.push_back(formCell);
					continue;
				}
			}

			result.push_back(values.at(address));
		}
	}

	return result;
}

std::vector<std::shared_ptr<Cell>> Sheet::GetBufferedCells(bool outOfDateOnly) const
{
	return GetBufferedCells(colHeadersBuff, rowHeadersBuff, outOfDateOnly);
}

std::vector<std::shared_ptr<Cell>> Sheet::GetBufferedCells(bool first, bool col, int number) const
{
	long size;
	int startIndex;

	if (col)
	{
		size = static_cast<long>(colHeadersBuff.size());
		if (number > size) { number = size; }
		startIndex = first ? 0 : size - number;
		auto firstIt = colHeadersBuff.begin() + startIndex;
		std::vector<std::shared_ptr<Cell>> cols{};
		std::ranges::copy_n(firstIt, number, std::back_inserter(cols));
		return GetBufferedCells(cols, rowHeadersBuff);
	}

	size = static_cast<long>(rowHeadersBuff.size());
	if (number > size) { number = size; }
	startIndex = first ? 0 : size - number;
	auto firstIt = rowHeadersBuff.begin() + startIndex;
	std::vector<std::shared_ptr<Cell>> rows{};
	std::ranges::copy_n(firstIt, number, std::back_inserter(rows));
	return GetBufferedCells(colHeadersBuff, rows);
}

SMALL_RECT Sheet::GetScrollableArea(const Direction& scrollDirection)
{
	SMALL_RECT scrollArea = tableArea;
	scrollArea.Top = scrollDirection & Right || scrollDirection & Left ?
		tableArea.Top - defCellH : tableArea.Top;
	scrollArea.Left = scrollDirection & Up || scrollDirection & Down ?
		tableArea.Left - defRowHeaderW : tableArea.Left;

	return scrollArea;
}

COORD Sheet::GetScrollDestination(const Direction& scrollDirection, const SMALL_RECT& scrollArea)
{
	COORD destination{ scrollArea.Left, scrollArea.Top };
	switch (scrollDirection)
	{
	case Left:
		destination.X -= static_cast<short>(defCellW);
		break;
	case Right:
		destination.X += static_cast<short>(defCellW);
		break;
	case Up:
		destination.Y -= static_cast<short>(defCellH);
		break;
	case Down:
		destination.Y += static_cast<short>(defCellH);
		break;
	}

	return destination;
}

void Sheet::ScrollBuffer(const Direction& direction)
{
	ScrollHeaders(direction, 1);
}

Position Sheet::BufferAddressToPosition(const Address& buffAddress)
{
	int x = buffAddress.Col() == -1 ? 0 : defRowHeaderW + buffAddress.Col() * defCellW;
	int y = buffAddress.Row() == -1 ? displayH : displayH + (buffAddress.Row() + 1) * defCellH;

	return Position{ x, y };
}

Address Sheet::BufferAddressToAddress(const Address& buffAddress)
{
	int col = colHeadersBuff[buffAddress.Col()]->Index();
	int row = rowHeadersBuff[buffAddress.Row()]->Index();

	return Address{ col, row };
}

Address Sheet::AddressToBufferAddress(const Address& address)
{
	auto colIndex = address.Col() == -1 ? -1 :
		std::ranges::find_if(colHeadersBuff, [&address](const auto& col)
			{
				return col->Index() == address.Col();
			}) - colHeadersBuff.begin();

			auto rowIndex = address.Row() == -1 ? -1 :
				std::ranges::find_if(rowHeadersBuff, [&address](const auto& row)
					{
						return row->Index() == address.Row();
					}) - rowHeadersBuff.begin();

					if (colIndex == colHeadersBuff.size() || rowIndex == rowHeadersBuff.size())
					{
						throw std::invalid_argument("The cell is not in the buffer.");
					}

					return Address{ static_cast<int>(colIndex), static_cast<int>(rowIndex) };
}

Position Sheet::AddressToPosition(const Address& address)
{
	return BufferAddressToPosition(AddressToBufferAddress(address));
}

Address Sheet::PositionToBufferAddress(const Position& position)
{
	int col = (position.X() - defRowHeaderW) / defCellW;
	int row = (position.Y() - displayH - defCellH) / defCellH;

	return Address{ col, row };
}

Address Sheet::PositionToAddress(const Position& position)
{
	return BufferAddressToAddress(PositionToBufferAddress(position));
}

Address Sheet::CellTitleToAddress(const std::string& title)
{
	auto firstDigitIndex = std::ranges::find_if(title, [](const char& c) { return std::isdigit(c); }) - title.begin();
	std::string colTitle = title.substr(0, firstDigitIndex);
	for (char& c : colTitle)
	{
		c = toupper(c);
	}
	std::string rowTitle = title.substr(firstDigitIndex, title.size());

	int colIndex = ColTitleToIndex(colTitle);
	int rowIndex = std::stoi(rowTitle) - 1;

	return Address{ colIndex, rowIndex };
}

int Sheet::ColTitleToIndex(const std::string& colTitle)
{
	std::string input = colTitle;
	const int base = 26;
	int result = 0;
	for (char c : input)
	{
		input.erase(input.begin());
		result += (c - 'A' + 1) * static_cast<int>(pow(base, input.size()));
	}

	return result - 1;
}

nlohmann::json Sheet::Serialize()
{
	nlohmann::json output;

	for (auto it = values.begin(); it != values.end(); it++)
	{
		const std::pair<Address, std::shared_ptr<Cell>>& value = *it;

		auto cellTitle = std::get<1>(value)->Title();

		int column = std::get<0>(value).Col();
		int row = std::get<0>(value).Row();

		auto& cellPtr = std::get<1>(value);
		auto formCellPtr = std::dynamic_pointer_cast<FormulaCell>(cellPtr);
		auto cellValue = formCellPtr ?
			formCellPtr->Expression() : cellPtr->ValueString();

		
		output[cellTitle]["column"] = column;
		output[cellTitle]["row"] = row;
		output[cellTitle]["value"] = cellValue;
	}

	return output;
}

bool Sheet::Deserialize(const nlohmann::json& source)
{
	std::map<Address, std::string> initializerList{};

	for (const auto& element : source.items())
	{
		try
		{
			int col = source.at(element.key()).at("column");
			int row = source.at(element.key()).at("row");
			auto newAddress = Address(col, row);
			std::string value = source.at(element.key()).at("value");

			initializerList[newAddress] = value;
		}
		catch (const nlohmann::json::out_of_range&)
		{
			return false;
		}	
	}

	Initialize();

	for (const auto& pair : initializerList)
	{
		AddCell(std::get<0>(pair), std::get<1>(pair));
	}

	return true;
}



void Sheet::ScrollHeaders(const Direction& direction, int positions)
{
	for (int i = 0; i < positions; i++)
	{
		ScrollHeaders(direction);
	}
}

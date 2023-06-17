#include "TextCell.h"

TextCell::TextCell(const Address& address, Sheet* sheet) : Cell(address, sheet)
{
	value = std::string{ "" };
}

TextCell::TextCell(const Address& address, Sheet* sheet, const std::string& newValue) : Cell(address, sheet)
{
	value = newValue;
}

TextCell::TextCell(const Address& address, int width, int height, Sheet* sheet) : Cell(address, width, height, sheet)
{
	value = std::string{ "" };
}

TextCell::TextCell(const Address& address, int width, int height, Sheet* sheet, const std::string& newValue) : Cell(address, width, height, sheet)
{
	value = newValue;
}

std::string TextCell::ValueString() const
{
	return std::get<std::string>(value);
}

std::string TextCell::ValueToDraw() const
{
	return Misc::ValueToDraw(false, true, std::get<std::string>(value), width);
}

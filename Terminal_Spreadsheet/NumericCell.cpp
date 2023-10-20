#include "NumericCell.h"

NumericCell::NumericCell(const Address& address, Sheet* sheet) : Cell(address, sheet)
{
    value = static_cast<double>(0);
}

NumericCell::NumericCell(const Address& address, Sheet* sheet, double newValue) : Cell(address, sheet)
{
    value = newValue;
}

NumericCell::NumericCell(const Address& address, int width, int height, Sheet* sheet, double newValue) : Cell (address, width, height, sheet)
{
    value = newValue;
}

std::string NumericCell::ValueString() const
{
    return std::to_string(std::get<double>(value));
}

std::string NumericCell::ValueToDraw() const
{
    return StringUtility::ValueToDraw(false, false, ValueString(), width);
}

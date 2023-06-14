#include "NumericCell.h"

NumericCell::NumericCell(Address address, Sheet* sheet) : Cell(address, sheet)
{
    value = static_cast<double>(0);
}

NumericCell::NumericCell(Address address, Sheet* sheet, double newValue) : Cell(address, sheet)
{
    value = newValue;
}

NumericCell::NumericCell(Address address, int width, int height, Sheet* sheet, double newValue) : Cell (address, width, height, sheet)
{
    value = newValue;
}

std::string NumericCell::ValueString() const
{
    return std::to_string(std::get<double>(value));
}

std::string NumericCell::ValueToDraw() const
{
    return Misc::ValueToDraw(false, false, ValueString(), width);
}

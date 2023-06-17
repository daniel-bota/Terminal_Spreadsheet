#pragma once
#include "Cell.h"

class NumericCell : public Cell
{
public:
	NumericCell(const Address& address, Sheet* sheet);
	NumericCell(const Address& address, Sheet* sheet, double newValue);
	NumericCell(const Address& address, int width, int height, Sheet* sheet, double newValue);
	std::string ValueString() const override;
	std::string ValueToDraw() const override;
};


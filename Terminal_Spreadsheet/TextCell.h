#pragma once
#include "Cell.h"

class TextCell : public Cell
{
public:
	TextCell(Address address, Sheet* sheet);
	TextCell(Address address, Sheet* sheet, std::string newValue);
	TextCell(Address address, int width, int height, Sheet* sheet);
	TextCell(Address address, int width, int height, Sheet* sheet, std::string newValue);
	std::string ValueString() const override;
	std::string ValueToDraw() const override;
};


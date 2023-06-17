#pragma once
#include "Cell.h"

class RowHeader : public Cell
{
public:
	RowHeader(Sheet* sheet, int index);
	RowHeader(Sheet* sheet, int index, int width, int height);

	std::string ValueString() const override { return title; };
	std::string ValueToDraw() const override;
	int Index() const override { return index; }
private:
	int index;
};
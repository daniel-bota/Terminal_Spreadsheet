#pragma once
#include "Cell.h"

class ColHeader : public Cell
{
public:
	ColHeader(Sheet* sheet, int index);
	ColHeader(Sheet* sheet, int index, int width, int height);

	std::string ValueString() const override { return title; };
	std::string ValueToDraw() const override;
	int Index() const override { return index; }
private:
	int index;
};


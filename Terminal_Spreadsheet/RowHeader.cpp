#include "RowHeader.h"
#include "Sheet.h"

RowHeader::RowHeader(Sheet* sheet, int index) : Cell(sheet), index(index)
{
	address = Address{ -1, index };
	title = GenerateRowTitle(index);
	value = title;
}

RowHeader::RowHeader(Sheet* sheet, int index, int width, int height) : RowHeader(sheet, index)
{
	this->width = width;
	this->height = height;
}

std::string RowHeader::ValueToDraw() const
{
	return StringUtility::ValueToDraw(true, true, title, width);
}

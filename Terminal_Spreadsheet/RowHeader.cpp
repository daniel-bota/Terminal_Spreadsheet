#include "RowHeader.h"
#include "Sheet.h"

RowHeader::RowHeader(int index) : index(index)
{
	address = Address{ -1, index };
	title = GenerateRowTitle(index);
	value = title;
}

RowHeader::RowHeader(int index, int width, int height) : RowHeader(index)
{
	this->width = width;
	this->height = height;
}

std::string RowHeader::ValueToDraw() const
{
	return Misc::ValueToDraw(true, true, title, width);
}

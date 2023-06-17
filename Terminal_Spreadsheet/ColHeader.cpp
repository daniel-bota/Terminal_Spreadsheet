#include "ColHeader.h"
#include "Sheet.h"

ColHeader::ColHeader(Sheet* sheet, int index): Cell(sheet), index(index)
{
	address = Address{ index, -1 };
	title = GenerateColTitle(index);
	value = title;
}

ColHeader::ColHeader(Sheet* sheet, int index, int width, int height) : ColHeader(sheet, index)
{
	this->width = width;
	this->height = height;
}

std::string ColHeader::ValueToDraw() const
{
	return Misc::ValueToDraw(true, true, title, width);
}

#include "ColHeader.h"
#include "Sheet.h"

ColHeader::ColHeader(int index): index(index)
{
	address = Address{ index, -1 };
	title = GenerateColTitle(index);
	value = title;
}

ColHeader::ColHeader(int index, int width, int height) : ColHeader(index)
{
	this->width = width;
	this->height = height;
}

std::string ColHeader::ValueToDraw() const
{
	return Misc::ValueToDraw(true, true, title, width);
}

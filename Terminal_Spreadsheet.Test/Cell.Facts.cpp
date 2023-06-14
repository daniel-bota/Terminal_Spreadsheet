#include "pch.h"
#include "Sheet.h"

namespace CellFacts
{
	TEST(ColHeader, GeneratesColTitle)
	{
		ColHeader col(77);
		EXPECT_EQ("BZ", col.Title());
	}

	TEST(ColHeader, ReturnsValueToDrawByCellWidth)
	{
		ColHeader col(727, 5, 1);
		EXPECT_EQ(" AAZ ", col.ValueToDraw());
	}

	TEST(RowHeader, GeneratesRowTitle)
	{
		RowHeader row(7);
		EXPECT_EQ("8", row.Title());
	}
}
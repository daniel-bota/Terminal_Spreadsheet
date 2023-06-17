#include "pch.h"
#include "ColHeader.h"
#include "RowHeader.h"
#include "Sheet.h"
#include <memory>

namespace HeaderFacts
{
	Sheet sheet;

	TEST(ColHeader, HasValidSetterAndGetter)
	{
		ColHeader col(nullptr, 3);
		EXPECT_EQ(3, col.Index());
	}

	TEST(ColHeader, HasValidTitleGenerator)
	{
		ColHeader col(nullptr, 26);
		EXPECT_EQ("AA", col.Title());
	}
	
	TEST(RowHeader, HasValidTitleGenerator)
	{
		RowHeader row(nullptr, 108);
		EXPECT_EQ("109", row.Title());
	}
}
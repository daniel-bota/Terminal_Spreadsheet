#include "pch.h"
#include "Misc.h"

namespace MiscFacts
{
	TEST(Misc, CanSplitStringByDelimiter)
	{
		std::string input{ "twenty.two" };
		std::vector<std::string> result = Misc::Split(input, '.');

		EXPECT_EQ(std::vector<std::string>({ "twenty", "two" }), result);
	}

	TEST(Misc, CanTrimString)
	{
		std::string input{ "    twenty           " };
		auto result = Misc::Trim(input);

		EXPECT_EQ(std::string{ "twenty" }, result);
	}

	TEST(Misc, CanReturnValueByCellSize)
	{
		std::string result = "    3";
		EXPECT_EQ(result, Misc::ValueToDraw(false, false, "3", 5));

		result = "Tom  ";
		EXPECT_EQ(result, Misc::ValueToDraw(false, true, "Tom", 5));

		result = "Anoth";
		EXPECT_EQ(result, Misc::ValueToDraw(false, true, "Another day", 5));

		result = " A1  ";
		EXPECT_EQ(result, Misc::ValueToDraw(true, true, "A1", 5));
	}
	
	TEST(Misc, CanDetermineIfInputIsNumber)
	{
		double number = 0;
		EXPECT_FALSE(Misc::IsNumber(" 2353 62 asdas gaaea c", number));
		EXPECT_TRUE(Misc::IsNumber("   154      ", number));
	}
}
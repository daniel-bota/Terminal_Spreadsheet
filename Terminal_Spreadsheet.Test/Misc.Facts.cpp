#include "pch.h"
#include "StringUtility.h"

namespace StringUtilityFacts
{
	TEST(StringUtility, CanSplitStringByDelimiter)
	{
		std::string input{ "twenty.two" };
		std::vector<std::string> result = StringUtility::Split(input, '.');

		EXPECT_EQ(std::vector<std::string>({ "twenty", "two" }), result);
	}

	TEST(StringUtility, CanTrimString)
	{
		std::string input{ "    twenty           " };
		auto result = StringUtility::Trim(input);

		EXPECT_EQ(std::string{ "twenty" }, result);
	}

	TEST(StringUtility, CanReturnValueByCellSize)
	{
		std::string result = "    3";
		EXPECT_EQ(result, StringUtility::ValueToDraw(false, false, "3", 5));

		result = "Tom  ";
		EXPECT_EQ(result, StringUtility::ValueToDraw(false, true, "Tom", 5));

		result = "Anoth";
		EXPECT_EQ(result, StringUtility::ValueToDraw(false, true, "Another day", 5));

		result = " A1  ";
		EXPECT_EQ(result, StringUtility::ValueToDraw(true, true, "A1", 5));
	}
	
	TEST(StringUtility, CanDetermineIfInputIsNumber)
	{
		double number = 0;
		EXPECT_FALSE(StringUtility::IsNumber(" 2353 62 asdas gaaea c", number));
		EXPECT_TRUE(StringUtility::IsNumber("   154      ", number));
	}
}
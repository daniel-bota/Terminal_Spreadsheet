#include "pch.h"
#include "Sheet.h"

namespace FormulaFacts
{
	TEST(Formula, CanParseStringLiteral)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "=   \" literal\"");
		auto cellValue = std::get<std::string>(sheet.Values().at(address)->Value());
		EXPECT_EQ(" literal", cellValue);
	}

	TEST(Formula, CanParseNumberLiteral)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "=    32    ");
		auto& cell = sheet.Values().at(address);
		auto cellValueString = cell->ValueString();
		auto cellValue = cell->Value();
		EXPECT_EQ("32", cellValueString.substr(0, 2));
		EXPECT_EQ(32, std::get<double>(cellValue));
	}

	TEST(Formula, CanParseReferene)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address1(0, 0);
		sheet.AddCell(address1, "test  ");
		Address address2(1, 0);
		sheet.AddCell(address2, "=A1");
		auto& cell = sheet.Values().at(address2);
		EXPECT_EQ("test  ", std::get<std::string>(cell->Value()));

	}

	TEST(Formula, CanParseSum)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "test");
		address = Address(0, 1);
		sheet.AddCell(address, "15.5");
		address = Address(1, 0);
		sheet.AddCell(address, "test");
		address = Address(1, 1);
		sheet.AddCell(address, "34.5");

		address = Address(2, 0);
		sheet.AddCell(address, "=sum(a1;a2;b1;b2;50)");
		auto& cell = sheet.Values().at(address);
		EXPECT_EQ(100, std::get<double>(cell->Value()));
	}

	TEST(Formula, CanParseRefRangeFormula)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "test");
		address = Address(0, 1);
		sheet.AddCell(address, "15.5");
		address = Address(1, 0);
		sheet.AddCell(address, "test");
		address = Address(1, 1);
		sheet.AddCell(address, "34.5");

		address = Address(2, 0);
		sheet.AddCell(address, "=sum(a1:b2;a1:b2;50)");
		auto& cell = sheet.Values().at(address);
		EXPECT_EQ(150, std::get<double>(cell->Value()));
	}

	TEST(Formula, CanParseCount)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "test");
		address = Address(0, 1);
		sheet.AddCell(address, "15.5");
		address = Address(1, 0);
		sheet.AddCell(address, "test");
		address = Address(1, 1);
		sheet.AddCell(address, "34.5");

		address = Address(2, 0);
		sheet.AddCell(address, "=count(a1:b2)");
		auto& cell = sheet.Values().at(address);
		EXPECT_EQ(2, std::get<double>(cell->Value()));
	}

	TEST(Formula, CanParseAverage)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "test");
		address = Address(0, 1);
		sheet.AddCell(address, "15.5");
		address = Address(1, 0);
		sheet.AddCell(address, "test");
		address = Address(1, 1);
		sheet.AddCell(address, "34.5");

		address = Address(2, 0);
		sheet.AddCell(address, "=avg(a1;a2;b1;b2;49)");
		auto& cell = sheet.Values().at(address);
		EXPECT_EQ(33, std::get<double>(cell->Value()));
	}

	TEST(Formula, CanParseMinimum)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "test");
		address = Address(0, 1);
		sheet.AddCell(address, "15.5");
		address = Address(1, 0);
		sheet.AddCell(address, "test");
		address = Address(1, 1);
		sheet.AddCell(address, "34.5");

		address = Address(2, 0);
		sheet.AddCell(address, "=min(a1;a2;b1;b2;49)");
		auto& cell = sheet.Values().at(address);
		EXPECT_EQ(15.5, std::get<double>(cell->Value()));
	}

	TEST(Formula, CanParseMaximum)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "test");
		address = Address(0, 1);
		sheet.AddCell(address, "15.5");
		address = Address(1, 0);
		sheet.AddCell(address, "test");
		address = Address(1, 1);
		sheet.AddCell(address, "34.5");

		address = Address(2, 0);
		sheet.AddCell(address, "=max(a1;a2;b1;b2;49)");
		auto& cell = sheet.Values().at(address);
		EXPECT_EQ(49, std::get<double>(cell->Value()));
	}

	TEST(Formula, CanParseModulus)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "test");
		address = Address(0, 1);
		sheet.AddCell(address, "15");
		address = Address(1, 0);
		sheet.AddCell(address, "test");
		address = Address(1, 1);
		sheet.AddCell(address, "30.5");

		address = Address(2, 0);
		
		sheet.AddCell(address, "=mod(b2;a2)");
		auto value = sheet.Values().at(address)->Value();
		EXPECT_EQ(0.5, std::get<double>(value));

		sheet.AddCell(address, "=mod(b2;a1)");
		value = sheet.Values().at(address)->Value();
		EXPECT_TRUE(std::holds_alternative<std::monostate>(value));
	}

	TEST(Formula, CanParsePower)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "test");
		address = Address(0, 1);
		sheet.AddCell(address, "9");
		address = Address(1, 0);
		sheet.AddCell(address, "test");
		address = Address(1, 1);
		sheet.AddCell(address, "2");

		address = Address(2, 0);

		sheet.AddCell(address, "=POW(a2;b2)");
		EXPECT_EQ(81, std::get<double>(sheet.Values().at(address)->Value()));

		sheet.AddCell(address, "=pow(b2;a1)");
		EXPECT_TRUE(std::holds_alternative<std::monostate>(sheet.Values().at(address)->Value()));
	}

	TEST(Formula, CanParseLength)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "test");
		address = Address(0, 1);
		sheet.AddCell(address, "9");
		address = Address(1, 0);
		sheet.AddCell(address, "test");
		address = Address(1, 1);
		sheet.AddCell(address, "2");

		address = Address(2, 0);

		sheet.AddCell(address, "=len(a2;b2)");
		EXPECT_TRUE(std::holds_alternative<std::monostate>(sheet.Values().at(address)->Value()));

		sheet.AddCell(address, "=len(a1)");
		EXPECT_EQ(4, std::get<double>(sheet.Values().at(address)->Value()));
	}

	TEST(Formula, CanParseTrim)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "Trim      test");
		address = Address(0, 1);
		sheet.AddCell(address, "9");
		address = Address(1, 0);
		sheet.AddCell(address, "Trim    test   2   ");
		address = Address(1, 1);
		sheet.AddCell(address, "2");

		address = Address(2, 0);

		sheet.AddCell(address, "=trim(a2;b2)");
		EXPECT_TRUE(std::holds_alternative<std::monostate>(sheet.Values().at(address)->Value()));

		sheet.AddCell(address, "=trim(a1)");
		EXPECT_EQ("Trim test", std::get<std::string>(sheet.Values().at(address)->Value()));

		sheet.AddCell(address, "=trim(b1)");
		EXPECT_EQ("Trim test 2", std::get<std::string>(sheet.Values().at(address)->Value()));

		sheet.AddCell(address, "=trim(b2)");
		EXPECT_EQ("2.000000", std::get<std::string>(sheet.Values().at(address)->Value()));
	}
	
	TEST(Formula, CanParseConcat)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "Trim      test");
		address = Address(0, 1);
		sheet.AddCell(address, "9");
		address = Address(1, 0);
		sheet.AddCell(address, "Trim    test   2   ");
		address = Address(1, 1);
		sheet.AddCell(address, "2");

		address = Address(2, 0);

		sheet.AddCell(address, "=concat(a2;b2)");
		EXPECT_EQ("9.0000002.000000", std::get<std::string>(sheet.Values().at(address)->Value()));

		sheet.AddCell(address, "=concat(a1;b1)");
		EXPECT_EQ("Trim      testTrim    test   2   ", std::get<std::string>(sheet.Values().at(address)->Value()));

		sheet.AddCell(address, "=concat(a1:b2)");
		EXPECT_TRUE(std::holds_alternative<std::monostate>(sheet.Values().at(address)->Value()));
	}

	TEST(Formula, CircularDependencyShowsError)
	{
		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		sheet.AddCell(address, "1");
		address = Address(0, 1);
		sheet.AddCell(address, "9");
		address = Address(1, 0);
		sheet.AddCell(address, "2");
		address = Address(1, 1);
		sheet.AddCell(address, "3");

		auto address1 = Address(2, 0);
		auto address2 = Address(2, 1);
		address = Address(0, 0);

		sheet.AddCell(address1, "=sum(a1:b2)");
		sheet.AddCell(address2, "=c1");
		sheet.AddCell(address, "=c2");

		EXPECT_TRUE(std::holds_alternative<std::monostate>(sheet.Values().at(address)->Value()));
	}
}
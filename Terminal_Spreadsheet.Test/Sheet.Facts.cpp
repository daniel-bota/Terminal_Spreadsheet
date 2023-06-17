#include "pch.h"
#include "Sheet.h"
#include "ColHeader.h"
#include "RowHeader.h"

namespace SheetFacts
{
	TEST(Sheet, CanGenerateHeaders)
	{
		HANDLE handle = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(handle, COORD{ 127, 31 });
		SetStdHandle(STD_OUTPUT_HANDLE, handle);

		Sheet sheet(1, 20, 7, 255, 5);
		const auto& colHeaders = sheet.ColHeadersBuff();
		const auto& rowHeaders = sheet.RowHeadersBuff();

		std::string cellA1 = colHeaders[0]->Title();
		int colBuffSize = static_cast<int>(colHeaders.size());
		int rowBuffSize = static_cast<int>(rowHeaders.size());

		EXPECT_EQ("A", cellA1);
		EXPECT_EQ(6, colBuffSize);
		EXPECT_EQ(25, rowBuffSize);
	}

	TEST(Sheet, CanScrollHeadersBuffer)
	{
		HANDLE handle = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(handle, COORD{ 127, 31 });
		SetStdHandle(STD_OUTPUT_HANDLE, handle);

		Sheet sheet(1, 20, 7, 255, 5);
		std::string colA = sheet.ColHeadersBuff()[0]->Title();

		sheet.ScrollHeaders(Left, 3);
		std::string colD = sheet.ColHeadersBuff()[0]->Title();

		sheet.ScrollHeaders(Up, 3);
		std::string row28 = sheet.RowHeadersBuff().back()->Title();

		EXPECT_EQ("A", colA);
		EXPECT_EQ("D", colD);
		EXPECT_EQ("28", row28);

	}

	TEST(Sheet, CanImportValues)
	{
		HANDLE handle = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(handle, COORD{ 127, 31 });
		SetStdHandle(STD_OUTPUT_HANDLE, handle);

		Sheet sheet(1, 20, 7, 255, 5);
		bool initEmpty = sheet.Values().empty();

		Address address(0, 0);
		std::map<Address, std::shared_ptr<Cell>> imported;
		imported[address] = std::make_shared<TextCell>(address, &sheet);

		sheet.Initialize(imported);

		bool empty = sheet.Values().empty();
		std::string cellA1 = sheet.Values().at(address)->Title();

		EXPECT_TRUE(initEmpty);
		EXPECT_FALSE(empty);
		EXPECT_EQ("A1", cellA1);
	}

	TEST(Sheet, CanAddCell)
	{
		HANDLE handle = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(handle, COORD{ 127, 31 });
		SetStdHandle(STD_OUTPUT_HANDLE, handle);

		Sheet sheet(1, 20, 7, 255, 5);
		Address address(0, 0);
		auto cellPtr = std::make_shared<TextCell>(address, &sheet);
		sheet.AddCell(cellPtr);

		EXPECT_EQ(address, sheet.Values().at(address)->GetAddress());
		EXPECT_EQ(1, sheet.Values().size());
	}

	TEST(Sheet, CanGetAllBufferedCells)
	{
		HANDLE handle = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(handle, COORD{ 127, 31 });
		SetStdHandle(STD_OUTPUT_HANDLE, handle);

		Sheet sheet(1, 20, 7, 255, 5);
		sheet.AddCell(Address{ 1, 1 }, "");
		sheet.AddCell(Address{ 100, 2 }, "");
		sheet.AddCell(Address{ 2, 2 }, "");
		sheet.AddCell(Address{ 3, 3 }, "");
		sheet.AddCell(Address{ 4, 4 }, "");

		auto buffCells = sheet.GetBufferedCells();
		int size = static_cast<int>(buffCells.size());

		EXPECT_EQ(4, size);
	}

	TEST(Sheet, CanSerializeToJson)
	{
		HANDLE handle = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(handle, COORD{ 127, 31 });
		SetStdHandle(STD_OUTPUT_HANDLE, handle);

		Sheet sheet(1, 20, 7, 255, 5);
		sheet.AddCell(Address{ 1, 1 }, "25");
		sheet.AddCell(Address{ 100, 2 }, "someText");
		sheet.AddCell(Address{ 2, 2 }, "=sum(B2)");

		std::string output = sheet.Serialize().dump();
		std::string expected = "{\"B2\":{\"column\":1,\"row\":1,\"value\":\"25.000000\"},\"C3\":{\"column\":2,\"row\":2,\"value\":\"=sum(B2)\"},\"CW3\":{\"column\":100,\"row\":2,\"value\":\"someText\"}}";

		EXPECT_EQ(expected, output);
	}

	TEST(Sheet, CanDeserializeJson)
	{
		HANDLE handle = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(handle, COORD{ 127, 31 });
		SetStdHandle(STD_OUTPUT_HANDLE, handle);

		Sheet sheet(1, 20, 7, 255, 5);
		sheet.AddCell(Address{ 0, 0 }, "25");
		sheet.AddCell(Address{ 1, 0 }, "someText");
		auto output = sheet.Serialize();

		Sheet newSheet(1, 20, 7, 255, 5);
		newSheet.Deserialize(output);

		EXPECT_EQ("25.000000", newSheet.Values().at(Address{ 0, 0 })->ValueString());
		EXPECT_EQ("someText", newSheet.Values().at(Address{ 1, 0 })->ValueString());
	}
}
#include "Misc.h"

std::vector<std::string> Misc::Split(const std::string& input, char delimiter)
{
	std::vector<std::string> result;
	std::string substr;
	for (int i = 0; i < input.size(); i++)
	{
		char c = input[i];

		if (c == delimiter)
		{
			if (substr.empty())
			{
				continue;
			}
			result.push_back(substr);
			substr.clear();
			continue;
		}

		substr.push_back(c);

		if (i == input.size() - 1)
		{
			result.push_back(substr);
		}
	}

	return result;
}

std::string Misc::Trim(const std::string& input)
{
	int firstChar = static_cast<int>(input.find_first_not_of(' '));
	int lastChar = static_cast<int>(input.find_last_not_of(' '));
	return firstChar < 0? "" : input.substr(firstChar, lastChar - firstChar + 1);
}

std::string Misc::ValueToDraw(bool header, bool text, std::string value, int cellW)
{
	int visible = value.size() <= cellW ? static_cast<int>(value.size()) : cellW;
	int emptySpace = std::abs(cellW - visible);
	int emptyLeftSide;
	if (header)
	{
		emptyLeftSide = emptySpace / 2;
	}
	else
	{
		emptyLeftSide = text ? 0 : emptySpace;
	}
	int emptyRightSide = emptySpace - emptyLeftSide;

	return std::format("{}{}{}", std::string(emptyLeftSide, ' '), value.substr(0, visible), std::string(emptyRightSide, ' '));
}

bool Misc::IsNumber(std::string input, double& output)
{
	std::string result = Misc::Trim(input);
	size_t charsProcessed;
	try
	{
		output = std::stod(result, &charsProcessed);
	}
	catch (const std::invalid_argument&)
	{
		output = 0;
		return false;
	}

	return charsProcessed == result.size();
}

std::string Misc::GenerateRowTitle(int index)
{
	return std::to_string(index + 1);
}

std::string Misc::GenerateColTitle(int index)
{
	const int noZeroBase = 26;
	int charPosition = index + 1;
	std::string colTitle{ "" };

	while (charPosition > 0)
	{
		int currPosition = charPosition % noZeroBase == 0 ? 26 : charPosition % noZeroBase;
		colTitle = std::format("{}{}", std::string(1, ('A' - 1 + currPosition)), colTitle);
		charPosition -= currPosition;
		charPosition /= 26;
	}
	return colTitle;
}
